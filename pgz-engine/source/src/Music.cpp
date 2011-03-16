#include "Music.h"

//Construye una m�sica con la ayuda de SoundEngine
Music::Music(string path, SoundEngine* soundEngine){
	this->music = soundEngine->loadMusic(path);
	this->soundEngine = soundEngine;
	this->path = path;
}

//Libera la memoria de una m�sica
Music::~Music(){
	soundEngine->deleteMusic(path);
	//ya se encarga el soundEngine:
	//delete this->music;
	this->music = NULL;
	this->soundEngine = NULL;
}
