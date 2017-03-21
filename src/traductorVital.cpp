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
 
 //cabecera de la clase
 #include "traductorVital.h"
	
	//Traductor del objeto Numeric
	Numeric traducirNumeric();
	//Funcion para obtener la fecha y la hora. Ver si se puede reaprovechar la que ya esta hecha
	QStringList obtenerFechaHora();
 
 
/**
 * TraductorVital()
 * @brief Constructor general de la clase.
 *        E:/ 
 *        S:/ 
 * @author Susana Martín Toral
 * @date 09/08/05
 * @version 1.0
 * @note
 * @warning
 */
TraductorVital::TraductorVital()
{


}


/**
 * ~TraductorVital()
 * @brief Destructor de la clase.
 *        E:/ 
 *        S:/ 
 * @author Susana Martín Toral
 * @date 09/08/05
 * @version 1.0
 * @note
 * @warning
 */
TraductorVital::~TraductorVital()
{


}


/**
 * traducirVMD():
 *        E:/ iParticion : Identificador en nomenclatura VITAL. En este caso lo 
 *                         correspondiente a VMD
 *        E:/ iCodigo: Codigo en nomenclatura VITAL
 *        E:/ iId: Identifcador unico del dispositivo VMD
 *        E:/ sEtqueta: Representacion en texto del tipo, dado por iPartition e iCode
 *        E:/ bEstado: Estado en el que se encuentra en un momento dado la bomba
 *        E:/ sModelo: Modelo de dispositivo conel que se trabaja
 *        E:/ sFabricante: Fabricante del dispositivo con el que de trabaja
 *        E:/ canales: Vector con el conjunto de canales con el que trabaja el dispositivo
 *        S:/ VMD* : Puntero a una estructura del tipo VMD con informacion sobre un equipo
 * @brief  Traduce los datos dados por el equipo de cabecera en un objeto con la 
 *         estructura de un VMD
 * @author Susana Martin Toral
 * @date 10/08/05
 * @version 1.0
 * @note
 * @warning
 */
VMD TraductorVital::traducirVMD(int iParticion, int iCodigo, int iId, QString sEtiqueta, 
                                 short int siEstado, QString sModelo, QString sFabricante)
{
	//Estructura de tipo VITAL que se debe obtener del equipo de cabecera
	VMD pDatosEquipo;
	
	//((***)) Si es necesario, antes de rellenar la estructura de informacion
	//sera necesaraio transformar los valores correspondientes a su formato
	//adecuado VITAL. Preguntar a Jose Luis por los formatos originales de la
	//informacion.
	pDatosEquipo.type.iPartition=iParticion;
	pDatosEquipo.type.iCode=iCodigo;
	pDatosEquipo.iHandle=iId;
	pDatosEquipo.sLabel=sEtiqueta;
	pDatosEquipo.siStatus=siEstado;
	pDatosEquipo.sModelo=sModelo;
	pDatosEquipo.sFabricante=sFabricante;
	
	return pDatosEquipo;
	
	//QVector<type> & QVector::operator= ( const QVector<type> & v )
}


/**
 * traducirCanal():
 *        E:/ iParticion : Identificador en nomenclatura VITAL. En este caso lo 
 *                         correspondiente a VMD
 *        E:/ iCodigo: Codigo en nomenclatura VITAL
 *        E:/ iId: Identifcador unico del canal
 *        E:/ sEtqueta: Representacion en texto del tipo, dado por iPartition e iCode
 *        E:/ iIdCanal: Estado en el que se encuentra en un momento dado la bomba
 *        E:/ iNumCanalFisico: Modelo de dispositivo conel que se trabaja
 *        E:/ iOpMod:
 *        E:/ iOpEst:
 *        E:/ medidas: Vector con el conjunto de canales con el que trabaja el dispositivo
 *        S:/ Channel* : Puntero a una estructura del tipo Channel con informacion sobre un canal  
 * @brief  Traduce los datos dados por el equipo de cabecera en un objeto con la 
 *         estructura de un canal
 * @author Susana Martin Toral
 * @date 10/08/05
 * @version 1.0
 * @note
 * @warning
 */
