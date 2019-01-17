#ifndef MOTE
#define MOTE

#define NUM_MAX_MOTES 256 
#define MAX_SIZE_XML_PARAM 32
#define MAX_SIZE_XML_VALOR 10

static struct unicast_conn uc;
static struct broadcast_conn broadcast;

int lambda=2;
int tamanhoPacote=5;
uint8_t destino=100;
uint8_t distribuicao=0;
//int tempoMedicao=10;
uint8_t sink=1; //1 se for sink, 0 se for source
uint8_t start=0; //começa parado...logo start=0
uint8_t configuraBool=0;
uint8_t running=0;//running = 1...Uma das threads (sink or source) está on.

int Sink_totalPacotesRecebidos=0;
int Sink_pacoteID[NUM_MAX_MOTES]={0};//Numero de pacotes por mote.
int Source_totalPacotesEnviados=0;
int Source_pacoteID[NUM_MAX_MOTES]={0};//Numero de pacotes por mote.


#endif
