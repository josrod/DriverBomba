/**
 * @file main.cpp
 * @brief En la funci�n main se ejecuta el c�digo necesario para establecer una 
 *        comunicaci�n a trav�s del puerto serie con la bomba Plum XLD, siendo
 *        capaz de recolectar e interpretar los datos proporcionados por �sta.
 *        Esta funci�n se comporta, por tanto, como driver del dispositivo.      
 *        E:/ int atgc     : n�mero de argumentos que se pasan a la funci�n
 *            char *argv[] : argumentos que se pasan a la funci�n
 *        S:/ int          : entero que indica el estado al finalizar la funci�n
 *                           (-1 -> estado de error, o -> estado sin error)
 * @author Susana Mart�n Toral
 * @date 03/02/05
 * @version 1.0
 * @note --> C�digo por completar, quiz�s sea necesario meter se�ales de 
 *           notificaci�n, en funci�n de la implementaci�n del resto del 
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>

using namespace uciEquiposCabecera;
using namespace uciBomba;

int main(int argc, char *argv[])
{
  
  //char *vEntrada;                      //Respuesta que se recibe de la bomba
  //char vSalida2[]="T0;IALR;4260";       //Preguntar por las alarmas
  //char vSalida[]="T0;IDTY;8BC8";       //Preguntar por la identificacion        
  //char pcPuerto[]="/dev/ttyS1";
  //char pcPuerto[10];
   
  if(argc != 2){
     printf("\nUSO: ./driverbomba /dev/ttyS*\n\n");
     return UCI_ERROR;
  }  
  
  QString sPuerto=argv[1];
  QString sTipo="bomba de infusion";
  
  TraductorVital traductor;	//Objeto encargado de realizar la traduccion VITAL de los valores obtenidos
				//de los equipos de cabecera
  
  
  cout << "Inicializamos la bomba" << endl;
  driverBomba bomba(sPuerto,sTipo);
  
  cout << "Inicializamos VITAL" << endl;
  bomba.InicializarVITAL(traductor);
  
  cout << "Identificamos el dispositivo" << endl;
  
  
  
    
  cout << "Probamos la bomba" << endl;
  int resultado=bomba.Probar();
 // cout << ALR << endl;
  cout<<"-------------------------------------------------------------------"<<endl;
  if(resultado==UCI_ERROR){
  	cout<<"No ha sido posible identificar al dispositivo conectado"<<endl;
  }
  else
  	cout<<"Se ha identificado al dispositivo conectado "<<endl;

  cout<<"-------------------------------------------------------------------"<<endl;
  
 // IdDispositivo myDispositivo;
  //myDispositivo= bomba.Identificar();
//cout << "El modelo del dispositivo es :" << endl;
 // cout << myDispositivo.sModelo << endl;
  
  
  
   
  // A esta funcion es a la que llamamos para que nos de los datos 
	VMD *pVmd;			//Estructura que almacena el formato VITAL de los datos
					//de los equipos de cabecera
	Channel pCanal;			//Estructura que almacena los datos VITAL de un canal
	Numeric pNumeric;		//Estructura que almacena los datos VITAL de un numeric
	int iTamanoC;			//Numero de canales disponibles
	int iTamanoN;			//Numero de mediadas disponibles
	int iIdDispositivo=5;
  	cout << "\nAccediendo a la estructura VITAL de VMD ... \n" << endl;
  
	//Obtenemos todos los datos del equipo de cabecera
	pVmd=bomba.ObtenerDatos(traductor);
	//Indicamos el valor del identificador de VMD
	pVmd->iHandle=iIdDispositivo;
				
				
	//((***))Ahora es el momento de tratar los valores obtenidos e insertarlos en la MDIB.
	//En concreto hay que insertar los valores Numerics obtenidos y actualizar el status del
	//VMD si hace falta. Para ello ir obteniendo cada Numeric obtenido.
	if((iTamanoC=pVmd->canales.size())==0){
		cout<<"Hilo Equipo: ERROR: No existen canales para este VMD"<<endl;
	}
	else{
		cout<<"Hilo Equipo: Este VMD contiene "<<iTamanoC<<" canales"<<endl;
		cout<<"-------------------------------------------------------------------"<<endl;
		for(int i=0;i<iTamanoC;i++){
			pCanal=pVmd->canales.operator[](i);
			cout<<"Hilo Equipo: CANAL "<<i<<" - "<<pCanal.sLabel<<" :"<<endl;
			if((iTamanoN=pCanal.medidas.size())==0){
				cout<<"Hilo Equipo: ERROR: No existen numerics para este canal"<<endl;
			}
			else{
				cout<<"Hilo Equipo: Este canal contiene "<<iTamanoN<<" numerics"<<endl;
				for(int j=0;j<iTamanoN;j++){
					pNumeric=pCanal.medidas.operator[](j);
					//((***))Insertar en la BD el Numeric correspondiente
					cout<<"--->Hilo Equipo: MEDIDA "<<j<<":"<<endl;
					cout<<"\tParticion: "<<pNumeric.type.iPartition<<endl;
					cout<<"\tCodigo: "<<pNumeric.type.iCode<<endl;
					cout<<"\tIdentificador: "<<pNumeric.iHandle<<endl;
					cout<<"\tEtiqueta: "<<pNumeric.sLabel<<endl;
					cout<<"\tMetrica medida: "<<pNumeric.ms.sLabel<<endl;
					cout<<"\tValor: "<<pNumeric.fValue<<endl;
					cout<<"\tUnidades: "<<pNumeric.ms.sUnit_string<<endl;
					cout<<"\tFecha: "<<pNumeric.sFecha<<endl;
					cout<<"\tHora: "<<pNumeric.sHora<<endl;
					cout<<"-------------------------------------------------------------"<<endl;
				}
			}
		}
	}
	
	//cout << jose->iParticion<< endl;
	//cout << jose->iCodigo<< endl;
	//cout << jose->iId<< endl;
	//cout << jose->sEtiqueta<< endl;
	//cout << jose->siEstado<< endl; SUSANA 
					// !!!!! OJO en traducirVMD aparece diEstado
					// pero en la estructura de cabecera aparecce como siStatus
	//cout << jose->siStatus<< endl;
	//cout << jose->sModelo<< endl;
	//cout << jose->sFabricante<< endl;
  
  
  //antes de abandonar el programa, o cuando se quiera cerrar el puerto serie, se debe
  //reconfigurar= y cerrar el puerto
  cout << "Cerrando el puerto de comunicaci�n..." << endl;
  //if(bomba.cerrarPuerto()==-1)
  bomba.FinalizarComunicacion();
     //printf("ERROR al cerrar el puerto serie\n");
  cout<<"OK"<<endl;
  
  
  cout<<"\n\n\n\n\t\t\t FIN DEL PROGRAMA\n\n\n\n";
  return UCI_OK;
  
}