Channel TraductorVital::traducirCanal(int iParticion, int iCodigo, int iId, QString sEtiqueta, int iIdCanal,
                                       int iNumCanalFisico, int iOpMod, int iOpEst)
{
	//Estructura de tipo VITAL que se debe obtener del equipo de cabecera
	Channel pDatosCanal;
	
	//((***)) Si es necesario, antes de rellenar la estructura de informacion
	//sera necesaraio transformar los valores correspondientes a su formato
	//adecuado VITAL. Preguntar a Jose Luis por los formatos oritinales de la
	//informacion.
	pDatosCanal.type.iPartition=iParticion;
	pDatosCanal.type.iCode=iCodigo;
	pDatosCanal.iHandle=iId;
	pDatosCanal.sLabel=sEtiqueta;
	pDatosCanal.iCh_id=iIdCanal;
	pDatosCanal.iPhys_ch_num=iNumCanalFisico;
	pDatosCanal.iOm=iOpMod;
	pDatosCanal.iOs=iOpEst;
	//Borrar: pDatosCanal.medidas=medidas;
	
	return pDatosCanal;
  
}


/**
 * traducirMetrica():
 *        E:/ iParticion : Identificador en nomenclatura VITAL. En este caso lo 
 *                         correspondiente a VMD
 *        E:/ iCodigo: Codigo en nomenclatura VITAL
 *        E:/ iId: Identifcador unico de la metrica
 *        E:/ sEtiqueta: Representacion en texto del tipo, dado por iPartition e iCode
 *        E:/ iId_metrica: Identificador de la metrica empleada
 *        E:/ iPeriodoActualizacion: Periodo de actualizacion de la metrica
 *        E:/ siCategoria: Distinguidor de los diversos tipos de medidas obtendidas de diversas formas
 *        E:/ siAcceso: Como y cuando esta disponible un valor metrica
 *        E:/ siEstructura: Indica si se trata de un valor unico o relacionados multiples
 *        E:/ siRelevancia: Forma en que se utiliza la metrica obtenida
 *        E:/ iUnidades: Identificador de las unidades empleadas en la metrica
 *        E:/ sUnidades: Unidades empleadas en la metrica en texto
 *        S:/ Metric* : Puntero a una estructura del tipo Metric con informacion sobre una metrica
 * @brief  Traduce los datos dados por el equipo de cabecera en un objeto con la 
 *         estructura de una metrica
 * @author Susana Martin Toral
 * @date 10/08/05
 * @version 1.0
 * @note
 * @warning
 */
Metric TraductorVital::traducirMetrica(int iParticion, int iCodigo, int iId, QString sEtiqueta, 
                                        unsigned int iId_metrica, int iPeriodoActualizacion, 
					short int siCategoria, short int siAcceso, short int siEstructura, 
					short int siRelevancia, unsigned int iUnidades, QString sUnidades)
{

	//Estructura de tipo VITAL que se debe obtener del equipo de cabecera
	Metric pDatosMetrica;
	
	//((***)) Si es necesario, antes de rellenar la estructura de informacion
	//sera necesaraio transformar los valores correspondientes a su formato
	//adecuado VITAL. Preguntar a Jose Luis por los formatos oritinales de la
	//informacion.
	pDatosMetrica.type.iPartition=iParticion;
	pDatosMetrica.type.iCode=iCodigo;
	pDatosMetrica.iHandle=iId;
	pDatosMetrica.sLabel=sEtiqueta;
	pDatosMetrica.uiMetric_id=iId_metrica;
	pDatosMetrica.iUpdate_period=iPeriodoActualizacion;
	pDatosMetrica.siCategory=siCategoria;
	pDatosMetrica.siAccess=siAcceso;
	pDatosMetrica.siStructure=siEstructura;
	pDatosMetrica.siRelevance=siRelevancia;
	pDatosMetrica.uiUnit_code=iUnidades;
	pDatosMetrica.sUnit_string=sUnidades;
	
	return pDatosMetrica;
}
	

/**
 * traducirNumeric():
 *        E:/ iParticion : Identificador en nomenclatura VITAL. En este caso lo 
 *                         correspondiente a VMD
 *        E:/ iCodigo: Codigo en nomenclatura VITAL
 *        E:/ iId: Identifcador unico del valor numerico
 *        E:/ sEtiqueta: Representacion en texto del tipo, dado por iPartition e iCode
 *        E:/ metrica: Metrica a la que pertenece el valor numerico
 *        E:/ fValor: Valor numerico tomado del dispositivo
 *        E:/ sFecha: Fecha de la toma de la muestra
 *        E:/ sHora: Hora de la toma de la muestra
 *        E:/ siEstado: Estado en el que se encuentra la toma de la muestra
 *        S:/ Numeric* : Puntero a una estructura del tipo Numeric con informacion sobre un valor numerico
 * @brief  Traduce los datos dados por el equipo de cabecera en un objeto con la 
 *         estructura de un valor numerico
 * @author Susana Martin Toral
 * @date 10/08/05
 * @version 1.0
 * @note
 * @warning
 */
