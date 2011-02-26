#include "SoundManagerItem.h"


//! Construye el SoundManagerItem para el m�sic sound
/*!
	Inicializa el contador de enlaces a 0.
	\param sound Sonido encapsulado
*/
SoundManagerItem::SoundManagerItem(Sound* sound){
	// Enlazamos el sonido
	this->sound = sound;
	// contador de enlaces a 0
	links = 0;
}

//! A�ade un nuevo enlace al sonido
void SoundManagerItem::link(){
	this->links++; // incrementamos el n� de enlaces
}
	
//! Elimina un enlace al sonido
/*!
	\return true si el sonido puede borrarse porque el n�mero de enlaces es 0.
*/
bool SoundManagerItem::unlink(){
	this->links--; // decrementamos el n�mero de enlaces
	return (this->links == 0); // Si el n� de enlaces es 0 se puede eliminar
} // true si debe borrarse
	
//! Obtiene el sonido encapsulado
/*!
	\return Puntero a el sonido encapsulado
*/
Sound* SoundManagerItem::getSound(){
	return this->sound;
}
	
//! Devuelve el n�mero de enlaces actual al sonido
/*!
	\return N�mero de enlaces actuales
*/
int SoundManagerItem::getLinks(){
	return this->links;
};