#include "socket.h"

void main(void) {
	socket_init();
	printf("Vai comunicar");
	socket_communication();
}	


// char * msg;
//    if(abreComunicacaoSerial() < 0){
// 		printf("O mote coordenador não está acessível via serial\n");
		
// 		printf("Verifique se o mote está conectado e/ou se há permissão de acesso\n");
		
	
//    }else{
// 		printf("O mote coordenador está conectado via serial\n");
		
//    }
//    while(1){
// 	   printf("Digite um numero\n");
// 	   scanf("%s", msg); 
// 	   escreveSerial(msg);
// 	   printf("letra digitada: %s\n", msg);
// 	}
