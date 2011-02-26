#pragma once

#ifndef _SURFACEMANGER_H_
#define _SURFACEMANGER_H_

#include <map>
#include "SFML/Graphics.hpp"
#include "SurfaceManagerItem.h"

using namespace std;

//! Mantiene la colecci�n de superficies en memoria.
/*!
	Dentro de un mismo juego distintos elementos pueden compartir
	un mismo archivo gr�fico. Este elemento (al que llamaremos superficie)
	no deber�a cargarse varias veces en memoria, 
	sino compartirse entre todos con el consecuente ahorro.
	
	De esta tarea se encarga SurfaceManager, que mantiene la lista
	de superficies cargadas en memoria de forma transparente al programador.

	Se asegura de que las im�genes cargadas desde disco a memoria
	se mantienen sin duplicados y que se eliminan las que no se 
	necesiten, mediante un contador de enlaces asociado a cada
	superficie.
	
	Proporciona funcionalidad para comprobar si una superficie ya
	est� cargada, obtenerla en tal caso o a�adir una nueva entrada
	en otro caso.
*/
class SurfaceManager {
	private:
		//! Mapa de superficies enlazadas por nombre de archivo fuente
		map<string,SurfaceManagerItem*>* list;

	public:
		//! Instancia el SurfaceManager
		SurfaceManager();
		
		//! Destruye el SurfaceManager
		~SurfaceManager();
		
		//! Comprueba si la superficie obtenida a partir del archivo fname ya est� cargada.
		/*!
			Este m�todo tiene un coste computacional reducido (el mapa se indexa por nombre de archivo)
			\param fname Nombre de archivo fuente
			\return true si la imagen ya ha sido cargada
		*/
		bool isLoaded(string fname);
		
		//! Comprueba si la superficie indicada ya est� cargada
		/*!
			Este m�todo tiene un coste computacional elevado (hay que recorrer toda la lista de superficies)
			\param surf Puntero a la superficie a comprobar
			\return true si la imagen ya ha sido cargada
		*/
		bool isLoaded(sf::Image* surf);
		
		//! Obtiene una superficie almacenada en el manager
		/*!
			\param fname Nombre de archivo fuente solicitado
			\return Puntero a la imagen cargada en memoria
		*/
		const sf::Image* getSurface(string fname);
		
		//! A�ade una nueva superficie a la lista
		/*!
			\param fname Nombre de archivo fuente
			\param surf Puntero a la superficie
			\return false si la superficie o el fname ya estaban en memoria
		*/
		bool setSurface(string fname, sf::Image* surf);
		
		//! Elimina una entrada de la lista
		/*!
			Elimina la entrada de la lista y tambi�n la superficie de memoria
			\param fname Nombre de archivo fuente
			\return true si se ha eliminado correctamente
		*/
		bool remove(string fname);
		
		//! Elimina una entrada de la lista
		/*!
			Elimina la entrada de la lista y tambi�n la superficie de memoria
			\param surf Puntero a la superficie a eliminar
			\return true si se ha eliminado correctamente
		*/
		bool remove(const sf::Image* surf);
};

#endif // _SURFACEMANAGER_H_