/*
	Titulo: Simulacao de Disco Magnetico
	Disciplina: Organização de Arquivos – Turma C
	Semestre: 2/2016
	Prof.: Andre Drummond

	Alunos:
	- Renato Nobre 15/0146696
	- Khalil Carsten 15/0134495

	Resumo da disposicao e organizacao do codigo:


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define GREEN   "\x1b[32m"
#define RESET   "\x1b[0m"
#define RED     "\x1b[31m"

#define CLUSTER 4
#define TRILHA_SUPERF  10
#define SEEK_T_MEDIO 4
#define SEEK_T_MINIMO 1
#define T_MEDIO_LAT 6
#define TEMPO_TRANSF 12
#define TRUE 1
#define FALSE 0

FILE *fp;
int numb_files = -1;
int total_time = 0;


/* ------------------------------------------------------ */
/* COMECO DAS ESTRUTURAS PARA ORGANIZACAO DO DISCO RIGIDO */
/* ------------------------------------------------------ */


typedef struct block{
	/*
		Bloco
		TAM_SETOR = 512
	*/
	unsigned char bytes_s[512];
}block;

typedef struct sector_array{
	/*
		Setor
		SETORES_TRILHA = 60 setor/trilha
	*/
	block sector[60];
}sector_array;

typedef struct track_array{
	/*
		Trilha
		TRILHA_CILINDRO  = 5 trilha/cilindro
	*/
	sector_array track[5];
}track_array;


typedef struct fatlist_s{
	/* Tabela FAT */
	char file_name[100];
	unsigned int first_sector;
}fatlist;
fatlist *fat_list = NULL;

typedef struct fatent_s{
	/* Struct de setores*/
	unsigned int used;
	unsigned int eof;
	unsigned int next;
}fatent;
fatent *fat_ent;
/* --------------------------------------------------- */
/* FIM DAS ESTRUTURAS PARA ORGANIZACAO DO DISCO RIGIDO */
/* --------------------------------------------------- */


/* --------------------------------- */
/* COMECO DAS FUNCOES PARA ALOCACAO  */
/* --------------------------------- */
track_array *allocCylinder(){
	/*
		Aloca 10 cilindros porque:
		Qtd de cilindro = numeros de trila por superfice
	*/
	int i, j;

	track_array *new_track_array =
		(track_array*)malloc(sizeof(track_array)*10);

	return new_track_array;
}

void allocFatList(char file_name[], int pos_inicial){
	/*
		Re-aloca memoria para a estrutura de FatList de acordo com o necessario

		Param:
		- file_name[]: nome do arquivo respectivo a fatList
		- pos_inicial: setor de inicio do arquivo
	*/
	fat_list = (fatlist*)realloc(fat_list, (numb_files+1)*sizeof(fatlist));

	strcpy(fat_list[numb_files].file_name, file_name);
	fat_list[numb_files].first_sector = pos_inicial;
}

void allocFatEnt(int used ,int eof ,int next, int sector){
	/*
		Popula a FatEnt.

		Param:
		- used: Se a area esta sendo usada ou nao
		- eof: fim de arquivo TRUE ou FALSE
		- next: proximo setor
		- sector: setor respectivo da fatent
	*/
	fat_ent[sector].used = used;
	fat_ent[sector].eof = eof;
	fat_ent[sector].next = next;
}
/* ------------------------------ */
/* FIM DAS FUNCOES PARA AlOCACAO  */
/* ------------------------------ */


/* --------------------------------- */
/* COMECO DAS FUNCOES DE CONVERSAO   */
/* --------------------------------- */
void oneToThree(int index, int cyl_trk_sec[]){
	/*
		Converte o valor de setor bruto para sua posicao
		cilindro/trilha/setor

		Param:
		-index: int de setor bruto
		-cyl_trk_sec: array de inteiros para o cilindro/trilha/setor

	*/
	int t;
	int s;
    int c = index/300;

    index -= 300*c;
    if((index/60) >= 5){
        t = ((index/60)/5)-1;
    }else{
        t = (index/60);
    }
    s = index%60;


    cyl_trk_sec[0] = c;
    cyl_trk_sec[1] = t;
    cyl_trk_sec[2] = s;
}

