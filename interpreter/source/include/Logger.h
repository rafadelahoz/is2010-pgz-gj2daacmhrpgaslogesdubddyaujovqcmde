#pragma once

#ifndef _LOGGER_H_
#define _LOGGER_H_

using namespace std;

//!Clase encargada de escribir en archivo informaci�n relevante que pueda necesitar el programador
/*!
	Su funci�n es la de encapsular los posibles errores o trazas del programa como ayuda para
	el programador, pero puede ser usado para otros fines.
*/

class Logger {

	public:
		//! Crea flog en una ruta por defecto y lo prepara para escribir en el
		Logger();
		
		//! Crea flog en la ruta indicada y lo prepara para escribir en el
		/*!
			\param fname contiene la ruta en la que se crear� el archivo de log
		*/
		Logger(char* fname);
		
		//! Destructora, escribe un final para el archivo y lo cierra
		~Logger();

		//! Escribe el string en el archivo de log que est� abierto
		/*!
			\param s es la informaci�n que escribir�
		*/
		void log(string s);
		
		//! Devuelve la hora actual en formato de texto
		char* getTime();
		
		//! Devuelve la fecha actual en formato de texto
		char* getDate();
};

#endif // _LOGGER_H_