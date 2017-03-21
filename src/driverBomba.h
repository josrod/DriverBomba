/***************************************************************************
                          driverBomba.h  -  description
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

/**
 * @file driverBomba.h
 * @brief Cabecera de la clase driverBomba, que contiene todos
 *        los atributos y métodos necesarios para llevar a cabo
 *        comuniaciones con la bomba modelo XL a través del
 *        puerto serie RS-232
 * @author Susana Martín Toral
 * @date 28/01/05
 * @version 1.0
 * @note Clase por completar
 * @warning Avisos importantes (puede estar en blanco)
 */

#ifndef DRIVERBOMBA_H
#define DRIVERBOMBA_H 
 
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Definiciones.h"              //definiciones usadas en el sistema
#include "ComunicacionPuertoSerie.h"
#include "driverGenerico.h"
#include <ctype.h>

//anadido por susana
#include <iostream.h>
#include "vital.h"
#include <qstringlist.h>
#include <qdatetime.h>
#include <iostream.h>
#include <vector>
#include "traductorVital.h"

#ifdef HAVE_TIME_H
#include <time.h>
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

//---------------------------------------------------------------------------
//    Para la comunicacion serie asincrona con la bomba los parametros del canal
//  deben ser:
//        - velocidad = 1200 baudios.
//        - nº bits de datos = 8 bits.
//        - sin paridad.
//        - nº de bits de start = 1 bit
//        - nº bits de stop = 1 bit.
//    Estos valores son los que se ponen por defecto, aunque los parametros
//  se pueden configurar con otros valores distintos a través del propio driver.
//---------------------------------------------------------------------------

namespace uciEquiposCabecera {
namespace uciBomba{

#define VELOCIDAD 1200
#define PARIDAD 0
#define BIT_DATOS 8
#define BIT_START 1
#define BIT_STOP 1
#define FALSE 0
#define TRUE 1
//#define BUFSIZE 1
#define BUFSIZE 64
#define TOUT_C 1
#define TOUT_M 1                       //supuestamente 40ms
//por defecto el muestreo de medidas se realiza cada 600 segundos (10 minutos)
#define TMUESTREO 600
//por defecto el muestreo de alarmas se realiza cada 60 segundos (1 minutos)
#define TALARMAS 60


/**
 * @def UCI_BOMBA_CRC_LEN
 * @brief Código para representar la longitud del CRC
 */
#define UCI_BOMBA_CRC_LEN 4

/**
 * @def UCI_BOMBA_BLANK
 * @brief Código para representar el espacio en blanco en códicgo ASCII
 */
#define UCI_BOMBA_BLANK 0x20

/**
 * @def UCI_BOMBA_MAX_ASCII
 * @brief Código para representar la longitud máxima de una cadena ASCII
 */
#define UCI_BOMBA_MAX_ASCII 256

/**
 * @def UCI_BOMBA_CCCITT_REV
 * @brief Código para representar el polinomio inverso para caclular el CRC
 */
#define UCI_BOMBA_CCCITT_REV 0x8408

/**
 * @def UCI_BOMBA_IDENT
 * @brief Código para representar el comando "Identificar Bomba"
 */
#define UCI_BOMBA_IDENT 256


/**
 * @def UCI_BOMBA_SET_ID
 * @brief Código para representar el comando "Set ID Soft"
 */
#define UCI_BOMBA_SET_ID 257

/**
 * @def UCI_BOMBA_GET_ID
 * @brief Código para representar el comando "Get ID Soft"
 */
#define UCI_BOMBA_GET_ID 258

/**
 * @def UCI_BOMBA_CHEQ_AL
 * @brief Código para representar el comando "Chequeo de Alarmas"
 */
#define UCI_BOMBA_CHEQ_AL 259

/**
 * @def UCI_BOMBA_CHEQ_STA
 * @brief Código para representar el comando "Chequeo de Status"
 */
#define UCI_BOMBA_CHEQ_STA 260

/**
 * @def UCI_BOMBA_LECT_PARAM
 * @brief Código para representar el comando "Lectura de parámetros"
 */
#define UCI_BOMBA_LECT_PARAM 261

#define dp1_dpL 1
#define dv1_vt1_vol 2
#define dv2_vt2_hbt 3

#define UCI_BOMBA_ABOT 262
#define UCI_BOMBA_ABOT_DP1 263
// Añadido por Jose Luis
/**
 * @def UCI_BOMBA_CHEQ_ALH
 * @brief Código para representar el comando "Lectura del Historial de Alarmas"
 */
#define UCI_BOMBA_CHEQ_ALH 264

class driverBomba : public ::driverGenerico
{
protected:


  //<begin> Añadido por Leonardo
  datos_terminal pTerm;
  
  //cambiado por susana
  //char pcPuerto[12];
  QString sPuerto;
  //Vector que contiene una serie de estructuras de alarmas
  std::vector<Alarma> vVectorAlarmas;
  //fin cambiado por susana
  
