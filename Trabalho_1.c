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


typedef struct block{
	unsigned char bytes_s[512];
}block;

typedef struct sector_array{
	block sector[60];
}sector_array;

typedef struct track_array{
	sector_array track[5];
}track_array;

typedef struct fatlist_s{
	char file_name[100];
	unsigned int first_sector;
}fatlist;

typedef struct fatent_s{
	unsigned int used;
	unsigned int eof;
	unsigned int next;
}fatent;

int menu(){

	const int TRILHA_CILINDRO  = 5; // 5 trilhas/cilindro
	const int SETORES_TRILHA = 60; // 60 setores/trilha
	const int TRILHA_SUPERF = 10; // 10 trilhas/superficie
	const int TAM_SETOR = 512; // 512 bytes/cluster
	const int TAM_CLUSTER = 4; // 4 setores/cluster
	const int SEEK_T_MEDIO = 4;
	const int SEEK_T_MINIMO = 1;
	const int T_MEDIO_LAT = 6;
	const int TEMPO_TRANSF = 12;

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

int sizeOfFile(FILE *fp){ // Retorna o tamanho do arquivo em bytes
	/* Observação: o 'size' sempre
	   tera alguns bytes a mais devido ao
	   '\n' ao final do arquivo e das linhas.*/
	int size;

	fseek(fp, 0, SEEK_END); // Leva o ponteiro para o final do arquivo
	size = ftell(fp); // Retorna a posição do ponteiro dentro do arquivo
	return size;
}

int main(){

	int opc;
	int fpSize;
	char fileName[100];
	FILE *fp;
	track_array *cylinder;

	opc = menu();

	if(opc == 1){
		printf("Por favor, escreva o nome do arquivo: ");
		scanf(" %s", fileName);
		fp = fopen(fileName, "r");
		fpSize = sizeOfFile(fp);
		printf("Tamanho do Arquivo digitado: %d\n bytes", fpSize);

	}else if(opc == 2){

	}else if(opc == 3){

	}else if(opc == 4){

	}

	return 0;
}
