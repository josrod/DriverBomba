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

#ifndef UCITRADUCTORVITAL_H
#define UCITRADUCTORVITAL_H

#include <qdatetime.h>
#include <qapplication.h>
#include <qevent.h>
#include <iostream.h>
#include <qstringlist.h>
#include <qobject.h>
#include <stdlib.h>
#include <qtimer.h>
# include <iostream.h>
# include <string.h>
#include <qcstring.h>
#include <math.h>
//#include "Definiciones.h"
#include "vital.h"

/**
Clase que se encarga de traducir los datos especificos de los equipos a una
nomenclatura estandar en formato VITAL
@author Susana Martín Toral
@date 09-08-2005
@version 1
*/
class TraductorVital
{
  //Q_OBJECT
  
  
private:
   
	
	
  
public:
	
	//Constructor
	TraductorVital();
	//Destructor
	~TraductorVital();
	//Traductor del objeto VMD
	VMD traducirVMD(int iParticion, int iCodigo, int iId, QString sEtiqueta, 
                         short int siEstado, QString sModelo, QString sFabricante);
	//Traductor del objeto Canal
	Channel traducirCanal(int iParticion, int iCodigo, int iId, QString sEtiqueta, int iIdCanal,
                               int iNumCanalFisico, int iOpMod, int iOpEst);
	//Traductor del objeto Metrica
	Metric traducirMetrica(int iParticion, int iCodigo, int iId, QString sEtiqueta, 
                                unsigned int iId_metrica, int iPeriodoActualizacion, 
				short int siCategoria, short int siAcceso, short int siEstructura, 
				short int siRelevancia, unsigned int iUnidades, QString sUnidades);
	//Traductor del objeto Numeric
	Numeric traducirNumeric(int iParticion, int iCodigo, int iId, QString sEtiqueta, 
                                 Metric metrica, float fValor, QString sFecha, QString sHora, 
				 short int siEstado);
	//Funcion para obtener la fecha y la hora. Ver si se puede reaprovechar la que ya esta hecha
	//QStringList obtenerFechaHora();
	void setStatusVMD(VMD *pVmd, QString sEstado);
	void setModeloVMD(VMD *pVmd, QString sModelo);
	void setOEChannel(Channel *pCh, QString sOe);
	void setPeriodo(Metric *pMetric, int iT);
	void setValue(Numeric *pNumeric, int iValor);
	void setValue(Numeric *pNumeric, float fValor);
	void setValue(Numeric *pNumeric, QString sFecha, QString sHora);
	void setStateNumeric(Numeric *pNumeric, QString sEstado);
	int addChannel(VMD *pVmd, Channel pCh);
	int addNumeric(Channel *pCh, Numeric pNumeric);

	
    
};

//};

#endif
