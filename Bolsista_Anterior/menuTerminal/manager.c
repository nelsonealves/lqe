#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>    
#include "manager.h"
#include <sys/time.h>
#include <termios.h>
#include <string.h>
#include <time.h>

#include <time.h>
#include <stdlib.h>
//#include <libxml/parser.h>

static struct termios oldtio, newtio;

int 
abreLog(){
	//Arquivo para armazenar dados do experimento
	log_fd = fopen(ARQUIVO_LOG, "a+");
	
	if(log_fd == NULL){
	   return ERRO_OPEN_FILE;
	}
	return 0;
}

int
printLog(char * msg){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	fprintf(log_fd, "%d-%d-%d %d:%d:%d || %s", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, msg);
	fflush(log_fd);
}

int 
fechaLog(){
	fclose(log_fd);
}

int 
abreComunicacaoSerial(){
    char byte;
    fd = open("/dev/ttyUSB1", O_RDWR);

    tcgetattr (fd, &oldtio);    /* save current serial port settings */
    bzero (&newtio, sizeof (newtio));   /* clear struct for new port settings */

    /* 
     *BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
     *CRTSCTS : output hardware flow control (only used if the cable has
     *all necessary lines. )
     *CS8     : 8n1 (8bit,no parity,1 stopbit)
     *CLOCAL  : local connection, no modem contol
     *CREAD   : enable receiving characters
     **/
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;

    /*
     *IGNPAR  : ignore bytes with parity errors
     *ICRNL   : map CR to NL (otherwise a CR input on the other computer
     *          will not terminate input)
     *          otherwise make device raw (no other input processing)
     **/
    newtio.c_iflag = IGNPAR | ICRNL;

    /* 
     * now clean the modem line and activate the settings for the port
     **/
    tcflush (fd, TCIFLUSH);
    tcsetattr (fd, TCSANOW, &newtio);

    /*
     * terminal settings done, return file descriptor
     **/

    return fd;   
}

void 
escreveSerial(char *msg){
	write(fd, msg, strlen(msg));
	usleep ((strlen(msg) + 25) * 100);
}

int 
leSerialUntil(char *byte, char parada){
	ssize_t size ;
	char byte_lido;
	int i=0;
	
	memset(byte, 0, sizeof byte);
	do{
		size = read(fd, &byte_lido, 1);

		if(size>0)byte[i++] = byte_lido;
		else{
			if(size==-1)		
				perror("read() error");
	 		break;
		}
	}while(byte_lido!='\n');

	if(byte[0]=='\n'){
		 return -1;
	}

	//Tirando o '\n' do fim
	byte[i-1]='\0';



	return size;//i-2;
}

void 
leSerial(char *byte){
	ssize_t size = read(fd, byte, MAX_BITS_RECEPCAO);
	byte[size-2]='\0';
}

int 
esperaSelect(int segundos){
	//Utiliza select para atender uma recepçao via serial ou ao timeout 
	fd_set set;
	struct timeval timeout;

	/* Initialize the file descriptor set. */
	FD_ZERO (&set);
	FD_SET (fd, &set);

	//Set_timeout
	/* Initialize the timeout data structure. */
	timeout.tv_sec = segundos;
	timeout.tv_usec = 0;

	/* select returns 0 if timeout, 1 if input available, -1 if error. */
	return select (FD_SETSIZE, &set, NULL, NULL, &timeout);
}

int 
esperaOK(){
	int ret_select=0;
	char byte[MAX_BITS_RECEPCAO];
	//Ativa o select para esperar uma resposta do coordenador OU o estouro do timer
	ret_select = esperaSelect(1);
	if(ret_select<1){
		//Não recebeu resposta do coordenador
		return ret_select;
	}else{
		//Recebeu a resposta do coordenador
		leSerial(byte);
		
		//Pode ser que a serial atrase o envio do \n de uma transmissão anterior
		//E daí dá pau no strcmp abaixo...Por isso tem essa correção de BUG!
		if(byte[0]=='\n'){
			memmove (byte, byte+1, strlen (byte+1) + 1); 
		}	

		if(strcmp("ok", byte)==0){ 
			return COORDENADOR_OK;
		}else{
			return COORDENADOR_NOK;
		}
	}
}

