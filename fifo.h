#define FIFOsizeTX 2048			//rozmiar FIFO
#define FIFOTXsucces 1
#define FIFOTXfail 0

typedef unsigned short DATATXtype;		//typ przechowywanych danych w buforze
DATATXtype static FIFOTX[FIFOsizeTX];		//miejsce na dane

void FIFOTX_Init(void);		//inicjalizacja FIFO
unsigned short FIFOTXPut(DATATXtype data);		//dodanie danych do FIFO
unsigned short FIFOTXGet(DATATXtype *dataget);		//pobranie elementow z FIFO
unsigned short FIFOsizeAc(void);			//wolne miejsce w FIFO