Numeric TraductorVital::traducirNumeric(int iParticion, int iCodigo, int iId, QString sEtiqueta, 
                                         Metric metrica, float fValor, QString sFecha, QString sHora, 
					 short int siEstado)
{

	//Estructura de tipo VITAL que se debe obtener del equipo de cabecera
	Numeric pDatosNumeric;
	
	//((***)) Si es necesario, antes de rellenar la estructura de informacion
	//sera necesaraio transformar los valores correspondientes a su formato
	//adecuado VITAL. Preguntar a Jose Luis por los formatos oritinales de la
	//informacion.
	pDatosNumeric.type.iPartition=iParticion;
	pDatosNumeric.type.iCode=iCodigo;
	pDatosNumeric.iHandle=iId;
	pDatosNumeric.sLabel=sEtiqueta;
	pDatosNumeric.ms=metrica;
	pDatosNumeric.fValue=fValor;
	pDatosNumeric.sFecha=sFecha;
	pDatosNumeric.sHora=sHora;
	pDatosNumeric.siState=siEstado;
		
	return pDatosNumeric;
}


/**
 * setStatusVMD(VMD *pVmd,QString sEstado):
 *        E:/ pVmd: Puntero a una estructura del tipo VMD
 *        E:/ sEstado: Estado en el que se encuentra el equipo de cabecera
 * @brief  Actualiza el valor del estado del equipo en la estructura VMD
 * @author Susana Martin Toral
 * @date 29/08/05
 * @version 1.0
 * @note
 * @warning
 */
void TraductorVital::setStatusVMD(VMD *pVmd, QString sEstado)
{
	short int siEstado=0;
	
	if(sEstado=="Apagado"){
		siEstado|=int(pow(2,0));
		//siEstado=siEstado|1;
	}
	if(sEstado=="No listo"){
		siEstado|=int(pow(2,1));
	}
	
	pVmd->siStatus=siEstado;

}
 

/**
 * setModeloVMD(VMD *pVmd,QString sModelo):
 *        E:/ pVmd: Puntero a una estructura del tipo VMD
 *        E:/ sModelo: Modelo del dispositivo
 * @brief  Actualiza el modelo del equipo VMD
 * @author Susana Martin Toral
 * @date 29/08/05
 * @version 1.0
 * @note
 * @warning
 */
void TraductorVital::setModeloVMD(VMD *pVmd, QString sModelo)
{
	pVmd->sModelo=sModelo;
}


/**
 * setOEChannel(Channel *pCh, QString sOe):
 *        E:/ pCh: Puntero a una estructura del tipo Channel
 *        E:/ iOe: Estado de operacion
 * @brief  Actualiza el modo de operacion del canal
 * @author Susana Martin Toral
 * @date 29/08/05
 * @version 1.0
 * @note
 * @warning
 */
void TraductorVital::setOEChannel(Channel *pCh, QString sOe)
{
	int iOe=0;	

	if(sOe=="Backpriming"){
		iOe|=int(pow(2,24));		//ponemos a '1' el bit 24
	}
	if(sOe=="Malfuntion"){
		iOe|=int(pow(2,25));		//ponemos a '1' el bit 25
	}
	if(sOe=="Modo bombeo KVO"){
		iOe|=int(pow(2,1));		//ponemos a '1' el bit 1
	}
	if(sOe=="Bombeo primario"){
		iOe|=int(pow(2,0));		//ponemos a '1' el bit 0
	}
	if(sOe=="Bombeo secundario"){
		iOe|=int(pow(2,0));		//ponemos a '1' el bit 0
	}
	if(sOe=="Detenido, puerta abierta"){
		iOe|=int(pow(2,5));		//ponemos a '1' el bit 5
	}
	if(sOe=="Detenido"){
		iOe|=int(pow(2,4));		//ponemos a '1' el bit 4
	}
	
	pCh->iOs=iOe;
	
}


/**
 * setPeriodo(Metric *pMetric, int iT):
 *        E:/ pMetric: Puntero a una estructura del tipo Metric
 *        E:/ iT: Periodo de actualizacion de los datos
 * @brief  Actualiza el modo de operacion del canal
 * @author Susana Martin Toral
 * @date 29/08/05
 * @version 1.0
 * @note
 * @warning
 */
