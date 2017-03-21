/***************************************************************************
                          driverBomba.cpp  -  description
                             -------------------
    begin                : Thu Mar 17 2005
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

//cabecera de la clase
#include "driverBomba.h"

namespace uciEquiposCabecera {
namespace uciBomba{

//------------------- añadido por Susana -----------------------------------
extern "C" driverBomba *cargar(QString sPuerto,QString sTipo){
	return new driverBomba(sPuerto,sTipo);
}
//---------------- fin añadido por Susana -----------------------------------


/**
 * crc_get()
 * @brief Recibe una cadena y su longitud y devuelve el CRC de dicha cadena
 * en forma de 4 caracteres ASCII que deben ser interpretados como un número
 * hexadecimal.
 * @author V. Le, Adaptado por Leonardo Santamarina Gzyl
 * @date Marzo 2005
 * @version 1.0
 * @note
 * @warning Antes de usar esta función, la variable "crc_tab" debe estar
 * inicializada mediante la llamada a la función i_crc()
 */

//driverBomba::driverBomba()
//{

//}

/**
 * driverBomba(iDes,pcSalida,pTerm,pcPuerto)
 * @brief Constructor general de la clase.
 *        E:/ datos_terminal pTerm: Modos de control de la comunicación
 *            char pcPuerto: Puerto de comunicación serie (/dev/ttyS*)
 *        S:/ objeto de la clase driverBomba
 * @author Susana Martín Toral
 * @date 28/01/05
 * @version 1.0
 * @note
 * @warning
 */

// Constructor general
driverBomba::driverBomba(QString sParamPuerto, QString sParamTipo)
:driverGenerico(sParamTipo)
{


  i_crc();
  sprintf(ID_hard,"%s%c","@0",'\0');
  sprintf(ID_soft,"%s%c","@0",'\0');

  //cambiado por susana
  //for(int i=0;i<12;i++) pcPuerto[i]='\0';
  //sprintf(pcPuerto,"%s",puerto);
  sPuerto=sParamPuerto;
  //fin cambiado por susana

  pTerm.uiVelocidad=VELOCIDAD;
  pTerm.iParidad=PARIDAD;
  pTerm.iBit_datos=BIT_DATOS;
  pTerm.iBit_start=BIT_START;
  pTerm.iBit_stop=BIT_STOP;
  pTerm.iTmuestreo=TMUESTREO;
  pTerm.iTimeOutC=TOUT_C;
  pTerm.iTimeOutM=TOUT_M;


}

/**
 * Probar(): Hace uso de las funciones Configurar y Comunicar para detectar
 * la presencia de la bomba en el puerto.
 * Recibe: void
 * Devuelve: UCI_OK si está presente la bomba, UCI_ERROR de lo contrario.
 * @brief Detecta la presencia de la bomba en el puerto
 * @author Leonardo Santamarina G.
 * @date Marzo 2005
 * @version 1.0
 * @note
 * @warning
 */
int driverBomba::Probar(void){
  int presente=UCI_ERROR;
  if(Configurar()==UCI_OK){
    if(Comunicar(UCI_BOMBA_CHEQ_AL,0)==UCI_OK){
      presente=UCI_OK;
      }
      //Modificado por JoseLuis
      //if(Comunicar(UCI_BOMBA_IDENT,0)==UCI_OK){
      //presente=UCI_OK;
      //}
      //Fin modificado por Jose Luis
      cout << "Saco por pantalla el ALR" << endl;
      cout << ALR << endl;
      
    }
  return presente;
  }

/**
 * Configurar(): Abre y configura el puerto serie
 * Recibe: void
 * Devuelve: UCI_OK si se abre y configura el puerto serie con éxito, UCI_ERROR de lo contrario.
 * @brief Abre y configura el puerto serie
 * @author Leonardo Santamarina
 * @date Marzo 2005
 * @version 1.0
 * @note
 * @warning
 */
int driverBomba::Configurar (void)
{
  int estado=UCI_OK;
  //ComunicacionPuertoSerie p(pTerm,pcPuerto,BUFSIZE);
  //printf("\n DriverBomba: el puerto a abrir es: %s",pcPuerto);
  cout<<"\n DriverBomba: el puerto a abrir es: "<<sPuerto<<endl;

  ComunicacionPuertoSerie p(pTerm,sPuerto,BUFSIZE,O_RDWR | O_NOCTTY);

  cout<<"\nDriverBomba: Abriendo y configurando el puerto serie...";

  if((p.configurarPuerto())==-1){
   estado=UCI_ERROR;
   cout<<"DriverBomba: ERROR en la apertura y configuracion del puerto\n";
  }

  else{
    printf("OK\n");
    strcpy(p.nombre,"p");
    puerto=p;
    strcpy(puerto.nombre,"puerto");
  }

 return estado;
}

//---------------------------------------------------------------------------
//Set Things
//---------------------------------------------------------------------------
void driverBomba::SetRespuesta(char *r){strcpy(respuesta,r);}

//---------------------------------------------------------------------------
//Get Things
//---------------------------------------------------------------------------
char *driverBomba::GetPaquete(void){return paquete;}
char *driverBomba::GetRespuesta(void){return respuesta;}

//---------------------------------------------------------------------------
//Destructor de la clase
//---------------------------------------------------------------------------

driverBomba::~driverBomba()
{

}



//---------------------------------------------------------------------------
//Funciones Internas
//---------------------------------------------------------------------------


/**
 *
 * @brief
 * @author Leonardo Santamaria
 * @date Julio 2005
 * @version 1.0
 * @note Cambios realizados a partir del 4 de Julio por Jose Luis Rodríguez 
 * @warning
 */



/**
 * crc_get: Calcula el crc de una cadena y lo devuelve en forma de cadena (no el valor numérico)
 * Es útil para concatenar el CRC a una cadena preexixtente.
 * Recibe: una cadena y la longitud de la cadena en forma de entero
 * Devuelve: Aputador a la cadena
 * @brief Calcula el crc de una cadena.
 * @author Leonardo Santamarina
 * @date Marzo 2005
 * @version 1.0
 * @note
 * @warning
 */
char * driverBomba::crc_get(char *packet, int len)
//char *packet;
//int len;
{
    int i=0;
    int data;
    unsigned short crccitt;
    static char crc_buf[UCI_BOMBA_CRC_LEN + 1];
    crccitt=0;
    //printf("packet[i]=%d, crccitt=%u, data=%d \n",(int)packet[i],crccitt,data);
    for (i=0;i<len;i++)
    {
        data=crccitt^(int)packet[i];
        //printf("crccitt=%u xor packet[i]=%d = data=%d, %d %d\n",crccitt,(int)packet[i],data,(data & 0x00ff),(int)crc_tab[data & 0x00ff]);
        crccitt=(crccitt>>8)^crc_tab[data & 0x00ff];


    }
    sprintf(crc_buf,"%4x",crccitt);

    for (i=0;crc_buf[i]!='\0';i++)
    {
        crc_buf[i] = (char)toupper((int)crc_buf[i]);
        if(crc_buf[i]==UCI_BOMBA_BLANK)
                crc_buf[i]='0';
        //printf("crc_buf[i]=%c \n",crc_buf[i]);
    }
    return (crc_buf);
}

/**
 * crchware()
 * @brief Calcula el CRC de un byte.
 * @author Vu Le, Adaptado por Leonardo Santamarina.
 * @date Marzo 2005
 * @version 1.0
 * @note Función auxiliar utilizada por la funciín i_crc
 * @warning
 */
int driverBomba::crchware(unsigned short data,unsigned short genpoly,unsigned short accum)
{
    int i;
    int lsb_diff;

    for (i=0;i<8;i++)
    {
        lsb_diff=((data^accum)&0x0001);
        if(lsb_diff)
                accum=(accum>>1)^genpoly;
        else
                accum >>= 1;
        data >>= 1;
    }
    return (accum);
}

