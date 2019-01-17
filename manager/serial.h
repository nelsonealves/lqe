
#ifndef  SERIAL_H
#define  SERIAL_H

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>    

#include <sys/time.h>
#include <termios.h>
#include <string.h>
#include <time.h>

#include <time.h>
#include <stdlib.h>

#define BAUDRATE B115200

static struct termios oldtio, newtio;
int fd;
int abreComunicacaoSerial();
void serial_write(char *msg);

#endif 