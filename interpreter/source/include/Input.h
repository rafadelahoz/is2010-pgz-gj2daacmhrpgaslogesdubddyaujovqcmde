#pragma once
#ifndef __INPUT_H__
#define __INPUT_H__

using namespace std;

//! Provee funciones para obtener el estado del teclado, el rat�n y el gamepad.

/*!
	Se encarga de controlar las acciones realizadas en los distintos dispositivos de 
	entrada y devolver toda la informaci�n relevante para que la pueda usar el int�rprete. Ofrecer�
	m�todos para comprobar el estado de las teclas (si est�n pulsadas, puls�ndose durante 
	un tiempo o soltadas), tanto de un joystick como de un teclado.
*/

class Input
{
	friend class EngineManager;

	private:

	//! Inicializa la clase Input
	/*! Necesita que se le indique la ventana creada para la aplicaci�n
		\param window Ventana instanciada de la librer�a subyaciente
	*/
	bool init(sf::Window* window);

	public:
	
	//! Construye el sistema de input
	/*!
		Preparara las estructuras necesarias para la gesti�n de entrada de datos por el usuario, dejando
		al m�todo Input::init() la inicializaci�n de la clase y librer�a.
	*/
	Input();
	
	//! Destructora
	~Input();
	
	//!Actualiza la informaci�n de todos los elementos de entrada
	/*!
		\return true si ha actualizado todo correctamente, false si algo ha fallado
	*/
	bool checkInput();
	
	/*********************************************************************\
	*	Tratamiento de teclas por defecto								  *
	\*********************************************************************/
	
	//! Comprueba si se est� pulsando una tecla del teclado
	/*!
		\param key Tecla a comprobar.
		\return True si se est� pulsando una tecla, falso en caso contrario.
	*/
	bool key(Key key);

	//! Comprueba si se ha pulsado una tecla del teclado.
	/*!
		\param key Tecla a comprobar.
		\return True si se ha pulsado la tecla, falso en caso contrario.
	*/
	bool keyPressed(Key key);

	//! Comprueba si se ha soltado una tecla del teclado.
	/*!
		\param key Tecla a comprobar.
		\return True si se ha soltado la tecla, falso en caso contrario.

	*/
	bool keyReleased(Key key);

	/*********************************************************************\
	*	Tratamiento de teclas personalizadas							  *
	\*********************************************************************/
	
	//! Asigna un identificador personalizado a una tecla del sistema
	/*!
		\param name Nombre de la tecla personalizada.
		\param key Tecla a personalizar.
	*/
	void keySet(string name, Key key);
	
	//! Permite recuperar la tecla del sistema asociada al identificador.
	/*!
		\param name Identificador de la tecla personalizada.
		\return Tecla del sistema asignada al identificador dado.
	*/
	Key getKey(string name);
	
	//! Comprueba si se est� pulsando una tecla personalizada del teclado.
	/*!
		\param key Tecla a comprobar
		\return True si se est� pulsando la tecla, falso en caso contrario.
	*/	
	bool key(string key);

	//! Comprueba si se ha pulsado una tecla personalizada del teclado.
	/*!
		\param key Tecla a comprobar.
		\return True si se ha pulsado la tecla, falso en caso contrario.
	*/
	bool keyPressed(string key);

	//! Comprueba si se ha soltado una tecla personalizada del teclado.
	/*!
		\param key Tecla a comprobar.
		\return True si se ha soltado la tecla, falso en caso contrario.

	*/
	bool keyReleased(string key);

	/*********************************************************************\
	*	Tratamiento de input por joystick por defecto					  *
	\*********************************************************************/
	
	//!Devuelve el valor de un eje del stick en un gamePad.
	/*!
		\param joy Identificador del gamepad.
		\param axis Identificador del eje.
		\return Valor del stick.
	*/
	float joyAxis(int joy, int axis);

	//!Comprueba si se est� pulsando un bot�n determinado de un gamepad.
    /*!    
		\param joy Identificador del gamepad.
		\param button Identificador del bot�n.
		\return True si se est� pulsando el bot�n del gamepad, falso en caso contrario.
	*/
	bool joyButton(int joy, int button);
     
	//!Comprueba si se ha pulsado un bot�n determinado de un gamepad.
    /*!    
		\param joy Identificador del gamepad.
		\param button Identificador del bot�n.
		\return True si se ha pulsado el bot�n del gamepad, falso en caso contrario.
	*/
	bool joyPressed(int joy, int button);

	//!Comprueba si se ha soltado un bot�n determinado de un gamepad.
    /*!    
		\param joy Identificador del gamepad.
		\param button Identificador del bot�n.
		\return True si se ha soltado el bot�n del gamepad, falso en caso contrario.
	*/
    bool joyReleased(int joy, int button);
	
	/*********************************************************************\
	*	Tratamiento de input por joystick personalizado					  *
	\*********************************************************************/
	
	
	//! Asigna un identificador personalizado a un bot�n de un joystick del sistema
	/*!
		\param name Nombre del bot�n personalizado.
		\param button Bot�n a personalizar.
	*/
	void joySet(string name, int button);
	
	//! Permite recuperar el bot�n del joystick asociado al identificador.
	/*!
		\param name Identificador del bot�n personalizado.
		\return Bot�n del joystick asignado al identificador dado.
	*/
	int getJoy(string name);
	
	
	//!Comprueba si se est� pulsando un bot�n personalizado de un gamepad.
    /*!    
		\param joy Identificador del gamepad.
		\param button Identificador personalizado del bot�n.
		\return True si se est� pulsando el bot�n del gamepad, falso en caso contrario.
	*/
	bool joyButton(int joy, string button);
     
	//!Comprueba si se ha pulsado un bot�n personalizado de un gamepad.
    /*!    
		\param joy Identificador del gamepad.
		\param button Identificador personalizado del bot�n.
		\return True si se ha pulsado el bot�n del gamepad, falso en caso contrario.
	*/
	bool joyPressed(int joy, string button);

	//!Comprueba si se ha soltado un bot�n personalizado de un gamepad.
    /*!    
		\param joy Identificador del gamepad.
		\param button Identificador personalizado del bot�n.
		\return True si se ha soltado el bot�n del gamepad, falso en caso contrario.
	*/
    bool joyReleased(int joy, string button);
	
	
	/*********************************************************************\
	*	Dem�s teclas													  *
	\*********************************************************************/
    
	//!Coprueba si se ha pulsado el boton de cierre de la ventana.
    /*!
		\return Valor l�gico, cierto si se ha pulsado y falso si no.
	*/
    bool getFinished();

	//! Devuelve la coordenada x de la posici�n del rat�n relativa a la ventana.
    /*!
		\return Coordenada x del rat�n.
	*/
	int getMouseX();

	//!Devuelve la coordenada y de la posici�n del rat�n relativa a la ventana.
    /*!
		\return Coordenada y del rat�n.
	*/
    int getMouseY();
};

#endif