/**
 * i_crc()
 * @brief Inicializa la variable crc_tab con los valores CRC de cada
 * byte de datos (0 a 255).
 * @author Vu Le, Adaptado por Leonardo Santmarina Gzyl

 * @date Marzo 2005
 * @version 1.0
 * @note Debe ser llamada antes de crc_get
 **/
void driverBomba::i_crc ()
{
    int i;
    for (i=0;i<UCI_BOMBA_MAX_ASCII;i++)
    {
        crc_tab[i]=crchware(i,UCI_BOMBA_CCCITT_REV,0);
    }
}


/**
 * codificar: Genera el paquete a ser transmitido de acuerdo con los parámetros de entrada
 * Recibe: comando y parámetro del comando en forma de enteros sin signo.
 * Devuelve: UCI_OK si tiene exito, UCI_ERROR de lo contrario.
 * @author Leonardo Santamarina
 * @date Marzo 2005
 * @version 1.0
 * @note Añadido el Historial de alarmas
 * @warning
 */

int driverBomba::codificar (unsigned short int comando, unsigned short int param)
  {
    int estado=UCI_OK;

    switch (comando)
    {
      case UCI_BOMBA_IDENT: //Pregunta identificación de la bomba y versión de software
        sprintf(paquete,"T%s;IDTY;SWV;",ID_soft); // Se construye parte del paquete:
                                                  //Dirección y comando

        sprintf(paquete,"%s%s%c",paquete,crc_get(paquete,strlen(paquete)),13); // Se copia el CRC
                                                            // y el <CR>

      break;

      case UCI_BOMBA_SET_ID: // Cambia la dirección Soft (IDsoft)
        sprintf(paquete,"T%s;SSID %u;",ID_soft,param);// Se construye parte del paquete:
                                                      //Dirección y comando

        sprintf(paquete,"%s%s%c",paquete,crc_get(paquete,strlen(paquete)),13); // Se copia el CRC
                                                               // y el <CR>
      break;


      case UCI_BOMBA_GET_ID: //Obtiene la dirección Soft (debe usarse ID_hard)
        sprintf(paquete,"T%s;ISID;",ID_hard);// Se construye parte del paquete: *
                                                      //Dirección y comando
        sprintf(paquete,"%s%s%c",paquete,crc_get(paquete,strlen(paquete)),13); // Se copia el CRC
                                                            // y el <CR>
      break;

      case UCI_BOMBA_CHEQ_AL: // Pregunta si hay una alarma y la razón
        sprintf(paquete,"T%s;IALR;",ID_soft);// Se construye parte del paquete:
                                                      //Dirección y comando
        sprintf(paquete,"%s%s%c",paquete,crc_get(paquete,strlen(paquete)),13); // Se copia el CRC
                                                            // y el <CR>
      break;
      //Añadido por jose Luis
	case UCI_BOMBA_CHEQ_ALH: // Pregunta por el historial de alarmas
	
        sprintf(paquete,"T%s;IALH;",ID_soft);// Se construye parte del paquete:
                                                      //Dirección y comando
        sprintf(paquete,"%s%s%c",paquete,crc_get(paquete,strlen(paquete)),13); // Se copia el CRC
                                                            // y el <CR>
      break;
      // fin añadido por jose Luis
      case UCI_BOMBA_CHEQ_STA: // Pregunta el estado de la bomba
        sprintf(paquete,"T%s;ISTA;",ID_soft);// Se construye parte del paquete:
                                                      //Dirección y comando
        sprintf(paquete,"%s%s%c",paquete,crc_get(paquete,strlen(paquete)),13); // Se copia el CRC
                                                            // y el <CR>
      break;

      case UCI_BOMBA_LECT_PARAM: // Lee valores de la bomba dependiendo de 'param'
        switch (param)
        {
          case 1: // Presión distal y su límite
            sprintf(paquete,"T%s;IDP1;DPL;",ID_soft);// Se construye parte del paquete:
                                                      //Dirección y comando
            sprintf(paquete,"%s%s%c",paquete,crc_get(paquete,strlen(paquete)),13); // Se copia el CRC
                                                            // y el <CR>
          break;

          case 2: // 'Delivery rate' y volumen restante en la linea 1 y volumen total
            sprintf(paquete,"T%s;IDV1;VT1;VOL;",ID_soft);// Se construye parte del paquete:
                                                      //Dirección y comando
            sprintf(paquete,"%s%s%c",paquete,crc_get(paquete,strlen(paquete)),13); // Se copia el CRC
                                                            // y el <CR>
          break;

          case 3: //'Delivery rate' y volumen restante en la linea 1, horas de batería
            sprintf(paquete,"T%s;IDV2;VT2;HBT;",ID_soft);// Se construye parte del paquete:
                                                      //Dirección y comando
            sprintf(paquete,"%s%s%c",paquete,crc_get(paquete,strlen(paquete)),13); // Se copia el CRC
                                                            // y el <CR>
          break;
          default:  // Se llega al final y no se consigue un parámetro válido
            printf("\nERROR: Parámetro no valido\n");
            estado=UCI_ERROR;
          break;
        }
      break;

      default:           // Se llega al final y no se consigue un comando válido
         printf("\nERROR:Argumento no válido\n");
         estado=UCI_ERROR;
      break;

    }
    puerto.SetEscritura(paquete);
    return estado;
  }

/**
 * decodificar()
 * @brief Una vez recibida la respuesta de la bomba, esta función la analiza y
 * guarda los datos en las variables respectivas
 * Recibe: comando y parámetro del último paquete que se envió
 * (debo conocerlo porque la bomba no hace eco en su respuesta)
 * Devuelve: UCI_OK o UCI_ERROR dependiendo del caso
 * @author Leonardo Santamarina
 * @date Marzo 2005
 * @version 1.0
 * @note
 * @warning
 */
