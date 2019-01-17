#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>    

#include <sys/time.h>
#include <termios.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define BAUDRATE B115200


int fd;
static struct termios oldtio, newtio;


void serial_write(char * msg){
	write(fd, &msg, 5);
	 usleep ((strlen(msg) + 25) * 100);
}



int main(){
	

	char byte;
    fd = open("/dev/ttyUSB3", O_RDWR);

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

    /*
     * terminal settings done, return file descriptor
     **/
    if(fd < 0){
		printf("O mote coordenador não está acessível via serial\n");
		printf("Verifique se o mote está conectado e/ou se há permissão de acesso\n");
	}else{
		printf("O mote coordenador está conectado via serial\n");
	}	
    int numero;
	scanf("%d", &numero);

	//char msg[2000] = "Nelson";
	//printf("Tamanho: %d\n", strlen(buffer));
	//write(fd, &msg, 10);

	serial_write("Nelson\n");
	int i;
	// for(i=0; i < sizeof(msg); i++){
	// 	printf("Vai mandar: %c", msg[i]);
	// 	serial_write(msg[i]);
	// 	 usleep( 1000000 );
	// }
	//serial_write('\n');

	close(fd);
return 1;

}

