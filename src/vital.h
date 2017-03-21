
/***************************************************************************
                          vital.h  -  description
                             -------------------
    begin                : Tue Mar 29 2005
    copyright            : (C) 2005 by Leonardo Santamarina Gzyl and
                                       Susana Martin Toral
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

	#ifndef UCI_VITAL_H
	#define UCI_VITAL_H

	#include <time.h>
	#include <vector>

	/** creacion del nuevo tipo Type (Atributo tipo de VITAL)
 	* @def 
 	* @brief 
 	*/
	typedef struct {
 	int iPartition;			//Particion (VMD, canal o metrica)
 	int iCode;			//Codigo (especifico para cada uno de los anteriores.
 					//Por ejemplo, para la bomba de infusion:
 					//	VMD = NOM_DEV_PUMP_INFUS_VMD
					//	Canal = NOM_DEV_PUMP_INFUS_CHAN
					//	Metrica = NOM_FLOW_FLUID_PUMP, por ejemplo para la 
					//		  frecuencia de administracion
	}Type;


	/** creacion del nuevo tipo Metric (Objeto metrica de VITAL)
 	* @def 
 	* @brief 
 	*/
	typedef struct {
 	Type type;			//Para metrica: partition=NOM_PART_SCADA
 	int iHandle;			//Identificador unico de la metrica
 	QString sLabel;			//Representacion en texto del tipo	
 	unsigned int uiMetric_id;	//Debe ser un codigo de la particion de nomenclatura para metricas (NOM_PART_OBJ)
 	int iUpdate_period; 		//período de actualización de los datos (medido en "ticks" 80 Hz).
 	short int siCategory;		//Para distinguir entre mediciones, entornos y calculos-- ELIMINAR
 	short int siAccess;		//Como es posible acceder al valor metrica y cuando esta disponible un nuevo valor-- ELIMINAR
 	short int siStructure;		//Valor único o valores relacionados multiples-- ELIMINAR?
 	short int siRelevance;		//En que forma debería utilizarse la metrica (internamente, para almacenamiento, etc.)--ELIMINAR
 	unsigned int uiUnit_code;	//Debe ser un codigo de la particion de nomenclatura para dimensiones (NOM_PART_DIM)
 	QString sUnit_string;		//Dimension en texto ¿supongo?
	}Metric;


	/** creacion del nuevo tipo Numeric (Objeto numeric de VITAL)
 	* @def 
 	* @brief 
 	*/
	typedef struct {
 	Type type;			//No se qué tipo toca para numerics ¿?
 	int iHandle;			//Identificador unico de la medida
 	QString sLabel;			//Representacion en texto del tipo
 	Metric ms;			//Metrica a la que pertenece la medida
 	float fValue;			//Valor de la medida
 	QString sFecha;			//Fecha de la medida
 	QString sHora;			//Hora de la medida
 	short int siState;		//Estado de la medida
	}Numeric;


	/** creacion del nuevo tipo Channel (Objeto canal de VITAL)
	* @def
	* @brief 
	*/
	typedef struct {
	Type type;			//Para channel: partition=NOM_PART_OBJ y code=NOM_DEV_PUMP_INFUS_CHAN
	int iHandle;			//Identificador unico del canal
	QString sLabel;			//Representacion en texto del tipo
	int iCh_id;			//Tipo de canal: debe ser o NOM_CHAN_ID_DELIVERY o NOM_CHAN_ID_SOURCE
	int iPhys_ch_num;		//Asocia el canal con un mecanismo de administracion
	int iOm;			//Modo de operacion del canal
	int iOs;			//Estado en el que se encuentra el canal
	std::vector<Numeric> medidas;	//Medidas para un determinado canal
	}Channel;


	/** creacion del nuevo tipo VMD (Objeto VMD de VITAL)
 	* @def 
 	* @brief 
 	*/
	typedef struct {
 	Type type;			//Para VMD: partition=NOM_PART_OBJ y code=NOM_DEV_PUMP_INFUS_VMD
 	int iHandle;			//Identificador unico del dispositivo (VMD)
 	QString sLabel;			//Representacion en texto del tipo
 	short int siStatus;		//Estado de la bomba
 	QString sModelo;		//Tipo de dispositivo (DTY)
 	QString sFabricante;		//Fabricante del dispositivo
 	std::vector<Channel> canales;	//Canales de un determinado VMD
	}VMD;


	/** creacion del nuevo tipo IdDispositivo (Identificador de Dispositivo)
 	* @def 
 	* @brief 
 	*/
	typedef struct{
	QString sTipoEquipo;		//Tipo del equipo con el que se trabaja (ventilador, bomba, monitor, oximetro)
	QString sModelo;		//Modelo del equipo con el que se trabaja
	QString sFabricante;		//Fabricante del equipo con el que se trabaja
	int iIdDispositivo;		//Identificador unico del dispositivo
	}IdDispositivo;


	/** creacion del nuevo tipo Alarma (Alarma generada por el dispositivo)
 	* @def 
 	* @brief 
 	*/
 	typedef struct{
	int iIdRegIngreso;	//Identificador del paciente para el que se producen las alarmas
	int iVmdHandle;		//Identificador del equipo que produce las alarmas
	QString sTipoEquipo;	//Tipo de equipo que produce la alarma. Puede ser:
				//      ventilador
				//	bomba
				//	monitor
				//	oximetro	
	QString sModelo;	//Modelo del equipo que produce la alarma
	int iTipoAlarma;	//Tipo de alarma que se ha generado. Puede ser:
				//	0 - alarma fisiológica generada por un equipo de cabecera
				//	1 - alarma técnica generada por un equipo de cabecera
				//	2 - alerta que el propio sistema genera asociada a un paciente
	QString sDescripcion;	//Descripcion en texto de la causa de la alarma
	int iPrioridad;		//Prioridad de la alarma generada. Puede ser:
				//	0 - baja
				//	1 - media
				//	2 - alta
	QString sFecha;		//Fecha de produccion o notificacion de la alarma
	QString sHora;		//Hora de produccion o notificacion de la alarma
	}Alarma; 

	#endif