int driverBomba::decodificar (int comando, unsigned short int param)
{
  int estado=UCI_OK;
  char * pch, * pch1, pch2[UCI_BOMBA_MAX_ASCII];
  //char crc_env[3], crc_obt[3];
  char version[UCI_BOMBA_MAX_ASCII], resto[UCI_BOMBA_MAX_ASCII];
  //char hard[3], soft[5];

  alarma=FALSE; //Apago la condición de alarma antes de procesar la respuesta,
                //si hay una alarma en la respuesta se volverá a activar.

  strcpy(respuesta,puerto.GetLectura()); // Copio lo que se leyó en "respuesta" para guardarlo
  strcpy(resto,respuesta); // Copio lo que se leyó en "resto", que se irá modificando

      switch (comando)
      {
        case UCI_BOMBA_IDENT: //Se envió petición de Identificar la bomba
          if((CRC_check(resto))>0) // No hay error de CRC
          {
            pch=strchr(resto,'"'); // Busco el primer ' " '
            pch1=pch;
            pch1++;
            pch1=strchr(pch1,'"'); // Busco el siguiente ' " '
            strncpy(ID_Bomba,pch,pch1-pch+1);
            strncpy((ID_Bomba)+(pch1-pch)+1,"\0",1);
            pch1=pch1+3;
            pch=strrchr(pch1,'"');
            pch--;
            strncpy(version,pch1,(pch-pch1)+1);
            strncpy(version+(pch-pch1)+1,"\0",1);
            sw=atoi(version);
            printf("ID bomba: %s. Versión de software: %d.\n", ID_Bomba,sw);
          }
          else
          {
            printf("Error de CRC o formato de entrada inválido\n");
            estado=UCI_ERROR;
          }
        break;


        case UCI_BOMBA_SET_ID: // Se envió "cambiar la dirección Soft" (debe usarse ID_hard al comunicarse con la bomba)
          pch=strrchr(paquete,';');
          pch1=strrchr(respuesta,';');
          if((strncmp(pch,pch1,4))==0)
          {
            printf("CRC concuerda\n");
            pch=strchr(resto,';');
            pch++;
            pch=strchr(pch,';');
            pch++;
            if (((*pch)=='A')||((*pch)=='a'))
              printf ("Acknowledged\n");
            if (ALR_check(pch))
            {
              // Hay alarmas que leer en la bomba
              alarma=TRUE;
            }

          }
          else
          {
            printf("Error de CRC\n"); // Error en el CRC reportado
            estado=UCI_ERROR;
          }
        break;

        case UCI_BOMBA_GET_ID: //Se envió "obtener la dirección Soft" (debe usarse ID_hard)
          if((CRC_check(resto))>0) // No hay error de CRC
          {
            pch=strchr(resto,';'); // Busco el primer ';'
            pch++;
            pch1=strchr(pch,';'); // Busco el segundo ';'
            pch1++;
            if ((*pch1=='R')||(*pch1=='r')) // Respuesta ok
            {
              pch1++;
              pch=strrchr(pch1,';');
              strncpy(ID_soft,pch1,pch-pch1);
              printf("ID soft: %s\n",ID_soft);
            }
            else
            {
              printf ("Error: La respuesta no posee el formato adecuado\n");
              estado=UCI_ERROR;
            }
          }
          else
          {
            printf("Error de CRC o formato de entrada inválido\n");
            estado=UCI_ERROR;
          }
        break;


        case UCI_BOMBA_CHEQ_AL: //Se envió "Pregunta si hay una alarma y la razón"
          if((CRC_check(resto))>0) // No hay error de CRC
          {
                pch=strchr(resto,';');
                pch++;
                pch=strchr(pch,';');
                pch++;
                if (ALR_check(pch))
                {
                  // Hay alarmas que leer en la bomba
                  alarma=TRUE;
                }
                pch++;
                strcpy(ALR,"\0");
                while(*pch!='\0')
                {
                  printf ("%s\n",pch);
                  parse(pch,ALR);
                  pch=strchr(pch,';');
                  pch++;
                }
                printf ("%s\n",ALR);
          }
          else
          {
            printf("Error de CRC o formato de entrada inválido\n");
            estado=UCI_ERROR;
          }
        break;
	// inicio añadido por Jose Luis
	case UCI_BOMBA_CHEQ_ALH: //Se envió "Pregunta por el historial de las alarmas"
          
	  
	  if((CRC_check(resto))>0) // No hay error de CRC
          {
                
		
		if (ALH_check( pch))
  		{// Hay alarmas que leer en la bomba
                  alarma=TRUE;
  		cout << "La bomba esta en condicion de alarma !! " << endl;
  		}
		
		pch=strchr(resto,' ');
		pch++;
		// aqui ya tengo la cadena que queria
		strcpy(ALH,"\0");
		while(*pch!='\0')
                {
		 //  cout << "El valor de pch es : " << pch<< endl;
		  parsehistorial(pch,ALH);
                  pch=strchr(pch,' ');
                  pch++;
                }
		cout << "El archivo historico de las 15 ultimas alarmas : " <<endl;
                cout << ALH << endl;
                //printf ("%s\n",ALH);
          }
          else
          {
            printf("Error de CRC o formato de entrada inválido\n");
            estado=UCI_ERROR;
          }
        break;
	// fin añadido por JoseLuis
        case UCI_BOMBA_CHEQ_STA: //Se envió "Pregunta el estado de la bomba"
          if((CRC_check(resto))>0) // No hay error de CRC
          {
                pch=strchr(resto,';');
                pch++;
                pch=strchr(pch,';');
                pch++;
                if (ALR_check(pch))
                {
                  // Hay alarmas que leer en la bomba
                  alarma=TRUE;
                }
                pch++;
                pch1=strchr(pch,';');
                strncpy(pch2,pch,pch1-pch);
                pch2[(pch1-pch)]='\0';

                if ((strcmp(pch2,"BPR"))==0)
                    sprintf(STA,"Backpriming");
                else if ((strcmp(pch2,"MFN"))==0)
                    sprintf(STA,"Fallo");
                else if ((strcmp(pch2,"PKV"))==0)
                    sprintf(STA,"Bombeando Modo KVO");
                else if ((strcmp(pch2,"PPR"))==0)
                    sprintf(STA,"Bombeando Primaria");
                else if ((strcmp(pch2,"PSE"))==0)
                    sprintf(STA,"Bombeando Secundaria");
                else if ((strcmp(pch2,"SDO"))==0)
                    sprintf(STA,"Detenida, Puerta Abierta");
                else if ((strcmp(pch2,"STP"))==0)
                    sprintf(STA,"Detenida");
                else {
                   // Error en en formato de la respuesta
                   estado=UCI_ERROR;
                }
                printf("%s\n",STA);
          }
          else
          {
            printf("Error de CRC o formato de entrada inválido\n");
            estado=UCI_ERROR;
          }
        break;

        case UCI_BOMBA_LECT_PARAM: // Lee valores de la bomba dependiendo de 'param'
          if((CRC_check(resto))>0)
          {
            switch (param)
            {
              case dp1_dpL: // Presión distal y su límite

                pch=strchr(resto,';');
                pch++;
                pch=strchr(pch,';');
                pch++;
                if (ALR_check(pch))
                {
                  // Hay alarmas que leer en la bomba
                  alarma=TRUE;
                }
                pch++;
                pch1=strchr(pch,';');
                strncpy(pch2,pch,pch1-pch);
                pch2[(pch1-pch)]='\0';
                dp1=atof(pch2);
                pch1++;
                pch=strchr(pch1,';');
                strncpy(pch2,pch1,pch-pch1);
                pch2[(pch-pch1)]='\0';
                dpL=atof(pch2);
                printf("DP1: %f DPL:%f\n",dp1,dpL);


              break;

              case dv1_vt1_vol: // 'Delivery rate' y volumen restante en la linea 1 y volumen total
                pch=strchr(resto,';');
                pch++;
                pch=strchr(pch,';');
                pch++;

                if (ALR_check(pch))
                {
                  // Hay alarmas que leer en la bomba
                  alarma=TRUE;
                }
                pch++;
                pch1=strchr(pch,';');

                strncpy(pch2,pch,pch1-pch);
                pch2[(pch1-pch)]='\0';
                dv1=atof(pch2);
                pch1++;

                pch=strchr(pch1,';');
                strncpy(pch2,pch1,pch-pch1);
                pch2[(pch-pch1)]='\0';
                vt1=atof(pch2);
                pch++;
                pch1=strchr(pch,';');
                strncpy(pch2,pch,pch1-pch);
                pch2[(pch1-pch)]='\0';
                vol=atof(pch2);
                printf("DV1: %f VT1:%f VOL:%f\n",dv1,vt1,vol);
              break;

              case dv2_vt2_hbt: //'Delivery rate' y volumen restante en la linea 1, horas de batería
                pch=strchr(resto,';');
                pch++;
                pch=strchr(pch,';');
                pch++;

                if (ALR_check(pch))
                {
                  // Hay alarmas que leer en la bomba
                  alarma=TRUE;
                }
                pch++;
                pch1=strchr(pch,';');

                strncpy(pch2,pch,pch1-pch);
                pch2[(pch1-pch)]='\0';
                dv2=atof(pch2);
                pch1++;

                pch=strchr(pch1,';');
                strncpy(pch2,pch1,pch-pch1);
                pch2[(pch-pch1)]='\0';
                vt2=atof(pch2);
                pch++;
                pch1=strchr(pch,';');
                strncpy(pch2,pch,pch1-pch);
                pch2[(pch1-pch)]='\0';
                hbt=atof(pch2);
                printf("DV2: %f VT2:%f HBT:%f\n",dv2,vt2,hbt);
              break;
            }
          }
          else
          {
            printf("Error de CRC o formato de entrada inválido\n");
            estado=UCI_ERROR;
          }

        break;
      }
   return estado;
}


