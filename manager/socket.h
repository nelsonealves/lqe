
#ifndef  SOCKET_H
#define  SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "serial.h"
/* Headers para socket */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define port 9000
#define LEN 4096

struct sockaddr_in local;
struct sockaddr_in remoto;

int sockfd;
int cliente;
extern int length;
int slen;
char buffer[4096];

void socket_init();
void socket_communication();
void socket_closed();

#endif 

/**
 * Principais funções para escrever programas com sockets
 

getaddrinfo()  // Traduz nomes para endereços sockets
socket()       // Cria um socket e retorna o descritor de arquivo
bind()         // Associa o socket a um endereço socket e uma porta
connect()      // Tenta estabelecer uma conexão com um socket
listen()       // Coloca o socket para aguardar conexões
accept()       // Aceita uma nova conexão e cria um socket
send()         // caso conectado, transmite mensagens ao socket
recv()         // recebe as mensagens através do socket 
close()        // desaloca o descritor de arquivo
shutdown()     // desabilita a comunicação do socket

*/
