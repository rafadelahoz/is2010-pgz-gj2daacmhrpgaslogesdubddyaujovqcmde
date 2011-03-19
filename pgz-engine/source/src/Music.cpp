#include "Music.h"

//Construye una m�sica con la ayuda de SoundEngine
Music::Music(string path, SoundEngine* soundEngine)
{
	// Se carga la m�sica
	this->music = soundEngine->loadMusic(path);
	// Se podr�a avisar si falla la carga pero ya lo hace SoundEngine

	// Se actualizan punteros
	this->soundEngine = soundEngine;
	this->path = path;
}

//Libera la memoria de una m�sica
Music::~Music()
{
	// Se pide al SoundEngine que borre la m�sica
	soundEngine->deleteMusic(path);

	// Se limpian los punteros
	this->music = NULL;
	this->soundEngine = NULL;
}