/**
 * CRC_check()
 * @brief Manipula la cadena de entrada (respuesta de la bomba), verificando su CRC y quitandolo
 * Recibe: Una cadena (char *)
 * Devuelve: -1 si el string de entrada no posee un formato adecuado
 *            0 si el string de entrada tiene error de CRC
 *            1 si el CRC es correcto
 * de la cadena para mayor comodidad
 * @author Leonardo Santamarina
 * @date Febrero 2005
 * @version 1.0
 * @note
 * @warning Esta función modifica la cadena de entrada, se recomienda pasarle
 * siempre una copia de la cadena original a la que se desea verificar en CRC
 */
int driverBomba::CRC_check(char * r)
{
  char * pch;
  //char * pch1;
  char crc_env[4], crc_obt[4], resto[UCI_BOMBA_MAX_ASCII];
  //char aux[4];
  int resultado;
  long env, obt;
  //cout << "trama de entrada en CRC check" << endl;

  pch=strrchr(r,';'); // Encuentra el último ';' Lo siguiente es el CRC
  if (pch==NULL) // No se encontró el caracter ';'
  {
    printf("ERROR: El string de entrada no posee un formato adecuado en CRC_CHEQ\n");
    resultado = -1;
    return resultado;
  }
  pch++;
  strncpy(crc_env,pch,4); //
  strncpy(crc_env+4,"\0",1); //
  sscanf(crc_env,"%4X",&env);
  
  //cout << "El crc enviado "<< crc_env << endl;
  //cout << "El crc enviado "<< &env << endl;
  //cout << "El crc enviado "<< env << endl;
   
  int iLaOtra=env;
  
  strncpy(resto,r,pch-r);
  strncpy(resto+(pch-r),"\0",1);
  
  //cout << "Lo que mando a crc_get " << resto << endl;
  
  strcpy(crc_obt,crc_get(resto,strlen(resto)));
  sscanf(crc_obt,"%4X",&obt);
  //cout << "El crc obtenido "<< crc_obt << endl;
  //cout << "El crc obtenido "<< &obt << endl;
  //cout << "El crc obtenido "<< obt << endl;
  
  
  //cout << "Comparacion de los CRC " << endl;
  //cout << "CRC enviado por la bomba " << env << endl;
  //cout << "CRC enviado por la bomba " << iLaOtra << endl;
  //cout << "CRC obtenido por la bomba " << obt << endl;
  
  
  if(iLaOtra==obt) // Si el CRC concuerda -> Transmisión correcta
  {
    resultado=1;
    printf("MATCH!\n");
  }
  else
  {
    printf("ERROR DE CRC el crc no concuerda\n");
    resultado=0;

  }
  strcpy(r,resto);
  return resultado;
}


/**
 * ID_h_check. Verifica si la dirección hard que viene en una respuesta concuerda con la bomba
 * ocn la cual me estoy comunicando.
 * Recibe: Cadena (respuesta de la bomba)
 * Devuelve: TRUE o FALSE dependiendo del caso
 * @author Leonardo Santamarina Gzyl
 * @date Marzo 2005
 * @version 1.0
 * @note Función auxiliar
 * @warning
 */
bool driverBomba::ID_h_check (char *s)
{
  char * pch, * pch1;
  //char hard[3];
  bool resultado;

  pch=strchr(s,';'); // Busco el primer ' ; '
  pch++;
  pch1=strchr(pch,';'); // Busco el segundo ' ; '
  //strncpy(hard,pch,pch1-pch);
  if((strncmp(ID_hard,pch,pch1-pch))==0)
  {
    printf ("ID Hard Check!\n");
    resultado=TRUE;
  }
  else
  {
    resultado=FALSE;
  }

  return resultado;

}


/**
 * ID_s_check. Verifica si la dirección soft que viene en una respuesta concuerda con la bomba
 * ocn la cual me estoy comunicando.
 * Recibe: Cadena (respuesta de la bomba)
 * Devuelve: TRUE o FALSE dependiendo del caso
 * @author Leonardo Santamarina Gzyl
 * @date Marzo 2005
 * @version 1.0
 * @note Función auxiliar
 * @warning
 */
bool driverBomba::ID_s_check (char *s)
{
  char * pch, * pch1;
  //char soft[3];
  bool resultado;

  pch=strchr(s,';'); // Busco el primer ' ; '
  pch++;
  pch1=strchr(pch,';'); // Busco el segundo ' ; '
  pch1++;
  pch=strchr(pch1,';'); // Busco el tercer ' ; '
  //strncpy(hard,pch,pch1-pch);
  if((strncmp(ID_soft,pch1,pch-pch1))==0)
  {
    printf ("ID soft Check!\n");
    resultado=TRUE;
  }
  else
  {
    resultado=FALSE;
  }

  return resultado;
}

/**
 * ALR_check: Analiza la respuesta de la bomba y determina si hay o no condición de alarma
 * Recibe: cadena (respuesta de la bomba)
 * @author Leonardo Santamarina
 * @date Marzo 2005
 * @version 1.0
 * @note
 * @warning
 */
bool driverBomba::ALR_check ( char *s)
{
  bool resultado;
  if (islower(*s))
  {
    printf("Existe condición de alarma en la bomba\n");
    resultado = TRUE;
  }
  else
  {
    printf("No hay alarmas\n");
    resultado = FALSE;
  }
  return resultado;
}
// inicio añadido por JoseLuis
/**
 * ALH_check: Analiza la respuesta de la bomba y determina si hay o no condición de alarma
 * Recibe: cadena (respuesta de la bomba)
 * @author Jose Luis Rodriguez
 * @date Julio 2005
 * @version 1.0
 * @note
 * @warning
 */
bool driverBomba::ALH_check ( char *s)
{
  bool resultado;
  if (islower(*s))
  {
    printf("Existe condición de alarma en la bomba.\n");
    resultado = TRUE;
  }
  else
  {
    printf("No hay condicion de alarma en la bomba.\n");
    resultado = FALSE;
  }
  return resultado;
}
// Fin añadido por Jose Luis
/**
* parse: Extiende el texto resumido de alarma entregado por la bomba para hacerlo más comprensible.
* Recibe: char *p: cadena resumida.
*         char *s: aquí quedará la cadena extendida
* Devuelve: void
* @author Leonardo Santamarina Gzyl
* @date Marzo 2005
* @version 1.0
* @note
* @warning
*/
void driverBomba::parse (char *p, char *s)
{

  if ((strncmp(p,"ADB",3))==0)
  {
    strcat(s,"Aire en linea, Distal, Proximo. ");
  }
  if ((strncmp(p,"ADC",3))==0)
  {
    strcat(s,"Aire en linea, Distal, Acumulativo. ");
  }
  if ((strncmp(p,"AP1",3))==0)
  {
    strcat(s,"Aire en linea, Proximo. ");
  }
  if ((strncmp(p,"BLO",3))==0)
  {
    strcat(s,"Bateria baja, bomba operando. ");
  }
  if ((strncmp(p,"BLS",3))==0)
  {
    strcat(s,"Bateria baja, boma detenida. ");
  }
  if ((strncmp(p,"CHK",3))==0)
  {
    strcat(s,"Verificar conf. Rate o VTBI no configurados. ");
  }
  if ((strncmp(p,"CS1",3))==0)
  {
    strcat(s,"Fallo en cassette. ");
  }
  if ((strncmp(p,"DCO1",4))==0)
  {
    strcat(s,"Puerta o cassette abierto mientras se bombea. ");
  }
  if ((strncmp(p,"DTL",3))==0)
  {
    strcat(s,"Control dial turned when lockout switch is on. ");
  }
  if ((strncmp(p,"MAL",3))==0)
  {
    strcat(s,"Fallo del tipo indicado por el número en pantalla. ");
  }
  if ((strncmp(p,"OBP",3))==0)
  {
    strcat(s,"Oclusion, Backpriming. ");
  }

  if ((strncmp(p,"OCP",3))==0)
  {
    strcat(s,"Oclusion proxima. ");
  }
  if ((strncmp(p,"OK",3))==0)
  {
    strcat(s,"No hay alarmas. ");
  }
  if ((strncmp(p,"RL",3))==0)
  {
    strcat(s,"En reset por mas de 5 min. ");
  }
  if ((strncmp(p,"VTB",3))==0)
  {
    strcat(s,"Programa de bombeo completo. ");
  }

}
// <end> Añadido por Leonardo

