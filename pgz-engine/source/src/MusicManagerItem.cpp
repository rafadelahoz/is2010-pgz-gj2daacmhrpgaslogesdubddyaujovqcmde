#include "MusicManagerItem.h"


//! Construye el MusicManagerItem para la m�sic music
/*!
	Inicializa el contador de enlaces a 0.
	\param music M�sica encapsulada
*/
MusicManagerItem::MusicManagerItem(sf::Music* music){
	// Enlazamos el sonido
	this->music = music;
	// contador de enlaces a 0
	links = 1;
}

//! A�ade un nuevo enlace a la m�sica
void MusicManagerItem::link(){
	this->links++; // incrementamos el n� de enlaces
}
	
//! Elimina un enlace a la m�sica
/*!
	\return true si la m�sica puede borrarse porque el n�mero de enlaces es 0.
*/
bool MusicManagerItem::unlink(){
	this->links--; // decrementamos el n�mero de enlaces
	return (this->links == 0); // Si el n� de enlaces es 0 se puede eliminar
} // true si debe borrarse
	
//! Obtiene la m�sica encapsulada
/*!
	\return Puntero a la m�sica encapsulada
*/
sf::Music* MusicManagerItem::getMusic(){
	return this->music;
}
	
//! Devuelve el n�mero de enlaces actual a la m�sica
/*!
	\return N�mero de enlaces actuales
*/
int MusicManagerItem::getLinks(){
	return this->links;
};