int threeToOne(int c, int t, int s){
	/*
		Converte o valor de sua posicao
		cilindro/trilha/setor para o setor bruto

		Param:
		- c: numero do cilindro
		- t: numero da trilha
		- s: numero do setor

	*/
	int index = (300 * c) + (60 * t) + s;

	return index;
}
/* --------------------------------- */
/* FIM DAS FUNCOES DE CONVERSAO      */
/* --------------------------------- */
int sizeOfFile(){
	/*
	Retorna o tamanho do arquivo em bytes
	Observação: o 'size' sempre
	tera alguns bytes a mais devido ao
	'\n' ao final do arquivo e das linhas.
	*/
	int size;

	fseek(fp, 0, SEEK_END); /* Leva o ponteiro para o final do arquivo */
	size = ftell(fp); /* Retorna a posição do ponteiro dentro do arquivo */

	return size;
}



int searchFatList(int cyl_trk_sec[]){
	int i, j, k;
	i = j = k = 0;

	while(fat_ent[i].used == TRUE){
		i++;
		if(i % 60 == 0){
			j++;
			i = j*300;
		}
		if(i % 3000 == 0){
			k++;
			i = k*60;
		}
	}

	if(i < 300){
		total_time += SEEK_T_MEDIO * (int)(i/300);
	}

 	return i;

}

void readFile(char file_name[], track_array *cylinder){
	int i = 0, sec, j = 0, numb_sectors = 1, read_sector = 0, fp_size, l = 0;
	char file_name_2[100];
	int cyl_trk_sec[] = {0, 0, 0};

	while(strcmp(fat_list[i].file_name, file_name) != 0 && (i <= numb_files)){
		i++;
	}
	sec = fat_list[i].first_sector;

	while(fat_ent[sec].eof == FALSE){
		numb_sectors++;
		sec = fat_ent[sec].next;
	}

	if(i <= numb_files){
		strcpy(file_name_2, fat_list[i].file_name);
		fp = fopen(file_name_2, "r+");
		fp_size = sizeOfFile();
	}

	if(strcmp(fat_list[i].file_name, file_name) != 0){
		printf(RED "Arquivo Inexistente\n" RESET);
	} else {
		fp = fopen("saida.txt", "w+");
		sec = fat_list[i].first_sector;

		while(l < fp_size){
			oneToThree(sec, cyl_trk_sec);
			while(j < 512 && l < fp_size){
				fprintf(fp, "%c", cylinder[cyl_trk_sec[0]]
			      .track[cyl_trk_sec[1]].sector[cyl_trk_sec[2]].bytes_s[j]);
				j++;
				l++;
			}

			j = 0;
			sec = fat_ent[sec].next;
			read_sector++;
		}
		fclose(fp);
	}
}

void arquivoExiste(char file_name[]){
	/*
		Funcao para verificacao da existencia do arquivo passado
		por parametro, realiza-se o teste de consistencia para o mesmo.
	*/
	int existe = FALSE;

	printf("Por favor, escreva o nome do arquivo: ");
	scanf("%s", file_name);

	while(existe == FALSE){
		if(fopen(file_name, "r+") == NULL){
			printf(RED "O Arquivo nao existe!\n" RESET);
			printf("Ponha outro nome ou crie o arquivo\n");
			scanf("%s", file_name);
			existe = FALSE;
		}else{
			printf(GREEN "Arquivo encontrado!!!\n" RESET);
			existe = TRUE;
		}
	}
}

