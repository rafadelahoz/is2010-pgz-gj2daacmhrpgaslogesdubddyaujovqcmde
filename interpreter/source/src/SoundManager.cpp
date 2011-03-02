#include "SoundManager.h"


//! Instancia el SoundManager
SoundManager::SoundManager(){
	this->list = new map<string, SoundManagerItem*>();
}
		
//! Destruye el SoundManager
SoundManager::~SoundManager(){
	delete this->list;
}
		
//! Comprueba si el sonido obtenido a partir del archivo fname ya est� cargado.
/*!
	Este m�todo tiene un coste computacional reducido (el mapa se indexa por nombre de archivo)
	\param fname Nombre de archivo fuente
	\return true si el sonido ya ha sido cargado
*/
bool SoundManager::isLoaded(string fname){
	// Si el iterador que devuelve find es distinto del final entonces el sonido ya est� cargado
	return this->list->find(fname) != this->list->end();
}
		
//! Obtiene un sonido almacenado en el manager
/*!
	\param fname Nombre de archivo fuente solicitado
	\return Puntero al sonido cargado en memoria
*/
const Sound* SoundManager::getSound(string fname){
	// Antes comprobamos si el sonido est� cargado para que no casque
	if (this->isLoaded(fname)){
		// Si lo est� devolvemos el sonido del SoundManagerItem
		return this->list->find(fname)->second->getSound();
	}
	else{
		// Si no est� cargado devolvemos NULL
		return NULL;
	}
}
		
//! A�ade un nuevo sonido a la lista
/*!
	\param fname Nombre de archivo fuente
	\param sound Puntero a el sonido
	\return false si el fname ya estaban en memoria
*/
bool SoundManager::setSound(string fname, Sound* sound){
	// Si el sonido indexado por fname no est� en memoria
	if (!this->isLoaded(fname)){
		// envolvemos sound en SoundManagerItem
		SoundManagerItem* soundM = new SoundManagerItem(sound);
		// como al crear el SoundManagerItem el contador de enlaces est� a 0, se incrementa en 1
		soundM->link();
		// se inserta el par a la lista
		this->list->insert(make_pair(fname, soundM));
		// devolvemos true porque fname no estaba en memoria
		return true;
	}
	else{
		// Si no incrementamos el n� de enlaces del sonido porque ya estaba cargado
		this->list->find(fname)->second->link();
		// devolvemos false porque ya estaba en memoria
		return false;
	}
}
		
//! Elimina una entrada de la lista
/*!
	Elimina la entrada de la lista y tambi�n el sonido de memoria
	\param fname Nombre de archivo fuente
	\return true si se ha eliminado correctamente
*/
bool SoundManager::remove(string fname){
	// Si est� cargado en memoria...
	if (this->isLoaded(fname)){
		// Comprobamos si el n� de enlaces es 0
		if (this->list->find(fname)->second->unlink()){
			// Si no se elimina correctamente erase devuelve 0
			return (this->list->erase(fname) != 0);
		}
		else{
			// ya se ha decrementado el n� de enlaces en el unlink del if
			// devolvemos false porque el n� de enlaces es > que 0
			return false;
		}
	}
	else{
		// devolvemos false porque no estaba cargado
		return false;
	}
};