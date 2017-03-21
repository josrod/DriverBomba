/***************************************************************************
                          ComunicacionPuertoSerie.cpp  -  description
                             -------------------
    begin                : Thu Mar 17 2005
    copyright            : (C) 2005 by Leonardo Santamarina Gzyl
    email                : 
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <stdio.h>
#include <time.h>
//#include "driverEvita.h"
#include "ComunicacionPuertoSerie.h"

//---------------------------------------------------------------------------
//CONSTRUCTORES
//---------------------------------------------------------------------------

/**
 * ComunicacionPuertoSerie(void)
 * @brief Constructor por defecto de la clase.
 * @author Susana Martín Toral
 * @date 28/01/05
 * @version 1.0
 * @note
 * @warning
 */
  ComunicacionPuertoSerie::ComunicacionPuertoSerie(void)
  {
     iFd=0;
  vLectura[0]='\0';
  pcEscritura=NULL;
  pTerminal=NULL;
  //cambiado por susana
  //pcModemdevice=NULL;
  //fin cambiado por susana
  bufsize=BUFSIZE;
  }

/**
 * ComunicacionPuertoSerie(iDes,pcSalida,pTerm,pcPuerto)
 * @brief Constructor general de la clase.
 *        E:/ datos_terminal pTerm: Modos de control de la comunicación
 *            char pcPuerto: Puerto de comunicación serie (/dev/ttyS*)
 *        S:/ objeto de la clase ComunicacionPuertoSerie
 * @author Susana Martín Toral
 * @date 28/01/05
 * @version 1.0
 * @note
 * @warning
 */

  ComunicacionPuertoSerie::ComunicacionPuertoSerie(datos_terminal pT,QString sPuerto, int bs, int fl)
  {

  printf("\nComunicacionPuertoSerie: Dentro del constructor\n");
  //iFd=iDes;
  vLectura[0]='\0';
  //pcEscritura=new char[strlen(pcSalida)];

  pTerminal=new datos_terminal;
  //*pTerminal = pTerm;                  //copia una estructura en otra
                                       //campo a campo
  pTerminal->uiVelocidad=pT.uiVelocidad;
  pTerminal->iParidad=pT.iParidad;
  pTerminal->iBit_datos=pT.iBit_datos;
  pTerminal->iBit_start=pT.iBit_start;
  pTerminal->iBit_stop=pT.iBit_stop;
  pTerminal->iTmuestreo=pT.iTmuestreo;
  pTerminal->iTimeOutC=pT.iTimeOutC;
  pTerminal->iTimeOutM=pT.iTimeOutM;

  //cambiado por susana
  //pcModemdevice=new char[strlen(pcPuerto)+1];
  //for(int i=0; i<12; i++) pcModemdevice[i]='\0';
  //strcpy(pcModemdevice,pcPuerto);
  //pcModemdevice[strlen(pcPuerto)]='\0';
  sModemdevice=sPuerto;
  //printf("ComunicacionPuertoSerie: El puerto a abrir es pcPuerto %s \n",pcPuerto);
  //printf("ComunicacionPuertoSerie: El puerto a abrir es pcModemdevice%s \n",pcModemdevice);
  //cambiado por susana
  
  for(int i=0; i<64; i++) vTira[i]='\0';
  bufsize=bs;
  pcEscritura=NULL;
  flags=fl;
  printf("ComunicacionPuertoSerie: Final del constructor\n");

  }

  //---------------------------------------------------------------------------
//SetThings
//---------------------------------------------------------------------------

void ComunicacionPuertoSerie::SetFd(int iDes)
{
  iFd=iDes;
}
void ComunicacionPuertoSerie::SetLectura(char *pcEntrada)
{
  strcpy(vTira,pcEntrada);
  strcat(vTira,"\0");
}
void ComunicacionPuertoSerie::SetEscritura(char *pcSalida)
{
  if(pcEscritura != NULL){
    delete [] pcEscritura;
    //printf("\nborro pcEscritura de %s\n",nombre);
    pcEscritura=NULL;
  }
  pcEscritura=new char[strlen(pcSalida)];
  strcpy(pcEscritura,pcSalida);
}
void ComunicacionPuertoSerie::SetTerminal(datos_terminal pTerm)
{
  *pTerminal = pTerm;                 //copia una estructura en otra
                                      //campo a campo
}
void ComunicacionPuertoSerie::SetModemdevice(QString sPuerto)
{
  //strcpy(pcModemdevice,pcPuerto);
  sModemdevice=sPuerto;
}