  int crc_tab[UCI_BOMBA_MAX_ASCII]; // Tabla para almacanar los valores de CRC de cada byte
  char paquete[UCI_BOMBA_MAX_ASCII]; // Cadena donde se guarda el paquete a enviar a la bomba
  char respuesta[UCI_BOMBA_MAX_ASCII]; // Cadena donde se guarda el paquete recibido
  // Datos de la bomba:

  char ID_Bomba[UCI_BOMBA_MAX_ASCII], //Aquí se guarda el nombre de la bomba
                                      //tal y como esta la reporta
       STA[UCI_BOMBA_MAX_ASCII], //Aquí se guarda el estado de la bomba
       ALR[UCI_BOMBA_MAX_ASCII],//Aqui se guardan las alarmas de la bomba
       ALH[1000]; //Aquí se guardan el historico de las ultimas 15 alarmas 
  int sw; //versión de software
  //Valores reportados por la bomba:
  float dp1; //Presión distal
  float dpL; //Límite de presión distal
  float dv1; // Tasa de administración, línea 1
  float dv2; // Tasa de administración, línea 2
  float hbt; //Horas de operación con batería
  float vol; //Volumen total administrado
  float vt1; //Volumen a ser administrado, línea 1
  float vt2; //Volumen a ser administrado, línea 2
  bool alarma; //Indica si existe en la boma una condición de alarma
  char ID_soft[4]; // Cadena que guarda la dirección software de la bomba
  char ID_hard[2]; // Cadena que guarda la dirección hardware de la bomba
  float fMedidas[8];


  //Funciones internas
  void i_crc (void); // Inicializa la variable "crc_tab" necesaria para CRC
  char *crc_get(char *packet, int len); // Recibe una cadena y su longitud y calcula y devuelve el crc
  int crchware (unsigned short data, unsigned short genpoly, unsigned short accum);
  int CRC_check(char * r); // Verifica que el CRC reportado al final de la cadena de entrada (r) sea correcto
  bool ID_s_check (char *s);
  bool ID_h_check (char *s);
  bool ALH_check(char *s); //.
  bool ALR_check ( char *s); //Devuelve TRUE si hay condicion de alarma en la bomba, FALSE si no.
  void parse (char *p, char *s); // p es una cadena de 3 letras, la funcion parse la expande y devuelve s.
  // Añadido por Jose Luis
  void parsehistorial (char *p, char *s); // p es una cadena de 2 digitos, la funcion parse la expande y devuelve s.
  
  int Escribir (void); // Intenta escribir hasta 2 veces lo que haya en el bufer de salida. Devuelve UCI_OK o UCI_ERROR
  int Leer(void); // Intenta leer hasta 2 veces lo que haya en el bufer de entrada. Devuelve UCI_OK o UCI_ERROR
  int codificar (unsigned short int comando, unsigned short int param); //construye un paquete para ser enviado a la bomba
  int decodificar (int comando, unsigned short int param); // Analiza la respuesta de la bomba y llena las variables correspondientes
   
 //prueba añadida por susana
  //Estructuras VITAL necesarias para trabajar con las bombas
  VMD pVmd;						//Estructura del dispositivo medico virtual
  Channel pCSource,pCDelivery1,pCDelivery2;		//Estructuras de los canales source y delivery
  Metric pDP1,pDPL,pDV1,pDV2,pVOL,pVT1,pVT2,pHBT,pHTT;	//Estructuras de las diversas metricas de las bombas
  //fin prueba añadida por susana
 
public:

  ComunicacionPuertoSerie puerto;


  //Constructores
  //-------------------------------------------------------------------------
  //cosntructor por defecto
  //driverBomba(void);
  //constructor general

  driverBomba(QString sParamPuerto,QString sParamTipo);

  int Probar(void);

  int Configurar (void);

  void SetRespuesta(char *r); //función auxiliar. Posiblemente útil para debuging

  char *GetRespuesta(void); //función auxiliar. Posiblemente útil para debuging
  char *GetPaquete(void);  //función auxiliar. Posiblemente útil para debuging
  char *GetAlarmas(void);  // Devuelve la lista de Alarmas
  int Comunicar(int comando, unsigned short int param); // Hace uso de varias funciones de bajo nivel para llevar a cabo la comunicacion
  bool HayAlarmas(void); //Indica si hay o no condición de alarma en la bomba

  //añadido por susana
  IdDispositivo Identificar(void);
  int FinalizarComunicacion(void);
  std::vector<Alarma> *ObtenerAlarmas(void);
  //fin añadido por susana
  
  // añadido por jose luis
 // char *driverBomba::obtenerDatos(float fMedidas[8]);
  //fin añadido por Jose Luis
 
  
  
  //prueba añadida por susana
  void InicializarVITAL(TraductorVital traductor);
  VMD *ObtenerDatos(TraductorVital traductor);
  //fin prueba añadida por susana
 
  //Destructor
  //-------------------------------------------------------------------------
  ~driverBomba();

};

};
};

#endif

