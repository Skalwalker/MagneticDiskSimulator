/*
	Titulo:
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
	
	int opc;
	track_array *cylinder;
	
	opc = menu();
	
	if(opc == 1){
		
	}else if(opc == 2){
		
	}else if(opc == 3){
	
	}else if(opc == 4){
		
	}
	
	return 0;
}