//---------------------------------------------------------------------------
//GetThings
//---------------------------------------------------------------------------

int ComunicacionPuertoSerie::GetFd(void){return iFd;}
char *ComunicacionPuertoSerie::GetLectura(void){return vTira;}
char *ComunicacionPuertoSerie::GetEscritura(void){return pcEscritura;}
char *ComunicacionPuertoSerie::GetTira(void){return vTira;}
datos_terminal *ComunicacionPuertoSerie::GetTerminal(void){return pTerminal;}
QString ComunicacionPuertoSerie::GetModemdevice(void){return sModemdevice;}



//---------------------------------------------------------------------------
//FUNCIONES MIEMBRO
//---------------------------------------------------------------------------

/**
 * configurarPuerto():
 * @brief Función que configura y abre, de la manera adecuada según el driver,
 *        el puerto de comunicación RS_232_C. En el caso de la bomba se emplea
 *        una transmisión serie asíncrona, en modo no canónico, entre un PC y
 *        una bomba modelo XL.
 *        S:/ int  : entero que indica el estado al finalizar la función
 *                   (-1 -> estado de error, o -> estado sin error)
 * @author Susana Martín Toral
 * @date 02/02/05
 * @version 1.0
 * @note Notas que puedan ser de interés (puede estar en blanco)
 * @warning Avisos importantes (puede estar en blanco)
 */

int ComunicacionPuertoSerie::configurarPuerto(void)
{
	//printf("\nComunicacionPuertoSerie: dentro de funcion configurarPuerto\n");
	cout<<"ComunicacionPuertoSerie: ANTES Abrir el puerto "<<sModemdevice<<endl;
    
	//apertura del puerto para ser modo NO CANÓNICO
	if((iFd=open(sModemdevice.ascii(),flags))<0){             //O_RDWR | O_NOCTTY
		perror("Error en la apretura del puerto");
		return(UCI_ERROR);
	}  
	//iFd=open(pcModemdevice,O_RDWR | O_NOCTTY | O_NONBLOCK);
    
	tcgetattr(iFd,&viejotio);            //guarda los valores de configuración
                                       //del puerto actuales
       
	tcgetattr(iFd,&nuevotio);            //inicializa la nueva configuración
	//introducir la nueva configuracion:

	//--------------------------------------------------------------------------
	//Modos en lectura. Variable c_iflag
	nuevotio.c_iflag = 0;
	nuevotio.c_iflag |= INPCK;          //chequeo de la paridad de entrada

	//--------------------------------------------------------------------------
	//Modos en escritura. Variable c_oflag
	nuevotio.c_oflag = 0;               //no se habilita ningún flag
  
	//--------------------------------------------------------------------------
	//Modos de control. Variable c_cflag
	nuevotio.c_cflag = 0;
	nuevotio.c_cflag |= CLOCAL;         //ignora las lineas de control del modem
	nuevotio.c_cflag |= CREAD;          //habilita la recepción
	switch (pTerminal->uiVelocidad)     //velocidad en Baudios
	{
		case 150: nuevotio.c_cflag |= B150; break;
		case 200: nuevotio.c_cflag |= B200; break;
		case 300: nuevotio.c_cflag |= B300; break;
		case 600: nuevotio.c_cflag |= B600; break;
		case 1200: nuevotio.c_cflag |= B1200; break;    //(*)
		case 1800: nuevotio.c_cflag |= B1800; break;
		case 2400: nuevotio.c_cflag |= B2400; break;    
		case 4800: nuevotio.c_cflag |= B4800; break;
		case 9600: nuevotio.c_cflag |= B9600; break;
		case 19200: nuevotio.c_cflag |= B19200; break;
		case 38400: nuevotio.c_cflag |= B38400; break;
		default: printf("\aVelocidad imposible\n");
        		return(-1);
	}
	switch (pTerminal->iParidad)        //paridad para análisis de error
	{
		case 0: nuevotio.c_cflag &= ~PARENB; break;     //(*) no paridad
		case 1: nuevotio.c_cflag |= PARENB;             //paridad impar
        		nuevotio.c_cflag |= PARODD; break;
		case 2: nuevotio.c_cflag |= PARENB;           
			nuevotio.c_cflag &= ~PARODD; break;
		default: printf("\aParidad imposible\n");
			return(-1);
	}
	switch (pTerminal->iBit_datos)                      //nº de bits para datos
	{
		case 5: nuevotio.c_cflag |= CS5; break;
		case 6: nuevotio.c_cflag |= CS6; break;
		case 7: nuevotio.c_cflag |= CS7; break;         
		case 8: nuevotio.c_cflag |= CS8; break;         //(*)
		default: printf("\aNúmero de bits de datos imposible\n");
		return(-1);
	}
	switch (pTerminal->iBit_stop)                     //número de bits de stop
	{
		case 1: nuevotio.c_cflag &= ~CSTOPB; break;     //(*) 1 bit de stop 
		case 2: nuevotio.c_cflag |= CSTOPB; break;      //2 bits de stop
		default: printf("\aNúmero de bits de parada imposible\n");
		return(-1);
	}
  
	//--------------------------------------------------------------------------
	//Modos de la disciplina de linea. Variable c_lflag
	nuevotio.c_lflag = 0;               //no se habilita ningún flag
  
	//--------------------------------------------------------------------------
	//Caracteres de control
	nuevotio.c_cc[VMIN]=0;
	nuevotio.c_cc[VTIME]=pTerminal->iTimeOutC;   //la lectura se satisface si se recibe
                                                     //un caracter o si se excede el VTIME
	//--------------------------------------------------------------------------

	tcflush(iFd,TCIFLUSH);
	tcflush(iFd,TCOFLUSH);              //vacía los datos escritos pero no
                                            //transmitidos
	tcsetattr(iFd,TCSANOW,&nuevotio);   //envía los nuevos valores de la
                                            //estructura termios al tty driver
	return(UCI_OK);                     //devuelve 0 indicando éxito del metodo
}


