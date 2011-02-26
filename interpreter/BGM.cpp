#include "BGM.h"

/***********************************************
 *			Funciones P�blicas				   *
 ***********************************************/

Bgm::Bgm(string fname, SoundEngine* sndEngine)
{
	music = new Music(fname,sndEngine);
}
		
Bgm::~Bgm();

//! Reproduce la Bgm con el volumen indicado
/*!
	\param volume Volumen de reproducci�n (-1 para volumen por defecto)
*/
void Bgm::play(int volume = -1);

//! Finaliza la reproducci�n
void Bgm::stop();

//! Pausa la reproducci�n en curso
void Bgm::pause();

//! Reanuda la reproducci�n pausada
void Bgm::resume();

//! Informa si la Bgm est� reproduci�ndose actualmente
bool Bgm::isPlaying();

//! Indica si la reproducci�n se repite
/*!
	\param loop Activa o desactiva la reproducci�n repetida
*/
void Bgm::setLoop(bool loop);

//! Establece el volumen de reproducci�n
/*!
	\param volume Volumen de reproducci�n
*/
void Bgm::setVolume (int volume);

//! Cambia el volumen de reproducci�n vari�ndolo lentamente
/*!
	\param volume Volumen de reproducci�n
	\param time Tiempo de transici�n
*/
void Bgm::fade (int volume, int time);