void TraductorVital::setPeriodo(Metric *pMetric, int iT)
{
	pMetric->iUpdate_period=iT;
	
}


/**
 * setValue(Numeric *pNum, int iValor):
 *        E:/ pNum: Puntero a una estructura del tipo Numeric
 *        E:/ iValor: Valor entero obtenido del equipo
 * @brief  Indicar el valor numerico obtenido del equipo
 * @author Susana Martin Toral
 * @date 29/08/05
 * @version 1.0
 * @note
 * @warning
 */
void TraductorVital::setValue(Numeric *pNumeric, int iValor)
{
	pNumeric->fValue=float(iValor);
	
}


/**
 * setValue(Numeric *pNumeric, float fValor):
 *        E:/ pNum: Puntero a una estructura del tipo Numeric
 *        E:/ fValor: Valor flotante obtenido del equipo
 * @brief  Indicar el valor numerico obtenido del equipo
 * @author Susana Martin Toral
 * @date 29/08/05
 * @version 1.0
 * @note
 * @warning
 */
void TraductorVital::setValue(Numeric *pNumeric, float fValor)
{
	pNumeric->fValue=fValor;
	
}


/**
 * setFechaHora(Numeric *pNumeric, QString sFecha, QString sHora):
 *        E:/ pNum: Puntero a una estructura del tipo Numeric
 *        E:/ sFecha: Fecha de toma de la medida
 *        E:/ sHora: Hora de toma de la medida
 * @brief  Indicar la fecha y hora de obtencion de la medida
 * @author Susana Martin Toral
 * @date 29/08/05
 * @version 1.0
 * @note
 * @warning
 */
void TraductorVital::setValue(Numeric *pNumeric, QString sFecha, QString sHora)
{
	pNumeric->sFecha=sFecha;
	pNumeric->sHora=sHora;
	
}


/**
 * setStateNumeric(Numeric *pNumeric, QString sEstado):
 *        E:/ pNumeric: Puntero a una estructura del tipo Numeric
 *        E:/ QString: Estado de la medicion
 * @brief  Indica el estado de la medida realizada al equipo
 * @author Susana Martin Toral
 * @date 29/08/05
 * @version 1.0
 * @note
 * @warning
 */
void TraductorVital::setStateNumeric(Numeric *pNumeric, QString sEstado)
{
	short int siEstado=0;
	
	if(sEstado=="invalid"){
		siEstado|=int(pow(2,0));
	}
	if(sEstado=="questionable"){
		siEstado|=int(pow(2,1));
	}
	if(sEstado=="not-available"){
		siEstado|=int(pow(2,2));
	}
	if(sEstado=="calibration-ongoing"){
		siEstado|=int(pow(2,3));
	}
	if(sEstado=="test-data"){
		siEstado|=int(pow(2,4));
	}
	if(sEstado=="demo-data"){
		siEstado|=int(pow(2,5));
	}
	if(sEstado=="validated-data"){
		siEstado|=int(pow(2,8));
	}
	if(sEstado=="early-indication"){
		siEstado|=int(pow(2,9));
	}
	if(sEstado=="msmt-ongoing"){
		siEstado|=int(pow(2,10));
	}
	
	pNumeric->siState=siEstado;
	
}


/**
 * addChannel(VMD *pVmd, Channel pCh):
 *        E:/ *pVmd: Puntero a una estructura del tipo VMD
 *        E:/ pCh: Estructura del tipo Channel
 *        S:/ int: Indica si el canal se ha añadido con exito o  no
 * @brief  Añade un canal a una estructura VMD
 * @author Susana Martin Toral
 * @date 29/08/05
 * @version 1.0
 * @note
 * @warning
 */
int TraductorVital::addChannel(VMD *pVmd, Channel pCh)
{
	pVmd->canales.push_back(pCh);
	return 1;
}


/**
 * addNumeric(Channel *pCh, Numeric pNumeric):
 *        E:/ *pCh: Puntero a una estructura del tipo Channel
 *        E:/ pNumeric: Estructura del tipo Numeric
 *        S:/ int: Indica si el canal se ha añadido con exito o no
 * @brief  Añade una metrica a una estructura Channel
 * @author Susana Martin Toral
 * @date 29/08/05
 * @version 1.0
 * @note
 * @warning
 */
int TraductorVital::addNumeric(Channel *pCh, Numeric pNumeric)
{
	pCh->medidas.push_back(pNumeric);
	return 1;
}






