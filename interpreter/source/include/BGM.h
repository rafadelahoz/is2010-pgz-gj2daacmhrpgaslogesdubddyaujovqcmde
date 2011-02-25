#pragma once
#ifndef __BGM_H__
#define __BGM_H__

using namespace std;


//! M�sica de fondo de la capa superior del subsistema de audio
/*!
	Envuelve una Music, facilitando su manejo y reproducci�n
	
	Proporciona m�todos para reproducir y controlar bgms
	de forma sencilla y eficaz.
	
	\sa Music
*/
class Bgm
{
	public:
		
		//! Construye la Bgm carg�ndola de un archivo
		/*!
			\param fname Nombre de archivo a cargar
			\param sndEngine Puntero al SoundEngine del sistema
		*/
		Bgm(string fname, SoundEngine *sndEngine);
		
		//! Destruye la Bgm, liberando el archivo cargado
		~Bgm();

		//! Reproduce la Bgm con el volumen indicado
		/*!
			\param volume Volumen de reproducci�n (-1 para volumen por defecto)
		*/
		void play(int volume = -1);

		//! Finaliza la reproducci�n
		void stop();

		//! Pausa la reproducci�n en curso
		void pause();

		//! Reanuda la reproducci�n pausada
		void resume();

		//! Informa si la Bgm est� reproduci�ndose actualmente
		bool isPlaying();

		//! Indica si la reproducci�n se repite
		/*!
			\param loop Activa o desactiva la reproducci�n repetida
		*/
		void setLoop(bool loop);

		//! Establece el volumen de reproducci�n
		/*!
			\param volume Volumen de reproducci�n
		*/
		void setVolume (int volume);

		//! Cambia el volumen de reproducci�n vari�ndolo lentamente
		/*!
			\param volume Volumen de reproducci�n
			\param time Tiempo de transici�n
		*/
		void fade (int volume, int time);
};