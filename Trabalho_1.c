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


#define CLUSTER 4
#define TRILHA_SUPERF  10
#define SEEK_T_MEDIO 4
#define SEEK_T_MINIMO 1
#define T_MEDIO_LAT 6
#define TEMPO_TRANSF 12
#define TRUE 1
#define FALSE 0


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

/* QUE PORRA E ESSA KHALIL! */
typedef struct fatent_s{
	unsigned int used;
	unsigned int eof;
	unsigned int next;
}fatent;


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

int sizeOfFile(FILE *fp){ /* Retorna o tamanho do arquivo em bytes */
	/* Observação: o 'size' sempre
	   tera alguns bytes a mais devido ao
	   '\n' ao final do arquivo e das linhas.*/
	int size;

	fseek(fp, 0, SEEK_END); /* Leva o ponteiro para o final do arquivo */
	size = ftell(fp); /* Retorna a posição do ponteiro dentro do arquivo */
	return size;
}

void escreverArquivo(track_array *cylinder, char fileName[], int , FILE *fp){
	int fp_size, cluster_needed;


	fp = fopen(fileName, "r");
	fp_size = sizeOfFile(fp);
	printf("Tamanho do Arquivo digitado: %d\n bytes", fp_size);
	cluster_needed = ceil(fp_size / (CLUSTER * 512));



}


int main(){

	int opc;
	char fileName[100];
	const int CLUSTER = 4;
	FILE *fp;
	track_array *cylinder = (track_array*)malloc(10 * sizeof(track_array));


	opc = menu();

	if(opc == 1){
		printf("Por favor, escreva o nome do arquivo: ");
		scanf(" %s", fileName);

	}else if(opc == 2){

	}else if(opc == 3){

	}else if(opc == 4){

	}

	return 0;
}
