#pragma once
#ifndef __FRAMECONTROL_H__
#define __FRAMECONTROL_H__

//! Controla los frames por segundo que dibuja el juego.
/*! Permite imponer un l�mite a los FPS, 
	obtener la media de FPS a la que se est� ejecutando el juego
	y obtener la duraci�n del �ltimo frame.
*/
class FrameControl
{
	public:
		//! Instancia el controlador de frames.
		/*!
			\param fpsLimit L�mite de los FPS (0 para deshabilitarlo).
		*/
		FrameControl(int fpsLimit = 0);
		//! Destructora por defecto
		~FrameControl();
		
		//! Inicializa el control de FPS. Debe llamarse despu�s de la constructora.
		void init();
		
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
};

#endif