//---------------------------------------------------------------------------

/**
 * leerPuerto()
 * @brief Funcion que implementa un bucle que lee del buffer de entrada del
 *        puerto serie los datos recibidos en la transmisión. El funcionamiento
 *        de la lectura se rige por el modo NO CANÓNICO, por lo que para
 *        satisfacer una llamada a la primitiva read se debe recibir un
 *        caracter (MIN=0) ó se debe exceder el tiempo de 40 ms sin recibir
 *        caracteres (TIME=0.4).
 *        S:/ int  : entero que indica el estado al finalizar la función
 *                   (-1 -> estado de error, o -> estado sin error)
 * @author Susana Martín Toral
 * @date 03/02/05
 * @version 1.0
 * @note Notas que puedan ser de interés (puede estar en blanco)
 * @warning Avisos importantes (puede estar en blanco)
 */

int ComunicacionPuertoSerie::leerPuerto(void)
{
  int i=0;
  int total=0;
  int iNumCaracter=0;              //almacena el nº de caracteres leidos
  int iEstado=UCI_OK;
  //char temp='0'; 
 
   
  sleep(1);                        //con 1 segundo debería funcionar
  	
  //do{                                 //según el protocolo de comunicación
                                      //Invertomatic se leen caracteres hasta
                                      //que se detecte un ETX, NACK o no se
                                      //reciba nada en 2 seg
      //iNumCaracter+=read(iFd,vLectura,BUFSIZE);
      iNumCaracter+=read(iFd,vTira,BUFSIZE);
      
      if(iNumCaracter<= 0){
         iEstado=UCI_ERROR;
	            //No se han recibido datos en 40ms
		    tcflush(iFd,TCIFLUSH); 
		    tcflush(iFd,TCIFLUSH); 
	 return(iEstado);
      }
      //else
         //vTira[i++]=vLectura[0];      //los caracteres recibidos se copian en
                                     //la tira
  //}
  //while (vLectura[0] != 13 && iNumCaracter > 0);
  //vTira[i]='\0';              //añade un \0 para detectar el fin de
  vTira[iNumCaracter+1]='\0';              //añade un \0 para detectar el fin de
                                      //la tira
  tcflush(iFd,TCIFLUSH);              //vacía datos recibidos pero no leidos
  tcflush(iFd,TCIFLUSH);

 // printf(" llega la trama  %s\n",vTira);
  return(iEstado);
 
}