// Inicio Añadido por Jose Luis

/**
* parsehistorial: Extiende el texto resumido de alarma entregado por la bomba para hacerlo más comprensible.
* Recibe: char *p: cadena resumida.
*         char *s: aquí quedará la cadena extendida
* Devuelve: void
* @author Jose Luis Rodriguez
* @date Julio 2005
* @version 1.0
* @note
* @warning
*/
void driverBomba::parsehistorial (char *p, char *s)
{

  if ((strncmp(p,"01",2))==0)
  {
    strcat(s,"Obtruccion en linea Distal. ");
  }
    if ((strncmp(p,"03",2))==0)
  {
    strcat(s,"Obtruccion en linea Proximal. ");
  }
   if ((strncmp(p,"06",2))==0)
  {
    strcat(s,"Aire en la linea, Sensor Distal. ");
  }
   if ((strncmp(p,"07",2))==0)
  {
    strcat(s,"Aire en la linea, Sensor Proximal. ");
  }
   if ((strncmp(p,"08",2))==0)
  {
    strcat(s,"Aire en la linea, Backpriming. ");
  }
   if ((strncmp(p,"11",2))==0)
  {
    strcat(s,"DE vuelta para ejecucion. El control rotatorio no esta en la posición OFF,CHANGE o RUN, o no ha sido presionada ninguna teclas desde hace 5 minutos. ");
  }
   if ((strncmp(p,"12",2))==0)
  {
    strcat(s,"Completo VTBI. ");
  }
   if ((strncmp(p,"13",2))==0)
  {
    strcat(s,"Fallo chequeo cassette. ");
  }
   if ((strncmp(p,"14",2))==0)
  {
    strcat(s,"Bloqueado.Dial de control movido cuando el switch de bloqueo esta encendido. ");
  }
  
   if ((strncmp(p,"16",2))==0)
  {
    strcat(s,"De vuelta para ejecucion. El dial de control rotatorio entre estados validos por 5 segundos. ");
  }
   if ((strncmp(p,"17",2))==0)
  {
    strcat(s,"Bateria Baja, Funcionando. ");
  }
   if ((strncmp(p,"18",2))==0)
  {
    strcat(s,"Bateria Baja, Detenido. ");
  }
   if ((strncmp(p,"19",2))==0)
  {
    strcat(s,"Puerta Abierta mientras bombeaba. ");
  }
   if ((strncmp(p,"20",2))==0)
  {
    strcat(s,"Stack runaway error. ");
  }
   if ((strncmp(p,"29",2))==0)
  {
    strcat(s,"Error de suma de chequeo ROM. ");
  }
   if ((strncmp(p,"34",2))==0)
  {
    strcat(s,"Fallo de test de lectura/escritura de EEPROM. ");
  }
   if ((strncmp(p,"35",2))==0)
  {
    strcat(s,"Encontrados incorrectos valores criticos de RAM. ");
  }
   if ((strncmp(p,"44",2))==0)
  {
    strcat(s,"Fallo Alarma Audible. ");
  }
   if ((strncmp(p,"45",2))==0)
  {
    strcat(s,"Switch del teclado cerrado mas de 2 minutos 40 segundos. ");
  }
   if ((strncmp(p,"59",2))==0)
  {
    strcat(s,"Mas de un motor moviendose al mismo tiempo. ");
  }
   if ((strncmp(p,"60",2))==0)
  {
    strcat(s,"Motor Plunger no en posicion HOME. ");
  }
   if ((strncmp(p,"61",2))==0)
  {
    strcat(s,"Motor de la valvula de E/S no en posicion HOME. ");
  }
   if ((strncmp(p,"62",2))==0)
  {
    strcat(s,"Motor Piggyback no en posicion HOME. ");
  }
   if ((strncmp(p,"63",2))==0)
  {
    strcat(s,"Motor Plunger deslizando o atascado. ");
  }
   if ((strncmp(p,"64",2))==0)
  {
    strcat(s,"Motor de la valvula de E/S deslizando o atascada. ");
  }
   if ((strncmp(p,"65",2))==0)
  {
    strcat(s,"Motor Piggyback deslizando o atascado. ");
  }
   if ((strncmp(p,"71",2))==0)
  {
    strcat(s,"Software no ejecutado en un periodo de 10 ms. ");
  }
   if ((strncmp(p,"73",2))==0)
  {
    strcat(s,"Fallo en conversor A/D. ");
  }
   if ((strncmp(p,"74",2))==0)
  {
    strcat(s,"Fallo del sensor de aire. ");
  }
   if ((strncmp(p,"81",2))==0)
  {
    strcat(s,"Fallo de la fuente de alimentacion PWA. ");
  }
   if ((strncmp(p,"90",2))==0)
  {
    strcat(s,"Fallo de suma de chequeo para calibracion de datos en EEPROM. ");
  }
  
   if ((strncmp(p,"94",2))==0)
  {
    strcat(s,"Fallo del control rotatorio. ");
  }
   if ((strncmp(p,"95",2))==0)
  {
    strcat(s,"Pin defect circuitry failure. ");
  }
  
   if ((strncmp(p,"99",2))==0)
  {
    strcat(s,"Fallo en la autocomprobacion del software. ");
  }
  // Añadidos ppor Leo aparte de los que vienen en el manual
   if ((strncmp(p,"00",2))==0)
  {
    strcat(s,"Alarma en la conexion del detector de flujo. ");
  }
   if ((strncmp(p,"04",2))==0)
  {
    strcat(s,"Obtruccion en linea Secundaria Proximal. ");
  }
  
   if ((strncmp(p,"12",2))==0)
  {
    strcat(s,"Dosis Finalizada. ");
  }
  
  
 }
  
 //Fin añadido por Jose Luis


/**
 * Comunicar: Función de alto nivel que se llamar a las funciones 'codificar', 'decodificar'
 * 'Leer', 'Escribir' para llevar a cabo correctamente la comunicación con la bomba.
 * Recibe los mismos comandos que la función "codificar" (ver arriba)
 * Devuelve: UCI_OK o UCI_ERROR dependiendo del caso.
 * @brief
 * @author Leonardo Santamarina
 * @date Marzo 2005
 * @version 1.0
 * @note
 * @warning
 */

