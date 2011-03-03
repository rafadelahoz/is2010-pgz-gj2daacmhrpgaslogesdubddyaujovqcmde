#pragma once
#ifndef __SOUNDMANAGERITEM_H__
#define __SOUNDMANAGERITEM_H__

#include "Sound.h"

class Sound;

//! Elemento b�sico del SoundManager
/*!
	Dentro de un mismo juego distintos elementos pueden compartir
	un mismo archivo de efecto sonoro. Este elemento no deber�a cargarse 
	varias veces en memoria, sino compartirse entre todos 
	con el consecuente ahorro.
	
	De esta tarea se encarga SoundManager, que mantiene la lista
	de sonidos cargados en memoria de forma transparente al programador.

	SoundManagerItem es el elemento b�sico de esta lista.
	Relaciona un Sound (sonido) con el n�mero de elementos que lo utilizan
	para poder liberarlo cuando no sea necesario, asegurando que no
	se elimina mientras alguien lo est� utilizando.

	\sa SoundManager
	\sa Sound
*/
class SoundManagerItem
{

private:
	//! N�mero de enlaces actuales al sonido encapsulado
	unsigned int links;
	//! Sound (Sound encapsulado)
	Sound* sound;

public:
	//! Construye el SoundManagerItem para el m�sic sound
	/*!
		Inicializa el contador de enlaces a 0.
		\param sound Sonido encapsulado
	*/
	SoundManagerItem(Sound* sound);

	//! A�ade un nuevo enlace a el sonido
	void link();
	
	//! Elimina un enlace a el sonido
	/*!
		\return true si el sonido puede borrarse porque el n�mero de enlaces es 0.
	*/
	bool unlink(); // true si debe borrarse
	
	//! Obtiene el sonido encapsulado
	/*!
		\return Puntero a el sonido encapsulado
	*/
	Sound* getSound();
	
	//! Devuelve el n�mero de enlaces actual al sonido
	/*!
		\return N�mero de enlaces actuales
	*/
	int getLinks();
};

#endif