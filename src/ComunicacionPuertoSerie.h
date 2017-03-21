/***************************************************************************
                          ComunicacionPuertoSerie.h  -  description
                             -------------------
    begin                : Tue Mar 15 2005
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

#ifndef __TXSERIE_H__
#define __TXSERIE_H__

#include <termios.h>                   //libreria necesaria para trabajar
                                       // con el puerto serie
#include <fcntl.h>                     //para la función open
#include <unistd.h>                    //libreria que define constantes
#include <sys/signal.h>                //libreria que contiene señales para
                                       //llamadas al sistema
#include <string.h>				       				       
#include <sys/types.h>
#include "Definiciones.h"              //definiciones usadas en el sistema
#include <qstring.h>
#include <iostream.h>
//#define BUFSIZE 1
#define BUFSIZE 64

 

typedef struct{
     unsigned int uiVelocidad;        //velocidad de transmision en Baudios
     int iParidad;                    //tipo de paridad (par, impar o ninguno)
     int iBit_datos;                  //numero de bits usados para un dato
     int iBit_start;                  //numero de bits de comienzo
     int iBit_stop;                   //numero de bits de parada (1 o 2)
     int iTmuestreo;                  //periodo de muestreo de la bomba
     int iTimeOutC;                   //TimeOut entre caracteres (en segundos)
     int iTimeOutM;                   //TimeOut entre mensajes (en segundos)
}datos_terminal;

class ComunicacionPuertoSerie
{
protected:

  int iFd;                             //descriptor fichero puerto serie
  
  //cambiado por susana
  //char pcModemdevice[12];            //puerto que se va a utilizar para la comunicación
  QString sModemdevice;
				       
  //fin cambiado por susana
  char vLectura[1];                    //recoge cada caracter que se recibe
  char *pcEscritura;                   //almacena la cadena a enviar
  char vTira[64];                      //total de caracteres recibidos
  struct termios viejotio,nuevotio;    //nuevotio: nueva configuracion del
                                       //puerto
                                       //viejotio: vieja configuracion
  //int flags;                           //Banderas de configuración para el descriptor de fichero
  //struct sigaction saio;               //Definición de la señal
  
  datos_terminal *pTerminal;           //modos de control de la comunicación

  int bufsize;                         // Tamaño del buffer de recepción
  int flags;                           // Banderas para configuración de puerto serie

public:
   char nombre[10];
  //Constructores
  //-------------------------------------------------------------------------
  //cosntructor por defecto
  ComunicacionPuertoSerie(void);
  //constructor general
  ComunicacionPuertoSerie(datos_terminal pTerm,QString sPuerto, int bs, int fl);

  //SetThings
  //-------------------------------------------------------------------------
  void SetFd(int iDescriptor);
  void SetLectura(char *pcEntrada);
  void SetEscritura(char *pcSalida);
  void SetTerminal(datos_terminal pTerm);
  void SetModemdevice(QString sPuerto);


  //GetThings
  //-------------------------------------------------------------------------
  int GetFd(void);
  char *GetLectura(void);
  char *GetEscritura(void);
  char *GetTira(void);
  datos_terminal *GetTerminal(void);
  QString GetModemdevice(void);
  //QString GetModemdecive(void);
  //char *GetModemdevice(void);

  //Funciones miembro de la clase
  //-------------------------------------------------------------------------
  //inicializa y abre el puerto de comunicacion RS_232_C
  int configurarPuerto(void);
  //lee del buffer de entrada del puerto serie
  int leerPuerto(void);
  //escribe en el buffer de salida del puerto serie
  int escribirPuerto(void);
  //reconfigura el puerto con los parametros antiguos y cierra el puerto
  int cerrarPuerto(void);


  // Destructor de la clase
  //-------------------------------------------------------------------------
  ~ComunicacionPuertoSerie();

};
#endif
