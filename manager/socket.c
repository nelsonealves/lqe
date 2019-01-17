#include "socket.h"

int length = sizeof(remoto);

int numero;
void socket_init(){
	printf("Cliente init\n");
	
	
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	printf("Sockfd retornou %d\n", sockfd);
	if(sockfd == -1){
		printf("Não pôde criar!\n");
		exit(1);
	}
	printf("Socket criado\n");

	remoto.sin_family = AF_INET;
	remoto.sin_port = htons(port);
	remoto.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(local.sin_zero, 0x0, 8);
	
	printf("ops\n");
	if ((cliente = connect(sockfd, (struct sockaddr*)&remoto, length)) == 0){
		printf("Conectado com sucesso!\n");
	}else{
		perror("Falha na conexão!\n");
		exit(1);
	}
	//printf("cliente retornou %d", cliente);
	//strcpy(buffer, "Olá servidor, eu to vivo!");
	//send(cliente, buffer, strlen(buffer), 0);
	printf("Saindo da função\n");
}


	// strcpy(buffer, "Fala fala servidor! \n");

	// if(send(cliente, buffer, strlen(buffer), 0)){

	//	}
void socket_communication(){
	if(abreComunicacaoSerial() < 0){
		printf("O mote coordenador não está acessível via serial\n");
		printf("Verifique se o mote está conectado e/ou se há permissão de acesso\n");
	}else{
		printf("O mote coordenador está conectado via serial\n");
	}	
	printf("Escutando:\n");
	 for(;;){
		 if((slen = recv(sockfd, buffer, LEN, 0))>0){
		 	buffer[slen] = '\n';
		 	printf("Mensagem recebida: %s \n", buffer);
		 	serial_write(buffer);
		 }
	}
	socket_closed();
}

void socket_closed(){
	close(sockfd);
	printf("Servidor encerrado");
}
