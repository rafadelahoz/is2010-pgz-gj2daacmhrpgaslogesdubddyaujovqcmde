#include "MusicManager.h"

//! Instancia el MusicManager
MusicManager::MusicManager(){
	this->list = new map<string, MusicManagerItem*>();
}
		
//! Destruye el MusicManager
MusicManager::~MusicManager(){
	delete this->list;
}
		
//! Comprueba si la m�sica obtenida a partir del archivo fname ya est� cargada.
/*!
	Este m�todo tiene un coste computacional reducido (el mapa se indexa por nombre de archivo)
	\param fname Nombre de archivo fuente
	\return true si la m�sica ya ha sido cargada
*/
bool MusicManager::isLoaded(string fname){
	// Si el iterador que devuelve find es distinto del final entonces la m�sica ya est� cargada
	return this->list->find(fname) != this->list->end();
}
		
//! Obtiene una m�sica almacenada en el manager
/*!
	\param fname Nombre de archivo fuente solicitado
	\return Puntero a la m�sica cargada en memoria
*/
const Music* MusicManager::getMusic(string fname){
	// Antes comprobamos si el sonido est� cargado para que no casque
	if (this->isLoaded(fname)){
		// Devolvemos la m�sica del MusicManagerItem
		return this->list->find(fname)->second->getMusic();
	}
	else{
		// Si no est� cargado devolvemos NULL
		return NULL;
	}

}
		
//! A�ade una nueva m�sica a la lista
/*!
	\param fname Nombre de archivo fuente
	\param music Puntero a la m�sica
	\return false si la m�sica o el fname ya estaban en memoria
*/
bool MusicManager::setMusic(string fname, Music* music){
	// Si la m�sica indexada por fname no est� en memoria
	if (!this->isLoaded(fname)){
		// envolvemos music en MusicManagerItem
		MusicManagerItem* musicM = new MusicManagerItem(music);
		// como al crear el MusicManagerItem el contador de enlaces est� a 0, se incrementa en 1
		musicM->link();
		// se inserta el par a la lista
		this->list->insert(make_pair(fname, musicM));
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
	Elimina la entrada de la lista y tambi�n la m�sica de memoria
	\param fname Nombre de archivo fuente
	\return true si se ha eliminado correctamente
*/
bool MusicManager::remove(string fname){
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