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

/*
	Bloco
	 TAM_SETOR = 512
*/
typedef struct block{
	unsigned char bytes_s[512];
}block;

/*
	Setor
	SETORES_TRILHA = 60 setor/trilha
*/
typedef struct sector_array{
	block sector[60];
}sector_array;

/*
	Trilha
	TRILHA_CILINDRO  = 5 trilha/cilindro
*/
typedef struct track_array{
	sector_array track[5];
}track_array;


/* Tabela FAT */
typedef struct fatlist_s{
	char file_name[100];
	unsigned int first_sector;
}fatlist;
fatlist *fat_list = NULL;

/* Struct de setores*/
typedef struct fatent_s{
	unsigned int used;
	unsigned int eof;
	unsigned int next;
}fatent;
fatent *fat_ent;

track_array *allocCylinder(){
	int i, j;

	track_array *new_track_array =
		(track_array*)malloc(sizeof(track_array)*10);

	for(i = 0; i < 5; i++){
		new_track_array->track[i] =
			*(sector_array*)malloc(sizeof(sector_array));

		for(j = 0; j < 60; j++){
			new_track_array->track[i].sector[j] =
				*(block*)malloc(sizeof(block));
		}
	}

	return new_track_array;
}

void allocFatList(char file_name[], int pos_inicial){
	fat_list = (fatlist*)realloc(fat_list, numb_files+1*sizeof(fatlist));

	strcpy(fat_list[numb_files].file_name, file_name);
	fat_list[numb_files].first_sector = pos_inicial;

	printf("File %s\n", fat_list[numb_files].file_name);
}

void allocFatEnt(int used ,int eof ,int next, int sector){
	fat_ent[sector].used = used;
	fat_ent[sector].eof = eof;
	fat_ent[sector].next = next;
}

void oneToThree(int index, int cyl_trk_sec[]){
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
	int index = (300 * c) + (60 * t) + s;

	return index;
}

int searchFatList(int cyl_trk_sec[]){
	int i = 0, j = 0, k = 0;

	if(fat_list == NULL){
		cyl_trk_sec[0] = 0;
		cyl_trk_sec[1] = 0;
		cyl_trk_sec[2] = 0;
	}

	while(fat_ent[i].used != FALSE){
		if(i%60 == 0){
			j++;
			i = j*300;
		}
		if(i%3000 == 0){
			k++;
			i = k*60;
		}
		i++;
	}

	return i;
}

void readFile(char file_name[], track_array *cylinder){
	int i = 0, sec, j = 0;
	int cyl_trk_sec[] = {0, 0, 0};

	while((fat_list[i].file_name != file_name)&&(i<numb_files)){
		i++;
	}

	if(i>numb_files){
		printf("Arquivo Inexistente");
	} else {
		fp = fopen("saida.txt", "w+");
		sec = fat_list[i].first_sector;
		while(fat_ent[sec].eof != TRUE){
			oneToThree(sec, cyl_trk_sec);
			while((j < 512)&&(!feof(fp))){
				if(!feof(fp)){
					fprintf(fp, "%c", cylinder[cyl_trk_sec[0]].track[cyl_trk_sec[1]].sector[cyl_trk_sec[2]].bytes_s[j]);
					j++;
				}
			}
			j = 0;
			printf("%d", sec);
			sec = fat_ent[sec].next;

		}
		printf("Foi?");
		fclose(fp);
	}
}


int menu(){
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

void arquivoExiste(char file_name[]){
	int existe = FALSE;

	printf("Por favor, escreva o nome do arquivo: ");
	scanf("%s", file_name);

	while(existe == FALSE){
		if(fopen(file_name, "r+") == NULL){
			printf("O Arquivo nao existe!\n");
			printf("Ponha outro nome ou crie o arquivo\n");
			scanf("%s", file_name);
			existe = FALSE;
		}else{
			printf("Arquivo encontrado!!!\n");
			existe = TRUE;
		}
	}
}


int sizeOfFile(){ /* Retorna o tamanho do arquivo em bytes */
	/* Observação: o 'size' sempre
	   tera alguns bytes a mais devido ao
	   '\n' ao final do arquivo e das linhas.*/
	int size;

	fseek(fp, 0, SEEK_END); /* Leva o ponteiro para o final do arquivo */
	size = ftell(fp); /* Retorna a posição do ponteiro dentro do arquivo */
	return size;
}

void escreverArquivo(char file_name[], track_array *cylinder){
	double cluster_needed;
	double fp_size;
	int cyl_trk_sec[] = {0, 0, 0};
	int pos_inicial, i;

	fp = fopen(file_name, "r+");
	fp_size = sizeOfFile();
	printf("Tamanho do Arquivo digitado: %f bytes\n", fp_size);
	cluster_needed = ceil(fp_size / (CLUSTER * 512));
	printf("O arquivo necessitará de %.0lf clusters\n", cluster_needed);
	fclose(fp);
	pos_inicial = searchFatList(cyl_trk_sec);
	allocFatList(file_name, pos_inicial);
	oneToThree(pos_inicial, cyl_trk_sec);


	fp = fopen(file_name, "r+");
	while(!feof(fp)){
		if(!feof(fp)){
			allocFatEnt(TRUE, FALSE, pos_inicial+1, pos_inicial);
		}
		for(i = 0; i < 512; i++){
			fscanf(fp, "%c", &cylinder[cyl_trk_sec[0]].track[cyl_trk_sec[1]]
				.sector[cyl_trk_sec[2]].bytes_s[i]);

		}
		pos_inicial += 1;
		cyl_trk_sec[2] = pos_inicial;
	}
	allocFatEnt(TRUE, TRUE, pos_inicial+1, pos_inicial);

}

void printFatTable(){
	int i = 0;
	int fp_size;
	int sec;
	char file_name[100];

	printf("========================================================\n");
	printf("NOME:            TAMANHO EM DISCO            LOCALIZACAO\n");
	while(i <= numb_files){
		strcpy(file_name, fat_list[i].file_name);
		fp = fopen(file_name, "r+");
		fp_size = sizeOfFile(fp);
		fclose(fp);

		printf("%s%12d Bytes                  ", file_name, fp_size);
		sec = fat_list[i].first_sector;

		while(fat_ent[sec].eof != TRUE){
			printf("%d,", sec);
			sec = fat_ent[sec].next;
		}
		printf("\n");
		i++;
	}
	printf("========================================================\n");
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

		}else if(opc == 4){
			printFatTable();
		}
	}


	return 0;
}
