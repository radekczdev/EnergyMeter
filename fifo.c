#include "fifo.h"

unsigned long volatile PutTX;		//licznik danych odlozonych do FIFO
unsigned long volatile GetTX;		//licznik danych pobranych FIFO

//inicjalizacja bufora FIFO
void FIFOTX_Init(void){
	PutTX=GetTX=0;		//bufor jest pusty
}

//dodanie elementu do FIFO
unsigned short FIFOTXPut(DATATXtype data){
	if((PutTX-GetTX) & ~(FIFOsizeTX-1)){			//jezeli warunek prawdziwy tzn. ze FIFO jest pelne
		return(FIFOTXfail);				//FIFO jest pelne
}
	FIFOTX[PutTX&(FIFOsizeTX-1)]=data;		//dodajemy dana do FIFO
	PutTX++;		//zwiekszamy rozmiar bufora FIFO
	return(FIFOTXsucces);		
}

//pobieranie elementu z FIFO
unsigned short FIFOTXGet(DATATXtype *dataget){
	if(PutTX==GetTX){					//jezeli warunek prawdziwy tzn. ze w FIFO nie ma nowych danych do pobrania
	return(FIFOTXfail);
	}
	*dataget = FIFOTX[GetTX&(FIFOsizeTX-1)];
	GetTX++;
	return(FIFOTXsucces);
}

//ILOSC elementow w FIFO
unsigned short FIFOsizeAc(void){
	return((unsigned short)(PutTX-GetTX));
}














