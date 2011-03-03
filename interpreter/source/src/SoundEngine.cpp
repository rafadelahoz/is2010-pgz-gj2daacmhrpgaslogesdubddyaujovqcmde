#include "SoundEngine.h"

//Constructora por defecto de SoundEngine
SoundEngine::SoundEngine(){
	systemSoundVolume = 50.0; //Setea el volumen global de sonidos
	systemMusicVolume = 50.0; //Setea el volumen global de m�sicas
	init();
}

//Destructora
SoundEngine::~SoundEngine(){
	delete soundManager; //Libera la memoria de soundManager
	delete musicManager; //Libera la memoria de musicManager
}

//Inicializa las estructuras necesarias
bool SoundEngine::init(){
	soundManager = new SoundManager(); //Crea un SoundManager para su posterior utilizaci�n
	musicManager = new MusicManager(); //Crea un MusicManager para su posterior utilizaci�n
	if (soundManager == NULL || musicManager == NULL ) //Comprueba si se han creado las estructuras correctamente
		return false;
	else return true;
} 


//M�todos comunes a sonidos y m�sicas
void SoundEngine::setSoundVolume(float soundVolume) { systemSoundVolume = soundVolume;} //Cambia el volumen global de sonidos
float SoundEngine::getSoundVolume() { return systemSoundVolume;}

void SoundEngine::setMusicVolume(float musicVolume) { systemMusicVolume = musicVolume;} //Cambia el volumen global de m�sica
float SoundEngine::getMusicVolume() { return systemMusicVolume;}

void SoundEngine::stopAllSounds(){;} //Por implementar
void SoundEngine::stopAllMusic(){ stopMusic();} //Detiene la m�sica de fondo que se est� reproduciendo actualmente


//M�todos relacionados con la reproducci�n de Sonidos

//Carga un sonido en memoria para su posterior reproducci�n
sf::Music* SoundEngine::loadSound(string path){

	sf::Music* sound = new sf::Music();
	if( !soundManager->isLoaded(path)){ //Comprueba si ya est� cargada en memoria
		if ( !sound->OpenFromFile(path.c_str()) ){ //Si no est� cargada intenta cargarla
			cout<<"fallo al cargar sonido"<<endl;
		}
		return sound;
	}
	else return soundManager->getSound(path)->sound; // Si est� cargada la devuelve
}

//Reproduce un sonido
void SoundEngine::playSound(Sound* sound, bool loop, float volume){

	if (volume == -1) 
		sound->sound->SetVolume(systemSoundVolume);//Si no se ha solicitado un volumen concreto se pone el del sistema
	else
		sound->sound->SetVolume(volume); //Si se ha solicitado, se cambia al solicitado

	if (sound->sound->GetLoop() != loop) //Si el loop solicitado es distinto al que ten�amos
		sound->sound->SetLoop(loop);     //Lo cambiamos

	sound->sound->Play();

}

void SoundEngine::fadeSound(Sound* sound, float volume, int time){;}//Mirar

//Devuelve si sound se est� reproduciendo actualmente
bool SoundEngine::isSoundPlaying(Sound* sound){
	if (sound!=NULL && sound->sound->GetStatus() == sf::Music::Playing ) //Si existe el sonido y est� reproduci�ndose, se devuelve true
		return true;
	else return false;
}

//Detiene la reproducci�n de sound
void SoundEngine::stopSound(Sound* sound){
	if (isSoundPlaying(sound)) //Si el sonido est� reproduci�ndose, se detiene.
		sound->sound->Stop();
}

//M�todos relacionados con la reproducci�n de M�sica

//Carga en memoria la m�sica que haya en path
sf::Music* SoundEngine::loadMusic(string path){
	sf::Music* music = new sf::Music();
	if( !musicManager->isLoaded(path)){//Comprueba si el sonido est� cargado actualmente en memoria
		if( !music->OpenFromFile(path.c_str()) ){ //Si no est� cargado, se carga
			//Error
		}
		return music;
	}
	else return musicManager->getMusic(path)->music; //Si est� cargado, se devuelve
}

//Reproduce una m�sica de fondo, controlando que solo haya una reproduci�ndose al mismo tiempo
void SoundEngine::playMusic(Music* music, bool loop, float volume){
	
	if (actPlayingMusic != NULL && isMusicPlaying(actPlayingMusic)) //Si hay alguna m�sica de fondo reproduci�ndose, se para
		stopMusic();

	if (volume == -1)
		music->music->SetVolume(systemMusicVolume); //Si no se solicita un volumen espec�fico, se reproduce con el volumen del sistema
	else
		music->music->SetVolume(volume); // Si se solicita un volumen espec�fico, se cambia


	if (music->music->GetLoop() != loop) //Si el loop solicitado es distinto
		music->music->SetLoop(loop);     //Se cambia

	music->music->Play();

	actPlayingMusic = music;
}

//Finaliza la reproducci�n de una m�sica de fondo con atenuaci�n
void SoundEngine::fadeMusic(float volume, int time){
	/*if (actPlayingMusic != NULL && isMusicPlaying(actPlayingMusic)){
		int actVolume = (int)  actPlayingMusic->music->GetVolume();

		int diferencia = actVolume - (int)volume;
		if ( diferencia > 0){
			int delay = (time*1000/45);

			for (int i = (int)volume; i<actVolume; i++){
				actPlayingMusic->music->SetVolume(actVolume - i);
				Sleep(delay);
			}
		}
		stopMusic();
	}*/;
} 

//Nos dice si music est� reproduci�ndose actualmente
bool SoundEngine::isMusicPlaying(Music* music){
	if (music != NULL && music->music->GetStatus() == sf::Music::Playing ) //Si la m�sica existe y est� reproduciendose, se devuelve true
		return true;
	else return false;
}

//Pausa la m�sica de fondo
void SoundEngine::pauseMusic(){
	if (isMusicPlaying(actPlayingMusic)) //Si la m�sica est� reproduci�ndose, se pausa
		actPlayingMusic->music->Pause();
}

//Devuelve si la m�sica de fondo est� pausada
bool SoundEngine::isMusicPaused(Music* music){
	if (music != NULL && music->music->GetStatus() == sf::Music::Paused) //Si la m�sica existe y est� pausada, se devuelve true
		return true;
	else return false;
}

//Se reanuda la reproducci�n de la m�sica de fondo pausada
void SoundEngine::resumeMusic(){
	if (isMusicPaused(actPlayingMusic)) //Si la m�sica est� pausada, de reanuda su reproducci�n
		actPlayingMusic->music->Play();
}


//Se para la m�sica de fondo que se est� reproduciendo actualmente
void SoundEngine::stopMusic(){
	if ( isMusicPlaying(actPlayingMusic) || isMusicPaused(actPlayingMusic) ) //Si la m�sica est� reproduci�ndose o pausada, se para
		actPlayingMusic->music->Stop();
}

//Si alguna m�sica de fondo est� reproduci�ndose o pausada cambiamos su loop
void SoundEngine::setLoop(bool loop){
	if ( isMusicPlaying(actPlayingMusic) || isMusicPaused(actPlayingMusic) ) //Se cambia el loop de la m�sica, 
		actPlayingMusic->music->SetLoop(loop);					             //siempre y cuando est� reproduci�ndose o pausada
		

}