//---------------------------------------------------------------------------

/**
 * escribirPuerto()
 * @brief Funcion que se encarga de escribir los datos a transmitir a través
 *        del puerto serie en su buffer de salida y realizar dicha transmisión.
 *        S:/ int  : entero que indica el estado al finalizar la función
 *                   (-1 -> estado de error, 0 -> estado sin error)
 * @author Susana Martín Toral
 * @date 03/02/05
 * @version 1.0
 * @note Notas que puedan ser de interés (puede estar en blanco)
 * @warning El 'usleep' luego de leer es un paso delicado.
 *          se debe ajustar dependiendo de la velocidad de transmision.
            (ejemplo: como mínimo a 500 ms a 19200 baudios para los ventiladores Evita)
 *          De lo contrario escrituras susecivas se solapan
 *          causando transmisiones incorrectas
 *
 */

int ComunicacionPuertoSerie::escribirPuerto (void)
{
  int iNumCaracter=0;                 //almacena el nº de caracteres escritos
  int iEstado=UCI_OK;
  int iTamanoEscritura=strlen(pcEscritura);

  //Añadido por susana. Si no funciona bien la comunicacion quitarlo
  tcflush(iFd,TCOFLUSH);  
  tcflush(iFd,TCIFLUSH);  
  //fin añadido por susana
    
  iNumCaracter=write(iFd,pcEscritura,iTamanoEscritura);

  if (iNumCaracter<= 0 )
    iEstado=UCI_ERROR;

  usleep(900000);                       //sleep para que todos los datos escritos sean
                                       //transmitidos 
				       //¡¡DELICADO!!
                                       //De lo contrario escrituras susecivas se solapan 
				       //causando transmisiones incorrectas 
	
				        
  tcflush(iFd,TCOFLUSH);               //vacía los datos escritos pero no
                                      //transmitidos
  tcflush(iFd,TCOFLUSH);
  return(iEstado);
}


//---------------------------------------------------------------------------

/**
 * cerrarPuerto()
 * @brief Funcion que se encarga de reconfigurar el puerto serie a sus valores
 *        iniciales una vez que haya finalizado la transmisión. Después cerrará
 *        el puerto.
 *        S:/ int  : entero que indica el estado al finalizar la función
 *                   (-1 -> estado de error, o -> estado sin error)
 * @author Susana Martín Toral
 * @date 03/02/05
 * @version 1.0
 * @note Notas que puedan ser de interés (puede estar en blanco)
 * @warning Avisos importantes (puede estar en blanco)
 */

int ComunicacionPuertoSerie::cerrarPuerto(void)
{
  int iEstado=UCI_OK;
  if(tcsetattr(iFd,TCSANOW,&viejotio)==-1)    //vuelve a cargar viejos
                                              //valores de configuración
                                              //del puerto
     iEstado=UCI_ERROR;

  tcflush(iFd,TCOFLUSH);
  close(iFd);
  return(iEstado);                            //devuelve 0 si todo termina
                                              //correctamente
}



//---------------------------------------------------------------------------
//Destructor de la clase
//---------------------------------------------------------------------------

ComunicacionPuertoSerie::~ComunicacionPuertoSerie()
{
 /*printf("\nNombre:%s\n",nombre);
 printf("\npcEscritura: %s\n",pcEscritura);
 printf("\npcModemdevice: %s\n",pcModemdevice);*/

/*    if(pcEscritura!=NULL){
    printf("\n%s\n",pcEscritura);
    delete [] pcEscritura;
    }
  //if(pTerminal)
    //delete pTerminal;
  if(pcModemdevice!=NULL){
    printf("\n%s\n",pcModemdevice);
    delete [] pcModemdevice;
    }                       */

}
