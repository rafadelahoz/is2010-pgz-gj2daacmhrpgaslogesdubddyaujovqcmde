#pragma once
#ifndef __FRAMECONTROL_H__
#define __FRAMECONTROL_H__

// Clase "Window" de SFML en la que se apoya FrameControl
#include "SFML\Window.hpp"

//! Controla los frames por segundo que dibuja el juego.
/*! Permite imponer un l�mite a los FPS, 
	obtener la media de FPS a la que se est� ejecutando el juego
	y obtener la duraci�n del �ltimo frame.
*/
class FrameControl
{
	private:
		// L�mite de fps (0 = sin l�mite)
		int fpsLimit;
		// Media de fps
		int fps;
		// Puntero a la ventana que gestiona
		sf::Window* window;

	public:
		//! Instancia el controlador de frames.
		/*!
			\param fpsLimit L�mite de los FPS (0 para deshabilitarlo).
		*/
		FrameControl(int fpsLimit = 0);
		//! Destructora por defecto
		~FrameControl();
		
		//! Inicializa el control de FPS. Debe llamarse despu�s de la constructora.
		bool init(sf::Window* window);
		
		//! Establece el l�mite de los FPS.
		/*!
			\param fpsLimit L�mite de los FPS (0 para deshabilitarlo).
		*/
		void setFPSLimit(int fpsLimit = 0);
		
		//! Obtiene el l�mite establecido para los FPS (No los FPS medios reales)
		/*!
			\return L�mite establecido para los FPS.
		*/
		int getFPSLimit();
		
		//! Obtiene la media de FPS a la que se est� ejecutando el juego
		/*
			\return Media de FPS.
		*/
		int getFramesPerSecond();
		
		//! Obtiene la duraci�n del �ltimo frame.
		/*
			Calculada a partir de la diferencia de tiempos de los dos �ltimos frames.
			\return Duraci�n del �ltimo frame.
		*/
		int getDelay();

		// Obtiene el valor de la duraci�n del frame en cada paso del juego
		// y actualiza el valor de la media de fps en consecuencia
		// Debe llamarse en cada paso del juego
		void update();
};

#endif