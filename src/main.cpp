/***************************************************************************
 *   Copyright (C) 2005 by Susana Martín                                   *
 *   susmar@cartif.es                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


/**
 * @file main.cpp
 * @brief En la función main se ejecuta el código necesario para establecer una 
 *        comunicación a través del puerto serie con la bomba Plum XLD, siendo
 *        capaz de recolectar e interpretar los datos proporcionados por ésta.
 *        Esta función se comporta, por tanto, como driver del dispositivo.      
 *        E:/ int atgc     : número de argumentos que se pasan a la función
 *            char *argv[] : argumentos que se pasan a la función
 *        S:/ int          : entero que indica el estado al finalizar la función
 *                           (-1 -> estado de error, o -> estado sin error)
 * @author Susana Martín Toral
 * @date 03/02/05
 * @version 1.0
 * @note --> Código por completar, quizás sea necesario meter señales de 
 *           notificación, en función de la implementación del resto del 
 *           sistema <--
 * @warning Avisos importantes (puede estar en blanco)
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream.h>
#include <stdlib.h>

//se incluyen los ficheros.h de las clases a usar
#include "driverBomba.h"

int main(int argc, char *argv[])
{
  char *vEntrada;                      //Respuesta que se recibe de la bomba
  char vSalida2[]="T0;IALR;4260";       //Preguntar por las alarmas
  char vSalida[]="T0;IDTY;8BC8";       //Preguntar por la identificacion        
  //char pcPuerto[]="/dev/ttyS1";
  char pcPuerto[10];
   
  if(argc != 2){
     printf("\nUSO: ./driverbomba /dev/ttyS*\n\n");
     return UCI_ERROR;
  }  
  
  strcpy(pcPuerto,argv[1]);
    
  //inicializa la estructura que contiene los parametros de control para la
  //transmision
  datos_terminal pTerm={VELOCIDAD,PARIDAD,BIT_DATOS,BIT_START,BIT_STOP,TMUESTREO,
                        TOUT_C,TOUT_M};

  //construcción de objetos: uno por cada bomba	
  driverBomba bomba(pTerm,pcPuerto);
    
  printf("\nAbriendo y configurando el puerto serie...");
  if((bomba.configurarPuerto())==-1)
     printf("ERROR en la apertura y configuracion del puerto\n");
  else printf("OK\n"); 

//===========================================================================    
  char vFin[1];                        //para almacenar el caracter <CR>
  vFin[0]=13;   
 
  
  strcat(vSalida,vFin);                //añade al final del CP el caracter <CR>
  
  vFin[0]=10;
  strcat(vSalida,vFin);
  //vSalida[strlen(vSalida)+1]='\0';     //añade al final del CP un \0 para indicar 
                                       //fin de la cadena
  vSalida[strlen(vSalida)+1]='\n';     //añade al final del CP un \0 para indicar 
                                       //fin de la cadena
  vSalida[strlen(vSalida)+2]='\0';     //añade al final del CP un \0 para indicar 
                                       //fin de la cadena
				       
  bomba.SetEscritura(vSalida);         //almacena en la clase el paquete a enviar
    
  printf("\nEscribiendo datos en el puerto serie...");
  if((bomba.escribirPuerto())==-1)
     printf("ERROR en la escritura en el puerto\n");
  else printf("OK\n"); 
    
  printf("\nRecibiendo datos de puerto serie...");
  if((bomba.leerPuerto())==-1)
     printf("ERROR en la lectura del puerto\n");
  else printf("OK\n");
  
  vEntrada=bomba.GetTira();            //recupera de la clase el paquete recibido
  printf("\n-------------------------------------------------------\n");
  printf("\nLa respuesta obtenida de la bomba es: %s\n",vEntrada);
  printf("\n-------------------------------------------------------\n");
   
  //antes de abandonar el programa, o cuando se quiera cerrar el puerto serie, se debe
  //reconfigurar y cerrar el puerto
  printf("Cerrando el puerto de comunicación...");
  if(bomba.cerrarPuerto()==-1)
     printf("ERROR al cerrar el puerto serie\n");
  else printf("OK\n");
  
  
  printf("\n\n\n\n\t\t\t FIN DEL PROGRAMA\n\n\n\n");
  return UCI_OK;
}