int 
fsm_manager_status(int var){

	int ret_select=0;
	char byte[MAX_BITS_RECEPCAO];
	char cmd[MAX_BITS_RECEPCAO]={0};

	switch(estado){
		case Esperando_configuracao:;
			int ok;
			//printf("Esperando_configuracao\n");	
				  
			//!hello_coordinator	
			escreveSerial("hello_coordinator\n");
			if((ok=esperaOK())!=COORDENADOR_OK){
				estado = Esperando_configuracao;
				return ok;
			}
			estado = aguardando_status;	
			return COORDENADOR_OK;


		case aguardando_status:
		//printf("aguardando_status\n");	

			//!LêStatus_Motes	
			escreveSerial("leStatus_motes\n");		

			//Ativa o select para esperar uma resposta do coordenador OU o estouro do timer
			ret_select = esperaSelect(20);
			if(ret_select<1){
				//Não recebeu resposta do coordenador
				return ret_select;
			}else{
				sleep(4);
				//Recebeu a resposta do coordenador

				int ret=0;

				while(1){
					memset(cmd, 0, sizeof cmd);
					if((ret=leSerialUntil(byte, '\n'))>0){
						strncpy(cmd, byte, 11);
						if(strcmp("status_mote", cmd)==0){ 
							//Arquivo para armazenar dados do experimento
							FILE *fp = fopen(ARQUIVO_EXPERIMENTO, "a");
							if(fp == NULL){
							   estado = Esperando_configuracao;							 
							   return ERRO_OPEN_FILE;
							}
							fprintf(fp, "%s \n", byte);
							//Recebe informações de status dos motes até receber a mensagem fim ou estourar o timeout
							fclose(fp);

							memcpy (mostra, byte, sizeof byte);
							memset(cmd, 0, sizeof cmd);
						}
						else{
							if(strcmp("fim", byte)==0){ 
								estado = mostrando_status;		
								fsm_manager_status(1);		
								return FIM;
							}		
						}

					}


				}
			}
				

		break;

		case mostrando_status:;
			//printf("mostrando_status\n");	
			printf("%s\n", mostra);
			char log[MAX_BITS_RECEPCAO]={0};
			sprintf(log,"%s\n", mostra);
			printLog(log);

			estado = Esperando_configuracao;
			memset(mostra, 0, sizeof mostra);
		break;

	}
	return 100;
}

void 
enviaConf(char *param, int ID){
   	char ret[6]={0}; //6 é o tamanho maximo da string que deve retornar do test.xml
	char envia[MAX_BITS_RECEPCAO]={0};
	int ok=0;

	leParametro(ID, param, ret);
	//TESTAR SE LEU ALGO !!! USUARIO É LOUCO
	strcpy(envia, param);
	/*
		Uso o sprintf por ser mais pratico concatenar com o integer
		apesar do strcat ser mais rapido e indicado...
		Mas nao uso atoi! :D
	*/
	sprintf(envia,"%s/%d/%s\n",param,ID,ret); //param do mote com id ID é ret! EX: -> lambda do id 2 é 5 = lambda/2/5
	escreveSerial(envia);
}

int
configura(int ID, char* param){
	int  ok=0;
	int  ret_select=0;
	char byte[MAX_BITS_RECEPCAO];
	char log[MAX_BITS_RECEPCAO]={0};

	escreveSerial("configura\n");	
	if((ok=esperaOK())!=COORDENADOR_OK){
			return ok;
			printLog("COORDENADOR_NOK! \n");
	}
	printLog("COORDENADOR_OK! \n");

	enviaConf(param, ID);
	//Ativa o select para esperar uma resposta do coordenador OU o estouro do timer
	ret_select = esperaSelect(10);
	if(ret_select<1){
		//Não recebeu resposta do coordenador
		switch(ret_select)
		{				
			case TIMEOUT:
				printf("Timeout! \n");
				printLog("Timeout! \n");
				break;
			case ERRO_SELECT:
				printf("ERRO: Select retornou -1! \n");
				printLog("ERRO: Select retornou -1! \n");
				break;
		}
		return ret_select;
	}else{
		sleep(1);
		if((ret_select=leSerialUntil(byte, '\n'))>0){

			if(strlen(byte)==13){
				sprintf(log,"Configura: %s - Nenhum mote foi configurado \n", param);
				printLog(log);
				return -1;	
			}
				sprintf(log,"Configura: %s - %s \n", param, byte);
				printLog(log);
		}
	}
	sleep(2);//Dá um tempo pro mote
}