int driverBomba::Comunicar(int comando, unsigned short int param)
{
  int estado=UCI_OK;

  if /*If1*/(codificar(comando,param)==UCI_ERROR){  // Parámetros incorrectos, abortar.
    printf("\nERROR (codificar): Parámetros inválidos de entrada, abortando.\n");
    estado=UCI_ERROR;

  }
  else/*ELSE 1*/ {  // Codificación exitosa. Continuar
    if/*If2*/(Escribir()==UCI_ERROR){    // Si falla la escritura nos salimos de la función.
      estado=UCI_ERROR;
    }
    else/*ELSE 2*/{ //Escritura llevada a cabo correctamente. Procedo a leer.
      if/*If3*/(Leer()==UCI_ERROR){    // Error de lectura.
                                       //Limpiar bufer de la boma e intentar de nuevo.
        puerto.SetEscritura("\x03");  //Copio el caracter de vaciado en el buffer de salida
        if/*If4*/(Escribir()==UCI_ERROR){    // Se intenta escribir.
          estado=UCI_ERROR;
        }
        else/*ELSE 4*/ {  // Se escribió correctamente el caracter de vaciado.
                // La bomba limpió su bufer de recepción
          puerto.SetEscritura(paquete); // Copio en bufer de salida el comando anterior
          if/*If5*/(Escribir()==UCI_ERROR){    // Se intenta escribir.
            estado=UCI_ERROR;
          }
          else/*ELSE 5*/ {    // Se reenvió correctamente el paquete
            if/*If6*/(Leer()==UCI_ERROR){    // Se intenta leer. Si falla esta vez, me rindo.
              estado=UCI_ERROR;
            }
            else/*ELSE 6*/{   // Se lee correctamente el paquete reenviado
              if/*If7*/(decodificar(comando,param)==UCI_ERROR){ // Error de CRC/respuesta desconocida

                puerto.SetEscritura("\x03");  //Copio el caracter de vaciado en el buffer de salida
                if/*If7-1*/(Escribir()==UCI_ERROR){    // Se intenta escribir.
                  estado=UCI_ERROR;
                }
                else/*ELSE 7-1*/ {  // Se escribió correctamente el caracter de vaciado.
                    // La bomba limpió su bufer de recepción
                  puerto.SetEscritura(paquete); // Copio en bufer de salida el comando anterior
                  if(Escribir()==UCI_ERROR){    // Se intenta escribir.
                    estado=UCI_ERROR;
                  }
                  else/*ELSE 7-2*/ {    // Se reenvió correctamente el paquete
                    if(Leer()==UCI_ERROR){    // Se intenta leer nuevamente. Si falla esta vez, me rindo
                      estado=UCI_ERROR;
                    }
                    else/*ELSE 7-3*/{   // Se lee correctamente el paquete reenviado
                      if(decodificar(comando,param)==UCI_ERROR){ // Error de CRC/respuesta desconocida
                      // Me rindo después de 2 intentos.
                      estado=UCI_ERROR;
                      }
                      else/*ELSE 7-4*/{ //Comando decodificado correctamente
                        printf("\nComunicación exitosa\n");
                      }/*FIN ELSE 7-4*/
                    }/*FIN ELSE 7-3*/
                  }/*FIN ELSE 7-2*/
                }/*FIN ELSE 7-1*/
              }/*FIN IF 7*/

              else/*ELSE 7*/{ //Comando decodificado correctamente
                printf("\nComunicación exitosa\n");
              }/*FIN ELSE 7*/
            }/*FIN ELSE 6*/
          }/*FIN ELSE 5*/
        } /*FIN ELSE 4*/
      }/*FIN IF 3*/

      else/*ELSE 3*/{              //Lectura llevada a cabo correctamente, procedo a decodificar el dato
        if/*If8*/(decodificar(comando,param)==UCI_ERROR){ // Error de CRC o respuesta desconocida.
          puerto.SetEscritura("\x03");  //Copio el caracter de vaciado en el buffer de salida
          if/*If9*/(Escribir()==UCI_ERROR){    // Se intenta escribir.
            estado=UCI_ERROR;
          }
          else/*ELSE 9*/ {  // Se escribió correctamente el caracter de vaciado.
                  // La bomba limpió su bufer de recepción
            puerto.SetEscritura(paquete); // Copio en bufer de salida el comando anterior
            if/*If10*/(Escribir()==UCI_ERROR){    // Se intenta escribir.
              estado=UCI_ERROR;
            }
            else/*ELSE 10*/ {    // Se reenvió correctamente el paquete
              if/*If11*/(Leer()==UCI_ERROR){    // Se intenta leer nuevamente. Si falla esta vez, me rindo
                estado=UCI_ERROR;
              }
              else/*ELSE 11*/{   // Se lee correctamente el paquete reenviado
                if/*If12*/(decodificar(comando,param)==UCI_ERROR){ // Error de CRC/respuesta desconocida
                // Me rindo después de 2 intentos.
                estado=UCI_ERROR;
                }
                else/*ELSE 12*/{ //Comando decodificado correctamente
                  printf("\nComunicación exitosa\n");
                }/*FIN ELSE 12*/
              }/*FIN ELSE 11*/
            }/*FIN ELSE 10*/
          }/*FIN ELSE 9*/
        }/*FIN IF 8*/
        else/*ELSE 8*/{ printf("\nComunicación exitosa\n"); //Comando decodificado correctamente
        }/*FIN ELSE 8*/
      }/*FIN ELSE 3*/

    }/*FIN ELSE 2*/


  }/*FIN ELSE 1*/

  return estado;
}

/**
 * Escribir. Envía por el puerto lo que se haya estipulado con SetLectura().
 * Recibe: void
 * Devuelve: UCI_OK o UCI_ERROR dependiendo del caso.
 * @brief
 * @author Leonardo Santamarina
 * @date Marzo 2005
 * @version 1.0
 * @note intenta enviar 2 veces, al segundo error, aborta y devuelve UCI_ERROR
 * @warning
 */

int driverBomba::Escribir(void){
 int estado=UCI_OK;

  printf("\nEscribiendo datos el puerto serie... %s\n",puerto.GetEscritura());
  if(puerto.escribirPuerto()==UCI_ERROR){ //Fallo al intentar escribir al puerto
    printf("\nERROR de escritura en el puerto, intentando de nuevo\n");
    usleep(500000); // Espero 500 ms antes de intentar de nuevo
    printf("\nEscribiendo datos el puerto serie... %s\n",puerto.GetEscritura());
    if(puerto.escribirPuerto()==UCI_ERROR){ // Fallo al escribir al puerto por 2da vez
      printf("ERROR de escritura en el puerto\n");
      estado=UCI_ERROR;      // Fallo al escribir al 2do intento, abortar.
    }
    else printf("OK\n"); // Se escribieron los datos correctamente en el 2do intento
  }
  else printf("OK\n"); // Se escribieron los datos correctamente a la primera

 return estado;

}

/**
 * Leer. Lee la cadena recibida por el puerto.
 * Recibe: void
 * Devuelve: UCI_OK o UCI_ERROR dependiendo del caso.
 * @brief
 * @author Leonardo Santamarina
 * @date Marzo 2005
 * @version 1.0
 * @note intenta leer 2 veces, al segundo error, aborta y devuelve UCI_ERROR
 * @warning
 */

int driverBomba::Leer(void)
{
int estado=UCI_OK;

  printf("\nLeyendo datos el puerto serie...\n ");
  if(puerto.leerPuerto()==UCI_ERROR){ //Fallo al intentar leer del puerto
    printf("\nERROR de lectura en el puerto, intentando de nuevo\n");
    usleep(500000); // Espero 500 ms antes de intentar de nuevo
    printf("\nLeyendo datos el puerto serie... \n");
    if(puerto.leerPuerto()==UCI_ERROR){ // Fallo al escribir al puerto por 2da vez
      printf("ERROR de lectura en el puerto\n");
      estado=UCI_ERROR;      // Fallo al escribir al 2do intento, abortar.
    }
    else printf("OK\n"); // Se leyeron los datos correctamente en el 2do intento
  }
  else printf("OK\n"); // Se leyeron los datos correctamente a la primera

 return estado;

}

char * driverBomba::GetAlarmas(void)
{
  return ALR;
}

//--------------------------------------------------------------------------------------------

//añadido por Susana
/**
 * Identificar()
 * @brief Funcion que devuelve una identificacion completa del dispositvo
 *        S:/ estructura con los valores de identificacion
 * @author Susana Martín Toral
 * @date 25/05/05
 * @version 1.0
 * @note
 * @warning
 */
