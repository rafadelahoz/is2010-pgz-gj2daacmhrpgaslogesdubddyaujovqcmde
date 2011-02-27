#pragma once
#ifndef __SFX_H__
#define __SFX_H__

#include <string>
#include "SoundEngine.h"
#include "Sound.h"

using namespace std;


//! Efecto de sonido de la capa superior del subsistema de audio
/*!
	Envuelve un Sound, facilitando su manejo y reproducci�n
	
	Proporciona m�todos para reproducir y controlar sonidos
	de forma sencilla y eficaz.
	
	\sa Sound
*/
class Sfx
{
	private:
		//! Atributo que encapsula a Sound
		Sound* sound;
		SoundEngine* soundEngine;
		int volume;
		bool playing;
		bool paused;
		bool loop;

	public:
		
		//! Construye el sonido, carg�ndolo de un archivo
		/*!
			\param fname Archivo de audio a cargar
			\param soundEngine Puntero al SoundEngine del sistema
	    */
		Sfx(string fname, SoundEngine* soundEngine);
		
		//! Destruye el sonido, liberando el archivo cargado
		~Sfx();

		//! Reproduce un efecto de sonido con el volumen indicado
		/*!
			\param volume Volumen de la reproducci�n
		*/
		void play(int volume = -1);

		//! Finaliza la reproducci�n del efecto de sonido
		void stop();
		
		//! Cambia el volumen de reproducci�n vari�ndolo lentamente
		/*!
			\param volume Volumen de reproducci�n
			\param time Tiempo de transici�n
		*/
		void fade (int volume, int time);
		
		//! Informa si el sonido se esta reproduciendo.
		bool isPlaying();

		//! Establece el volumen de reproducci�n
		/*!
			\param volume Volumen de reproducci�n
		*/
		void setVolume (int volume);
};

#endif