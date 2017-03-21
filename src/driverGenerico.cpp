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

#include "driverGenerico.h"
#include <qstring.h>


driverGenerico::driverGenerico(QString sTipo){
  	//sTipoEquipo=new char[strlen(sTipo)+1];
	sTipoEquipo=sTipo;
	//strcpy(sTipoEquipo,sTipo);
}

//-------------------------------------------------------------------------------------

/**
 * obtenerFechaHoraEquipo():   
 * @brief Función que proporciona la fecha y la hora del momento de su 
 *        llamada.
 *        E/S:/ QString : fecha en el formato dd/mm/aaaa
 *        E/S:/ QString : hora en el formato hh:mm:ss
 * @author Susana Martín Toral
 * @date 07/06/05
 * @version 1.0
 * @note Notas que puedan ser de interés (puede estar en blanco)
 * @warning Avisos importantes (puede estar en blanco)
 */
QStringList driverGenerico::obtenerFechaHoraEquipo(void){
	QDateTime fechaHora=QDateTime::currentDateTime();
	QDate fecha=fechaHora.date();
	QTime hora=fechaHora.time();
	QString sDia, sMes, sAnio, sHora, sMinutos, sSegundos, sCad;
	QString sFecha, sTime;
	QStringList listaFechaTotal;
	int iDia=fecha.day();
	int iMes=fecha.month();
	int iAnio=fecha.year();
	int iHora=hora.hour();
	int iMinutos=hora.minute();
	int iSegundos=hora.second();
	
	if(iDia<10)
		sDia="0"+sCad.setNum(iDia);
	else
		sDia=sCad.setNum(iDia);
	if(iMes<10)
		sMes="0"+sCad.setNum(iMes);
	else
		sMes=sCad.setNum(iMes);
	sAnio=sCad.setNum(iAnio);
	if(iHora<10)
		sHora="0"+sCad.setNum(iHora);
	else
		sHora=sCad.setNum(iHora);
	if(iMinutos<10)
		sMinutos="0"+sCad.setNum(iMinutos);
	else
		sMinutos=sCad.setNum(iMinutos);
	if(iSegundos<10)
		sSegundos="0"+sCad.setNum(iSegundos);
	else
		sSegundos=sCad.setNum(iSegundos);
	
	
	sFecha=sDia+"/"+sMes+"/"+sAnio;
	sTime=sHora+":"+sMinutos+":"+sSegundos;
	listaFechaTotal<<sFecha<<sTime;
	
	return listaFechaTotal;
	
}
