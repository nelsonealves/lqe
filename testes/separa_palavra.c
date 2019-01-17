#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int pos, pos1, cont;
char data_receive[256] = "Teste=ALL;";
char data1[30];
char value1[30];

void main(){
	int tamanho =(int)strlen(data_receive);
	for(pos=0,pos1=0, cont=0; pos < tamanho; pos++){
		if(data_receive[pos]==';'){
			cont=0;
			pos1=0;
			continue;
		}
		if(data_receive[pos]=='='){
			cont=1;
			pos1=0;
			continue;
		}
		if(cont==0){
			data1[pos1]=data_receive[pos];	
			pos1++;
		}
		if(cont==1){
			value1[pos1]=data_receive[pos];
			pos1++;
		}	
			
		
		

	}
	int i;
	printf("data1:");
	for(i=0; i< strlen(data1); i++){
		printf("%c", data1[i]);
	}
	printf("\nvalue1:");
	for(i=0; i < strlen(value1); i++){
		printf("%c", value1[i]);
	}

}
