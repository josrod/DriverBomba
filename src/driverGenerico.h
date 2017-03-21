/***************************************************************************
                          driverGenerico.h  -  description
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

#ifndef UCIDRIVERGENERICO_H
#define UCIDRIVERGENERICO_H 

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <qstring.h>
#include "vital.h"
#include <qstringlist.h>
#include <qdatetime.h>
#include <vector>
#include "traductorVital.h"

 
 
class driverGenerico
{
protected:
  //char * sTipoEquipo;
  QString sTipoEquipo;
  
public:

  //driverGenerico(char *sTipo);
  driverGenerico(QString sTipo);
  //Funciones virtuales puras 
  virtual int codificar(unsigned short int comando, unsigned short int param) = 0;
  virtual int decodificar(int comando, unsigned short int param) = 0;
  
  //---------------- añadido por Susana ----------------------------------------
  virtual int Probar(void) = 0;
  virtual IdDispositivo Identificar(void) = 0;
  virtual int FinalizarComunicacion(void) = 0;
  //virtual EstructuraVital ObtenerDatos(void) = 0; // puedo ponerlo como un void de momento
  virtual std::vector<Alarma> *ObtenerAlarmas(void) = 0;
  QStringList obtenerFechaHoraEquipo(void);
  //---------------- fin añadido por Susana ------------------------------------
 //virtual VMD ObtenerDatos(void) = 0; // puedo ponerlo como un void de momento
  
  //prueba añadido por susana
  virtual void InicializarVITAL(TraductorVital traductor) = 0;
  virtual VMD *ObtenerDatos(TraductorVital traductor) = 0; 
  //fin prueba añadido por susana

  
};

#endif
