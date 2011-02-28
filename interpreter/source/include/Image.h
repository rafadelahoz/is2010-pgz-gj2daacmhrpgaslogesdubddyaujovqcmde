#pragma once
#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "SFML/Graphics.hpp"
#include "GfxEngine.h"

class GfxEngine;

//! Parte importante del subsistema gr�fico del motor.
/*!
	Una Image representa un mapa de bits cualquiera. Su ciclo de vida comienza
	a petici�n del programador, ya sea cargando un archivo de imagen (bmp, png...)
	o cre�ndo una imagen vac�a. Las im�genes pueden ser dibujadas sobre la
	pantalla de juego o sobre otras im�genes.

	Image encapsula un mapa de bits al nivel de la librer�a gr�fica subyaciente.
*/

class Image
{
	friend class GfxEngine;

	private:

        // Referencia al motor gr�fico
        GfxEngine* gfxEngine;

        bool writeable;
        bool transparent;

        // Imagen capaz de ser destino de un renderizado
        sf::RenderImage* wpic;

        // Imagen a pintar, elemento de s�lo lectura
        sf::Sprite* rpic;

        // Ruta de la imagen
        std::string path;

        // Devuelve la imagen modificable
		sf::RenderImage* getSurfaceW();

		// Devuelve la imagen de s�lo lectura
		sf::Sprite* getSurfaceR();

	public:

	//! Construye la imagen especificada con una ruta y unos par�metros dados.
	/*!
		Este m�todo, adem�s de inicializar los par�metros de la imagen,
		llama al m�todo Image::loadImage() para cargar el mapa de bits en memoria.
		\param path Ruta de la imagen dada.
		\param gfxEngine Puntero al subsistema gr�fico.
		\param transparent [Opcional] Flag que determina si la imagen ser� transparente o no.
		\param write [Opcional] Flag que determina si se puede pintar sobre la imagen o no.
	*/
	Image(std::string path, GfxEngine* gfxEngine, bool transparent = true, bool write = false);

	//! Construye una imagen vac�a para trabajar sobre ella.
	/*!
		\param gfxEngine Puntero al subsistema gr�fico.
		\param width Anchura de la imagen a crear.
		\param height Altura de la imagen a crear.
		\param transparent [Opcional] Flag que determina si la imagen ser� transparente o no.
		\param write [Opcional] Flag que determina si se puede pintar sobre la imagen o no.
	*/
	Image(GfxEngine* gfxEngine, int width, int height, bool transparent = true, bool write = false);

	//! Destructora.
	~Image();

	//! Carga la imagen especificada en la ruta.
	/*!
		\param path Ruta de la imagen dada.
		\param transparent [Opcional] Flag que determina si la imagen ser� transparente o no.
	*/
	void loadImage(string path, bool transparent = true);

	//! Realiza los cambios sobre la imagen writeable.
	void refresh();

	//! Obtiene el ancho de la imagen.
	/*!
		\return path Ancho de la imagen dada.
	*/
	int getWidth();

	//! Obtiene el alto de la imagen.
	/*!
		\return path Alto de la imagen dada.
	*/
	int getHeigth();

	//! Obtiene la ruta de la imagen.
	/*!
		\return path Ruta de la imagen.
	*/
	std::string getPath();
};

#endif
