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

/* Struct de setores*/
typedef struct fatent_s{
	unsigned int used;
	unsigned int eof;
	unsigned int next;
}fatent;

void searchFatlist(){

}

void allocFatList(int sector, int track, fatlist **fat_list, char file_name[]){

	*fat_list = (fatlist*)realloc(*fat_list, sizeof(fatlist));

	strcpy((*fat_list)[sector].file_name, file_name);
	(*fat_list)[sector].first_sector = (track * 60) + sector;

	printf("File %s\n", (*fat_list)[sector].file_name);
}

//
// void createFatEnt(int sector, fatent fat_ent){
//
// 	fatent *new_fatent = (fatent*)malloc(sizeof(fatent));
//
// 	new_fatent->used = TRUE;
// 	new_fatent
//
// 	fat_ent[sector] = new_fatent;
//
// }

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


int sizeOfFile(){ /* Retorna o tamanho do arquivo em bytes */
	/* Observação: o 'size' sempre
	   tera alguns bytes a mais devido ao
	   '\n' ao final do arquivo e das linhas.*/
	int size;

	fseek(fp, 0, SEEK_END); /* Leva o ponteiro para o final do arquivo */
	size = ftell(fp); /* Retorna a posição do ponteiro dentro do arquivo */
	return size;
}

void dividirArquivo(char file_name[], double cluster_needed){
	char atual;
	int i;

	fp = fopen(file_name, "r");
	while(feof(fp)){
		for(i=0;i<512*CLUSTER*cluster_needed;i++){
			atual = fgetc(fp);
			if(!feof(fp)){
				atual = fgetc(fp);
				/*cylinder.track[0].sector[0].block_s[i] = atual*/
			}
			printf("%c", atual);
		}
	}
	fclose(fp);

}


void escreverArquivo(char file_name[]){
	double fp_size;
	double cluster_needed;

	fp = fopen(file_name, "r");
	fp_size = sizeOfFile();
	printf("Tamanho do Arquivo digitado: %.0lf bytes\n", fp_size);
	cluster_needed = ceil(fp_size / (CLUSTER * 512));
	printf("O arquivo necessitará de %.0lf clusters\n", cluster_needed);
	fclose(fp);
	dividirArquivo(file_name, cluster_needed);
}



int main(){

	int opc, s;
	char file_name[100];
	fatlist *fat_list = NULL;

	opc = menu();

	if(opc == 1){
		printf("Por favor, escreva o nome do arquivo: ");
		scanf(" %s", file_name);
		escreverArquivo(file_name);
		allocFatList(0, 1, &fat_list, file_name);
		allocFatList(2, 0, &fat_list, "teste3.txt");

	}else if(opc == 2){

	}else if(opc == 3){

	}else if(opc == 4){

	}

	return 0;
}
