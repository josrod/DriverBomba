/***************************************************************************
 *   Copyright (C) 2005 by artcas                                          *
 *   artcas@isc3                                                           *
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
 * @file Definiciones.h
 * @brief Archivo de cabecera que contiene definiciones de car�cter general
 *
 * @author Arturo Casas Sanz
 * @date 14-01-2005
 */

#ifndef UCI_DEFINICIONES_H
#define UCI_DEFINICIONES_H

namespace uci{

/**
 * @def UCI_ERROR
 * @brief C�digo para representar un error
 */

#define UCI_ERROR      -1

/**
 * @def UCI_OK
 * @brief C�digo para representar �xito en una operaci�n
 */

#define UCI_OK          0

/**
 * @defgroup CodigosEnvio Codigos para las comunicaciones entre m�dulos
 * Definiciones de los c�digos que se emplean en las comunicaciones entre m�dulos
 * @author Arturo Casas Sanz
 * @date 14-05-2004
 */
 /*@{*/


#define UCI_CONSULTASQL        0
#define UCI_ALARMA             1
#define UCI_AVISO              2
#define UCI_RESPUESTA          3
#define UCI_INFORMACION        4


/**
 * @def UCI_CONSULTASQL
 * @brief C�digo para representar una consulta a la base de datos
 *
 * No se recomienda que se ejecuten directamente consultas dentro del c�digo fuente del cliente, pero se reserva este c�digo para permitir esta posibilidad. Siempre que sea posible las consultas se realizar�n a trav�s de las operaciones que defina el m�dulo usando @link uci:UCI_OPERACION UCI_OPERACION @endlink
 */

/**
 * @def UCI_ALARMA
 * @brief C�digo para representar una alarma
 */

 /**
 * @def UCI_AVISO
 * @brief C�digo para representar un aviso
 */

 /**
 * @def UCI_RESPUESTA
 * @brief C�digo para representar una respuesta a una consulta
 */

/**
 * @def UCI_INFORMACION
 * @brief C�digo para representar un envio de informaci�n distinto de los anteriores
 */

  
/*@}*/
};

#endif


