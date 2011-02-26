#include "BGM.h"

/***********************************************
 *			Funciones P�blicas				   *
 ***********************************************/

/* Bgm::Bgm(string fname, SoundEngine* sndEngine) */
/*
Creamos un elemento de tipo Music (que estamos encapsulando), y apuntamos al
SoundEngine que nos va a gestionar para pasarle las �rdenes
*/
Bgm::Bgm(string fname, SoundEngine* sndEngine)
{
	soundEngine = sndEngine;
	music = new Music(fname,sndEngine);

	volume= -1;
	loop = true;
	playing  = false;
	paused = false;
}

/* Bgm::~Bgm() */
/*
Eliminamos el elemento Music que creamos, al sndEngine le ponemos el puntero
a NULL pero no le hacemos el delete pues otras clases podr�an estar utiliz�ndolo
*/
Bgm::~Bgm()
{
	delete music;
	music = NULL;
}

/* Bgm::play(int volume = -1) */
/*
Modificamos el atributo volume de BGM con el valor env�ado, establecemos
los valores de playing y paused para indicar el estado de la m�sica
y reproducimos la m�sica usando SoundEngine.
*/
void Bgm::play(int volume = -1)
{
	playing = true;
	paused = false;
	this->volume = volume;
	soundEngine->playMusic(music,volume,loop);
}

/* void Bgm::stop() */
/*
Paramos la reproducci�n de este sonido utilizando la funcionalidad
de SoundEngine
*/
void Bgm::stop()
{
	paused = false;
	playing = false;
	soundEngine->stopMusic();
}

/* Bgm::pause() */
/*
Pausamos la reproducci�n de este sonido utilizando la funcionalidad
de SoundEngine
*/
void Bgm::pause()
{
	paused = true;
	soundEngine->pauseMusic();
}

/* Bgm::resume() */
/*
Reanudamos la reproducci�n de este sonido, si estaba previamente
pausado utilizando la funcionalidad de SoundEngine
*/
void Bgm::resume()
{
	if (paused)
	{
		soundEngine->resumeMusic();
		paused = false;
	}
}


bool Bgm::isPlaying()
{
	return !paused && playing;
}

void Bgm::setLoop(bool loop)
{
	this->loop = loop;
	soundEngine->setLoop(loop);
}

void Bgm::setVolume (int volume)
{
	this->volume = volume;
	soundEngine->setMusicVolume(volume);
}

void Bgm::fade (int volume, int time)
{
	soundEngine->fadeMusic(volume, time);
}