int
iniciaTermina(int op){
	int ok=0;
	int  ret_select=0;
	char byte[MAX_BITS_RECEPCAO];
	char log[MAX_BITS_RECEPCAO]={0};

	if(op==0){
		escreveSerial("start\n");	
	}
	else{
		escreveSerial("stop\n");	
	}

	//Ativa o select para esperar uma resposta do coordenador OU o estouro do timer
	ret_select = esperaSelect(10);
	if(ret_select<1){
		//Não recebeu resposta do coordenador
		switch(ret_select)
		{				
			case TIMEOUT:
				printf("Timeout! \n");
				printLog("Timeout! \n");
				break;
			case ERRO_SELECT:
				printf("ERRO: Select retornou -1! \n");
				printLog("ERRO: Select retornou -1! \n");
				break;
		}
		return ret_select;

	}else{
		sleep(3);
		leSerial(byte);
		printf("%s \n", byte);
		sprintf(log,"%s \n", byte);
		printLog(log);
	}	

}


int menu(){

   int menu_option=0;
   int retorno = 0;
   int pedeOption = 1;	
   int id=0;

   do{
	   if(pedeOption){
		   printf("\n _________________________________________________________\n");
		   printf("Aperte 1 para ler status \n");
		   printf("Aperte 2 para configuração de motes \n");
		   printf("Aperte 3 para iniciar uma medição \n");
		   printf("Aperte 4 para terminar uma medição \n");
		   scanf("%d", &menu_option);	
		   pedeOption=0;
	   }
	   switch(menu_option)
	   {
	    case 1:
		printLog("Ler status:\n");
		retorno = fsm_manager_status(1);
		switch(retorno)
		{				
			case COORDENADOR_OK:
				printLog("COORDENADOR_OK! \n");
				break;
			case FIM:
				printLog("FIM! \n");
				pedeOption=1;
				break;
			case TIMEOUT:
				printf("Timeout! \n");
				printLog("Timeout! \n");
				pedeOption=1;
				break;
			case ERRO_SELECT:
				printf("ERRO: Select retornou -1! \n");
				printLog("ERRO: Select retornou -1! \n");
				pedeOption=1;
				break;
			case ERRO_OPEN_FILE:
				printf("ERRO: Não foi possível criar/abrir um arquivo plataformaDeExperimenos.txt \n");
				printLog("ERRO: Não foi possível criar/abrir um arquivo plataformaDeExperimenos.txt \n");
				pedeOption=1;
				break;
		}
	        break;

		case 2:
			printLog("Configuração de motes:\n");
			printf("Qual o ID do mote a ser programado? \n");
			scanf("%d", &id);	
		   	char ret[6]={0}; //6 é o tamanho maximo da string que deve retornar do test.xml
			leParametro(id, "modo", ret);

			configura(id, "modo");				
			if(strcmp("source", ret)==0){
				configura(id, "lambda");	
				configura(id, "tamanho_pacote");
				configura(id, "destino");	
				configura(id, "distribuicao");
			}

	
			//Lê o valor de configurar_MAC para saber se a camada MAC vai ser configurada ou nao

			leParametro(id, "configurar_MAC", ret);
			if(strcmp("1", ret)==0){
				configura(id, "CSMA_MAX_BE");
				configura(id, "CSMA_MIN_BE");
				configura(id, "macMaxCSMABackoffs");
				configura(id, "macMaxFrameRetries");
			}
			printLog("FIM! \n");
			pedeOption=1;
		break;			

		case 3:
			iniciaTermina(0);
			pedeOption=1;
		break;
		case 4:
			iniciaTermina(1);
			pedeOption=1;
		break;
		default:
			close(fd);
			menu_option = -1;
		   	printf("Invalido\n");
	   }
   }while(menu_option > 0);	
}

int 
main() {

   abreLog();

   if(abreComunicacaoSerial() < 0){
	printf("O mote coordenador não está acessível via serial\n");
	printLog("O mote coordenador não está acessível via serial\n");
	printf("Verifique se o mote está conectado e/ou se há permissão de acesso\n");
	printLog("Verifique se o mote está conectado e/ou se há permissão de acesso\n");
	return ERRO_CONEXAO_SERIAL;
   }else{
	printf("O mote coordenador está conectado via serial\n");
	printLog("O mote coordenador está conectado via serial\n");
   }
   menu();
   close(fd);
   fechaLog();

    return 0;
}