IdDispositivo driverBomba::Identificar(void){
	IdDispositivo pIdent;
		
	// Llamar a la funcion comunicar
	
	
	if(Comunicar(UCI_BOMBA_IDENT,0)==UCI_OK){
      //Busco los datos actuales
      }
      // Alguna funcion superior tendria que decir que es una bomba de infusion
	pIdent.sTipoEquipo="Bomba de infusion";
	//preguntar por el modelo: comunicar()
	//pIdent.sModelo="Plum XLD Fix";
	pIdent.sModelo = ID_Bomba;
	
	
	//preguntar por el fabricante: comunicar() , la bomba no responde
	// con el nombre del fabricante
	pIdent.sFabricante="Abbot";
		
	pIdent.iIdDispositivo=-1;

	return pIdent;

}

//--------------------------------------------------------------------------------------------

/**
 * FinalizarComunicacion()
 * @brief Funcion que realiza todas las acciones necesarias antes de terminar la comunicacion
 *        con el equipo de cabecera en cuestion. Entre otras cosas esta funcion debera
 *        cerrar el puerto de comunicaciones, ya que no se hace en ningun otro punto
 *        S:/ estructura con los valores de identificacion
 * @author Susana Martín Toral
 * @date 17/06/05
 * @version 1.0
 * @note
 * @warning
 */
int driverBomba::FinalizarComunicacion(void){

	//Cerrar el puerto de comunicaciones que se haya estado usando
	int iEstado=puerto.cerrarPuerto();
	
	return iEstado;

}

//--------------------------------------------------------------------------------------------

/**
 * ObtenerAlarmas()
 * @brief Funcion que indica si en el equipo de cabecera se ha generado alguna alarma y los
 *        valores para dicha alarma
 *        S:/ Vector de estructuras vital? con la información sobre la/s alarma/s producida/s
 *            En el caso de que no existan alarmas dicha vector apuntara a NULL
 * @author Susana Martín Toral
 * @date 20/06/05
 * @version 1.0
 * @note
 * @warning
 */
std::vector<Alarma> *driverBomba::ObtenerAlarmas(void){
	//En esta función se deberán rellenar los campos de una estructura del tipo Alarma en el 
	//caso de que se detecte una condición de alarma en la bomba
	//std::vector<Alarma> vVectorAlarmas;
	Alarma pAlarma;
	QString sTextoAlarma;
	
	//Vaciamos el vector de alarmas para rellenarlo con las nuevas alarmas si las hay
	vVectorAlarmas.clear();
	
	//Preguntar a la bomba si tiene nuevas alarmas /**

	while(HayAlarmas()==true){
		sTextoAlarma=sTextoAlarma.setLatin1(GetAlarmas());
		//Generamos una estructura del tipo Alarma y la vamos rellenando
		
		pAlarma.sTipoEquipo="Bomba";
		//preguntar por el modelo: comunicar()
		if(Comunicar(UCI_BOMBA_IDENT,0)==UCI_OK){
      		//presente=UCI_OK;
      		}
		
		//pAlarma.sModelo="Plum XLD";
		pAlarma.sModelo=ID_Bomba;
		
		//Tratar la alarma recibida para determinar de qué tipo es y para
		//determinar de qué prioridad es
		//Simulamos que es una alarma técnica producida por la bomba
		pAlarma.iTipoAlarma=1;
		pAlarma.sDescripcion=sTextoAlarma;
		pAlarma.sDescripcion=ALR;
		//Simulamos que es una alarma de prioridad baja
		pAlarma.iPrioridad=0;
		
		//Tenemos que obtener la fecha y hora actuales porque la bomba,
		//al parecer, no es capaz de darte las alarmas con su fecha y hora de produccion
		QStringList listaFechaHora=obtenerFechaHoraEquipo();
		QStringList::Iterator it=listaFechaHora.begin();
		QString sFecha=(*it).latin1();
		++it;
		QString sHora=(*it).latin1();
		
		pAlarma.sFecha=sFecha;
		pAlarma.sHora=sHora;
		
		vVectorAlarmas.push_back(pAlarma);
	}
	
	return &vVectorAlarmas;

}

//--------------------------------------------------------------------------------------------

/**
 * HayAlarmas()
 * @brief Funcion indicada por Leo en el .h de la clase, pero sin implementacion en el .cpp,
 *        cuyo objetivo es el de indicar si existe en el equipo o no condicion de alarma
 *        S:/ booleano indicando si hay alarma (true) o no (false)
 * @author Susana Martín Toral
 * @date 20/06/05
 * @version 1.0
 * @note
 * @warning
 */
bool driverBomba::HayAlarmas(void){
	
	// si esto no funciona habria que construir un mensaje preguntando 
	// a la bomba mediante un mensaje especifico y la funcion comunicar
	if(Comunicar(UCI_BOMBA_CHEQ_AL,0)==UCI_OK){
      // Con esto simplememente tengo un valor actualizado para saber si hay
      // una alarma en la bomba
      }
	//Simplemente devuelve el valor del dato alarma perteneciente a la clase
	return (this->alarma);
}



/**
 * InicializarVITAL(void):   
 * @brief Función que inicializar la estructura VITAL VMD para una bomba de 
 *        infusion de Abbott
 * @author Susana Martín
 * @date 29/08/05
 * @version 1.0
 * @note 
 * @warning Avisos importantes (puede estar en blanco)
 */
void driverBomba::InicializarVITAL(TraductorVital traductor)
{
	//Orden que es necesario seguir para rellenar la estructura: Metricas,Numerics,Canales,VMD
	//Generacion de metricas:------------------------------
	//DP1
	pDP1=traductor.traducirMetrica(2,0,1,"DP1",26764,0,1,2,0,0,0,"PSI");
	//DPL
	pDPL=traductor.traducirMetrica(2,0,2,"DPL",0,0,1,2,0,0,0,"PSI");
	//DV1
	pDV1=traductor.traducirMetrica(2,0,3,"DV1",26724,0,1,2,0,0,3122,"mL_Hour");
	//DV2
	pDV2=traductor.traducirMetrica(2,0,4,"DV2",26724,0,1,2,0,0,3122,"mL_Hour");
	//VOL
	pVOL=traductor.traducirMetrica(2,0,5,"VOL",26816,0,1,2,0,0,1618,"mL");
	//VT1
	pVT1=traductor.traducirMetrica(2,0,6,"VT1",26812,0,1,2,0,0,1618,"mL");
	//VT2
	pVT2=traductor.traducirMetrica(2,0,7,"VT2",26812,0,1,2,0,0,1618,"mL");
	//HBT
	pHBT=traductor.traducirMetrica(2,0,8,"HBT",0,0,1,2,0,0,2240,"h");
	//HTT
	pHTT=traductor.traducirMetrica(2,0,9,"HTT",0,0,1,2,0,0,2240,"h");
	
	//Generacion de numerics
		//No tenemos hasta que no los obtengamos del dispositivo
	
	//Generacion de canales:------------------------------
	//Channel source
	pCSource=traductor.traducirCanal(1,4450,1,"Canal source 1",0,1,1,16);
	//Channel delivery1
	pCDelivery1=traductor.traducirCanal(1,4451,2,"Canal delivery 1",0,1,1,16);
	//Channel delivery2
	pCDelivery2=traductor.traducirCanal(1,4451,3,"Canal delivery 2",0,1,1,16);
	
	//Generacion del VMD:------------------------------
	//Channel source
	pVmd=traductor.traducirVMD(1,4450,1,"Bomba de infusion",1,"","Abbott");	
	
}


/**
 * VMD ObtenerDatos(void):   
 * @brief Función que recopila de la bomba los valores adecuados para ser
 *        
 *        E:/ float *: Array donde almacenar los datos
 *        S:/ char * : Cadena con la identificacion de la bomba (modelo) ID
 * @author Jose Luis
 * @date 15/07/05
 * @version 1.0
 * @note Reutilizada a partir de ObtenerValoresGraficas de Susana Martin
 * @warning Avisos importantes (puede estar en blanco)
 */      