void escreverArquivo(char file_name[], track_array *cylinder){
	double cluster_needed;
	double fp_size;
	int cyl_trk_sec[] = {0, 0, 0};
	int pos_inicial, i, written_sector = 0, j, next_sector, actual_sector;

	fp = fopen(file_name, "r+");
	fp_size = sizeOfFile();

	printf(" - Nome do arquivo:" RED " %s\n" RESET, file_name);
	printf(" - Tamanho do Arquivo digitado:" RED " %.0f bytes\n"RESET, fp_size);

	cluster_needed = ceil(fp_size / (CLUSTER * 512));
	printf(" - O arquivo necessitará de" RED " %.0lf cluster(s)\n"
	 		RESET, cluster_needed);

	fclose(fp);

	pos_inicial = searchFatList(cyl_trk_sec);
	allocFatList(file_name, pos_inicial);
	oneToThree(pos_inicial, cyl_trk_sec);

	fp = fopen(file_name, "r+");

	while(written_sector < (cluster_needed*4)){
		/* Conta até o written_sector chegar ao número de setores necessários
		   multiplo de 4 */

		actual_sector = pos_inicial;
		/* Guarda o setor atual pois pos_incial se altera
		   no decorrer do algoritmo*/

		for(i = 0; i < 512; i++){
		/* Loop para leitura do arquivo */
			fscanf(fp, "%c", &cylinder[cyl_trk_sec[0]].track[cyl_trk_sec[1]]
				.sector[cyl_trk_sec[2]].bytes_s[i]);
		}
		written_sector++;
		/* soma 1 nos setores ja escritos(tanto faz o local) */

		if(written_sector % 4 == 0){
			/*
			Aqui eh checado se o algoritmo escreveu 1 cluster, se sim ele
			soma 57.(57 = 60 - 4 + 1) Em que somar 60 pula para o cluster
			de baixo e se reduz 4 para voltar ao inicio do cluster para
			assim somar 1 Deus sabe pq.
			*/
			j = 0;
			cyl_trk_sec[2] += 57;
			while(fat_ent[cyl_trk_sec[2]].used != FALSE){
				/* Checa se o cluster logo abaixo esta vago */
				j++;
				cyl_trk_sec[2]++;
				if(j == 4){
				/* Caso todo o cluster abaixo esteja ocupado,
				   ele pula para o de baixo */
					cyl_trk_sec[2] += 57;
				}
			}
			pos_inicial = cyl_trk_sec[2];
		} else {
			/* Caso nao tenha escrito um cluster pos_inicial++ normalmente */
			pos_inicial++;
			cyl_trk_sec[2] = pos_inicial;
		}

		next_sector = pos_inicial;
		allocFatEnt(TRUE, FALSE, next_sector, actual_sector);
	}
	fat_ent[actual_sector].eof = TRUE;
	/* Esta linha atribui eof no ultimo setor escrito.
	   Melhor aqui do que com um if dentro do loop. */
}

void deleteFile(char file_name[]){
	int i = 0, sec;

	while(strcmp(fat_list[i].file_name, file_name) != 0 && i < numb_files){
		i++;
	}

		if(strcmp(fat_list[i].file_name, file_name) != 0){
			printf(RED "O arquivo nao existe!!\n" RESET);
		}else{
			sec = fat_list[i].first_sector;

			while(fat_ent[sec].eof != 1){
				fat_ent[sec].used = 0;
				sec = fat_ent[sec].next;
			}
			fat_ent[sec].used = 0;
			fat_ent[sec].eof = 0;

			strcpy(fat_list[i].file_name, "0");
		}
	}

void printFatTable(){
	/*
		Funcao para imprimir a tabela FAT
	*/
	int i = 0;
	int fp_size;
	int sec;
	char file_name[100];

	printf("========================================================\n");
	printf("NOME:            TAMANHO EM DISCO            LOCALIZACAO\n");
	while(i <= numb_files){
		if(strcmp(fat_list[i].file_name, "0") != 0){
			strcpy(file_name, fat_list[i].file_name);
			fp = fopen(file_name, "r+");
			fp_size = sizeOfFile();
			fclose(fp);

			printf("%s%12d Bytes                  ", file_name, fp_size);
			sec = fat_list[i].first_sector;

			while(fat_ent[sec].eof != TRUE){
				printf("%d,", sec);
				sec = fat_ent[sec].next;
			}
			printf("%d,", sec);
			printf("\n");
		}
		i++;
	}
	printf("========================================================\n");
}


int menu(){
	/*
		Funcao para impressao do menu principal retorna a escolha
		feita pelo usuario
	*/
	int escolha;

	printf("\n1 - Escrever Arquivo\n");
	printf("2 - Ler Arquivo\n");
	printf("3 - Apagar Arquivo\n");
	printf("4 - Mostrar Tabela FAT\n");
	printf("5 - Sair\n");
	printf("\n>>> ");
	scanf("%d", &escolha);
	while((escolha < 1)||(escolha > 5)){
		printf("Favor escolher um valor entre 1 e 5\n");
		printf(">>> ");
		scanf("%d", &escolha);
	}
	return escolha;
}


int main(){

	int opc = 0;
	int fp_size;
	char file_name[100];
	track_array *cylinder = allocCylinder();
	fat_ent = (fatent*)malloc(30000*sizeof(fatent));

	while(opc != 5){
		opc = menu();

		if(opc == 1){
			arquivoExiste(file_name);
			numb_files++;
			escreverArquivo(file_name, cylinder);
		}else if(opc == 2){
			printf("Por favor, escreva o nome do arquivo para leitura: ");
			scanf("%s", file_name);
			readFile(file_name, cylinder);

		}else if(opc == 3){
			printf("Por favor, escreva o nome do arquivo para ser deletado: ");
			scanf("%s", file_name);
			deleteFile(file_name);
		}else if(opc == 4){
			printFatTable();
		}
	}


	return 0;
}