VMD *driverBomba::ObtenerDatos(TraductorVital traductor)
{
	Numeric pNumeric;
	int iResultado;
	QStringList listaFechaHora=obtenerFechaHoraEquipo();
	QStringList::Iterator it=listaFechaHora.begin();
	QString sFecha=(*it).latin1();
	++it;
	QString sHora=(*it).latin1();
	
	
	//Cambiar el estado de la bomba a "Encendido"
	traductor.setStatusVMD(&pVmd,"Encendido");
	
	//Cambiar el estado de los canales!!!!
	
	
// Tengo que llamar a la funcion comunicar en sus distintas variedades
	//-------------------------------------------------------------------------- 
	//MODELO DE LA BOMBA
	codificar(UCI_BOMBA_IDENT,0); 		// Llena "paquete" con el comando a enviar
	
	puerto.SetEscritura(GetPaquete());	//almacena en la clase el paquete a enviar
	
	if((puerto.escribirPuerto())==-1)
	printf("ERROR en la escritura en el puerto\n");
	
	if((puerto.leerPuerto())==-1)
	printf("ERROR en la lectura del puerto\n");
	
	decodificar(UCI_BOMBA_IDENT,0);
	
	//Conocemos el modelo de la bomba en ID_Bomba
	traductor.setModeloVMD(&pVmd,ID_Bomba);
	
	//--------------------------------------------------------------------------
	//ESTADO DE LA BOMBA
	codificar(UCI_BOMBA_CHEQ_STA,0); 	// Llena "paquete" con el comando a enviar
	
	puerto.SetEscritura(GetPaquete());      //almacena en la clase el paquete a enviar
	
	if((puerto.escribirPuerto())==-1)
	printf("ERROR en la escritura en el puerto\n");
	
	if((puerto.leerPuerto())==-1)
	printf("ERROR en la lectura del puerto\n");
	
	decodificar(UCI_BOMBA_CHEQ_STA,0);
	
	//Conocemos el estado en el que se encuentra la bomba, que se almacena en el atributo operational
	//stalus de los canales de la bomba
	traductor.setOEChannel(&pCSource,STA);
	traductor.setOEChannel(&pCDelivery1,STA);
	traductor.setOEChannel(&pCDelivery2,STA);
	
		
	//----------------------------------------------------------------------------
	//PRESION DISTRAL Y LIMITE DE PRESION DISTRAL
	codificar(UCI_BOMBA_LECT_PARAM,1);
	
	puerto.SetEscritura(GetPaquete());         //almacena en la clase el paquete a enviar
	
	if((puerto.escribirPuerto())==-1)
	printf("ERROR en la escritura en el puerto\n");
	
	if((puerto.leerPuerto())==-1)
	printf("ERROR en la lectura del puerto\n");
	
	decodificar(UCI_BOMBA_LECT_PARAM,1);
	
	fMedidas[0]=dp1;
	fMedidas[1]=dpL;
	
	//Conocemos DP1 y DPL
	//DP1
	pNumeric=traductor.traducirNumeric(6,0,0,"medida",pDP1,dp1,sFecha,sHora,0);
	iResultado=traductor.addNumeric(&pCDelivery1,pNumeric);
	iResultado=traductor.addNumeric(&pCDelivery2,pNumeric);
	//DPL
	pNumeric=traductor.traducirNumeric(6,0,0,"medida",pDPL,dpL,sFecha,sHora,0);
	iResultado=traductor.addNumeric(&pCSource,pNumeric);
	
	//----------------------------------------------------------------------------  
	//TASA DE ENTREGA, VOLUNMEN A SER INFUNDIDO EN EL CANAL 1 Y VOLUMEN TOTAL
	codificar(UCI_BOMBA_LECT_PARAM,2);
	
	puerto.SetEscritura(GetPaquete());         //almacena en la clase el paquete a enviar
	
	if((puerto.escribirPuerto())==-1)
	printf("ERROR en la escritura en el puerto\n");
	
	if((puerto.leerPuerto())==-1)
	printf("ERROR en la lectura del puerto\n");
	
	decodificar(UCI_BOMBA_LECT_PARAM,2);
	
	fMedidas[2]=dv1;
	fMedidas[3]=vt1;
	fMedidas[4]=vol;
	
	//Conocemos DV1,VT1 y VOL 
	//DV1
	pNumeric=traductor.traducirNumeric(6,0,0,"medida",pDV1,dv1,sFecha,sHora,0);
	iResultado=traductor.addNumeric(&pCDelivery1,pNumeric);
	//DPL
	pNumeric=traductor.traducirNumeric(6,0,0,"medida",pVT1,vt1,sFecha,sHora,0);
	iResultado=traductor.addNumeric(&pCSource,pNumeric);
	//VOL
	pNumeric=traductor.traducirNumeric(6,0,0,"medida",pVOL,vol,sFecha,sHora,0);
	iResultado=traductor.addNumeric(&pCDelivery1,pNumeric);
	iResultado=traductor.addNumeric(&pCDelivery2,pNumeric);
	
	//----------------------------------------------------------------------------    
	//TASA DE ENTREGA Y VOLUMEN A SER INFUNDIDO EN EL CANAL 2
	codificar(UCI_BOMBA_LECT_PARAM,3);
	
	puerto.SetEscritura(GetPaquete());         //almacena en la clase el paquete a enviar
	
	if((puerto.escribirPuerto())==-1)
	printf("ERROR en la escritura en el puerto\n");
	
	if((puerto.leerPuerto())==-1)
	printf("ERROR en la lectura del puerto\n");
	
	decodificar(UCI_BOMBA_LECT_PARAM,3);
	
	fMedidas[5]=dv2;  
	fMedidas[6]=vt2;
	fMedidas[7]=hbt;
	
	//Conocemos DV2,VT2 
	//DV2
	pNumeric=traductor.traducirNumeric(6,0,0,"medida",pDV2,dv2,sFecha,sHora,0);
	iResultado=traductor.addNumeric(&pCDelivery2,pNumeric);
	//VT2
	pNumeric=traductor.traducirNumeric(6,0,0,"medida",pVT2,vt2,sFecha,sHora,0);
	iResultado=traductor.addNumeric(&pCSource,pNumeric);
	
	//----------------------------------------------------------------------------    
	//HORAS DE LA BATERIA
	//Conocemos HBT
	//HBT
	pNumeric=traductor.traducirNumeric(6,0,0,"medida",pHBT,hbt,sFecha,sHora,0);
	iResultado=traductor.addNumeric(&pCDelivery1,pNumeric);
	iResultado=traductor.addNumeric(&pCDelivery2,pNumeric);
	
	//----------------------------------------------------------------------------    
	//Añadir los canales al VMD
	iResultado=traductor.addChannel(&pVmd,pCSource);
	iResultado=traductor.addChannel(&pVmd,pCDelivery1);
	iResultado=traductor.addChannel(&pVmd,pCDelivery2);
	
	
	printf("\n\nValores de los datos de las bombas:\n");
	printf("Identificador de la bomba: %s\n",ID_Bomba);
	printf("El estado de la Bomba es  :%s\n",STA);
	printf("Presión distral: %f PSI\n Límite de la presión distral: %f PSI\n",fMedidas[0],fMedidas[1]);
	printf("Datos canal 1:\n");
	printf("   Frecuencia de administración: %f ml/h\n   Volumen programado: %f ml\n",fMedidas[2],fMedidas[3]);
	printf("Datos canal 2:\n");
	printf("   Frecuencia de administración: %f ml/h\n   Volumen programado: %f ml\n",fMedidas[5],fMedidas[6]);
	printf("Volumen total proporcionado hasta el momento: %f ml\n",fMedidas[4]);
	printf("Autonomía de la batería: %f horas",fMedidas[7]);
	//return "Tus Datos no estan disponibles";

	return &pVmd;
}

// Parentesis de los namespaces !
};
};

