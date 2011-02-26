#pragma once
#ifndef __GFXENGINE_H__
#define __GFXENGINE_H__

#include "SFML/Graphics.hpp"
#include "Color.h"
#include "SurfaceManager.h"
#include "Image.h"

class Image;

//! Subsitema gr�fico encargado de facilitar funciones de renderizado al programador
/*!
	Se trata de la pieza clave del motor gr�fico y usa la clase Image
	encapsulando ambas la librer�a gr�fica subyaciente.
	
	Permite trabajar con los siguientes elementos:
	- Ventana de la aplicaci�n (Screen)
	- Ventana del juego (GameScreen)
	
	La ventana de la aplicaci�n es la ventana que ve el usuario.
	Sobre ella se dibuja la ventana del juego, que debe ser menor o igual en tama�o, y si el tama�o
	de la ventana de la aplicaci�n lo permite, puede mostrarse expandida en un factor determinado.
	El trabajo con la ventana de la aplicaci�n es transparente al usuario, que s�lo debe indicar
	el tama�o de la misma.
	
	GfxEngine proporciona, adem�s, m�todos para dibujar elementos sobre la ventana del juego;
	as� como m�todos para el manejo de im�genes (Images).
*/


class GfxEngine
{
	friend class Image;
	friend class EngineManager;
	
	private:
	
	/* Ventana del juego y de la aplicaci�n */
	
	int gameX, gameY;						// Posici�n de la ventana del juego
	int gameW, gameH;						// Dimensiones de la ventana del juego
	int gameScaleH, gameScaleV;				// Escala de la ventana del juego
	
	int screenW, screenH;					// Dimensiones de la ventana de la aplicaci�n
	int screenBPP;							// Profundidad de color de la ventana de la app
	Color* screenBgColor;					// Color de fondo de la ventana de la app
	
	sf::RenderWindow* appScreen;			// Ventana de la aplicaci�n
	sf::RenderImage* gameScreen;			// Ventana del juego
	sf::RenderImage* currentRenderTarget;	// Destino de render actual
	
	SurfaceManager* surfaceManager;			// Nuestro SurfaceManager

	/* ****************** *\
	|*	M�todos privados  *|
	\* ****************** */

	//! Inicializa el GfxEngine
	/*! Necesita que se le indique la ventana creada para la aplicaci�n
		\param window Ventana instanciada de la librer�a subyaciente
	*/
	bool init(sf::RenderWindow* window);

	// Centra la ventana del juego en la ventana de la aplicaci�n
	void centerGameScreen();

	/* *** Manejo de im�genes *** */

	// Crea una imagen (para escritura) de tama�o w x h
	sf::RenderImage* createImage(int w, int h);
	
	// Carga la imagen indicada
	sf::Image* loadImage(std::string fname);

	// Libera la imagen indicada
	bool deleteImage(std::string fname);
	bool deleteImage(sf::Image* image);

	public:

	//! Construye el GfxEngine con los par�metros indicados.
	/*!
		Este m�todo s�lo prepara la estructura del GfxEngine,
		que deber� inicializarse expresamente mediante GfxEngine::Init()
		\param screenw Ancho de la ventana de la aplicaci�n
		\param screenh Alto de la ventana de la aplicaci�n
		\param screenbpp Profundidad de color
		\param gameW [Opcional] Ancho de la ventana del juego (-1 para el ancho de la ventana de aplicaci�n)
		\param gameH [Opcional] Alto de la ventana del juego (-1 para el alto de la ventana de aplicaci�n)
		\param zoom [Opcional] Factor de ampliaci�n de la ventana de juego.
	*/
	GfxEngine(int screenw, int screenh, int screenbpp, int gameW = -1, int gameH = -1, int zoom = -1);
	
	//! Destructora
	~GfxEngine();
	
	/*********************************************************************\
	*	Manejo de la ventana del juego y de la ventana de la aplicaci�n	  *
	\*********************************************************************/
	
	//! Establece las dimensiones de la ventana de la aplicaci�n
	/*!
		En caso de no ser capaz de establecer las nuevas dimensiones, los valores
		no se ver�n modificados.
		\param width Ancho de la ventana
		\param height Alto de la ventana
		\return True si la operaci�n ha tenido �xito, false en otro caso
	*/
	bool setScreenSize(int width, int height);
	
	//! Establece las dimensiones de la ventana del juego
	/*!
		En caso de no ser capaz de establecer las nuevas dimensiones, los valores
		no se ver�n modificados.
		\param width Ancho de la ventana
		\param height Alto de la ventana
		\return True si la operaci�n ha tenido �xito, false en otro caso
	*/
	bool setGameScreenSize(int width, int height);
	
	//! Establece el factor de escalado de la ventana del juego
	/*!
		\param hFactor Escalado horizontal (1 para tama�o real)
		\param vFactor Escalado Vertical (1 para tama�o real)
		\return True si la operaci�n ha tenido �xito, false en otro caso
	*/
	bool setGameScreenScale(int hFactor, int vFactor);
	
	//! Obtiene el ancho de la ventana de la aplicaci�n
	/*! \return Ancho de la ventana de la aplicaci�n */
	int getScreenWidth();
	
	//! Obtiene el alto de la ventana de la aplicaci�n
	/*! \return Alto de la ventana de la aplicaci�n */
	int getScreenHeight();
	
	//! Obtiene el ancho de la ventana del juego
	/*! \return Ancho de la ventana del juego */
	int getGameScreenWidth();
	
	//! Obtiene el alto de la ventana del juego
	/*! \return Alto de la ventana del juego */
	int getGameScreenHeight();
	
	//! Obtiene el factor de zoom horizontal del juego
	/*! \return Factor de zoom horizontal del juego */
	int getGameScreenScaleH();
	
	//! Obtiene el factor de zoom vertical del juego
	/*! \return Factor de zoom vertical del juego */
	int getGameScreenScaleV();
	
	//! Establece el color del �rea de la ventana de la aplicaci�n no ocupada por la ventana del juego
	/*! \param color Color de fondo */
	void setScreenBackgroundColor(Color* color);
	
	//! Obtiene el color del �rea de la ventana de la aplicaci�n no ocupada por la ventana del juego
	/*! \return Color del �rea de la ventana de la aplicaci�n no ocupada por la ventana del juego */
	Color getScreenBackgroundColor();
	
	/*********************************************************************\
	*						M�todos de Renderizado						  *
	\*********************************************************************/
	
	//! Establece la Image sobre la que se realizar�n los renderizados que no especifiquen destino.
	/*! \param target Image sobre la que se realizar� el renderizado (debe estar inicializada y aceptar escritura) */
	void setRenderTarget(Image* target);
	
	//! Establece la ventana del juego como destino de los renderizados que no especifiquen destino.
	void resetRenderTarget();
	
	//! Renderiza una Image sobre el destino especificado o sobre la ventana del juego si no se especifica.
	/*!
		\param image Imagen a renderizar
		\param x Posici�n horizontal de image en dest
		\param y Posici�n vertical de image en dest
		\param dest [Opcional] Imagen destino del renderizado (NULL para Imagen por defecto)
	*/
	void render(Image* image, int x, int y, Image* dest = NULL);
	
	//! Renderiza parte de una Image sobre el destino especificado o sobre la ventana del juego si no se especifica.
	/*!
		\param image Imagen a renderizar
		\param x Posici�n horizontal de image en dest
		\param y Posici�n vertical de image en dest
		\param xOrigin Coordenada X de la esquina superior izquierda de la parte de image a renderizar
		\param yOrigin Coordenada Y de la esquina superior izquierda de la parte de image a renderizar
		\param width Ancho de la parte de image a renderizar
		\param height Alto de la parte de image a renderizar
		\param dest [Opcional] Imagen destino del renderizado (NULL para Imagen por defecto)
	*/
	void renderPart(Image* image, int x, int y, int xOrigin, int yOrigin, int width, int height, Image* dest = NULL);
	
	//! Renderiza una Image ampliada sobre el destino especificado o sobre la ventana del juego si no se especifica.
	/*!
		\param image Imagen a renderizar
		\param x Posici�n horizontal de image en dest
		\param y Posici�n vertical de image en dest
		\param xScale Factor de ampliaci�n horizontal
		\param yScale Factor de ampliaci�n vertical
		\param dest [Opcional] Imagen destino del renderizado (NULL para Imagen por defecto)
	*/
	void renderResized(Image* image, int x, int y, int xScale, int yScale, Image* dest = NULL);
	
	//! Renderiza una Image aplicando efectos
	/*!
		\param image Imagen a renderizar
		\param x Posici�n horizontal de image en dest
		\param y Posici�n vertical de image en dest
		\param color Color de tintado de image
		\param alpha Transparencia a aplicar
		\param scaleH Factor de ampliaci�n horizontal
		\param scaleV Factor de ampliaci�n vertical
		\param rotation �ngulo de rotaci�n a aplicar
		\param dest [Opcional] Imagen destino del renderizado (NULL para destino por defecto)
	*/
	void renderExt(Image* image, int x, int y, Color color, float alpha, float scaleH, float scaleV, float rotation, Image* dest = NULL);
	
	//! Renderiza parte de una Image aplicando efectos
	/*!
		\param image Imagen a renderizar
		\param x Posici�n horizontal de image en dest
		\param y Posici�n vertical de image en dest
		\param xOrigin Coordenada X de la esquina superior izquierda de la parte de image a renderizar
		\param yOrigin Coordenada Y de la esquina superior izquierda de la parte de image a renderizar
		\param width Ancho de la parte de image a renderizar
		\param height Alto de la parte de image a renderizar
		\param color Color de tintado de image
		\param alpha Transparencia a aplicar
		\param scaleH Factor de ampliaci�n horizontal
		\param scaleV Factor de ampliaci�n vertical
		\param rotation �ngulo de rotaci�n a aplicar
		\param dest [Opcional] Imagen destino del renderizado (NULL para destino por defecto)
	*/
	void renderPartExt(Image* image, int x, int y, int xOrigin, int yOrigin, int width, int height, Color color, float alpha, float scaleH, float scaleV, float rotation, Image* dest = NULL);
	
	//! Renderiza la ventana del juego centrada sobre la ventana de la aplicaci�n, aplicando el escalado correspondiente
	/*!
		Es necesario llamar a este m�todo para que el contenido de la ventana del juego sea visible al jugador.
	*/
	void display();
	
	/*********************************************************************\
	*						Manejo de Im�genes							  *
	\*********************************************************************/
/*	
	//! Crea una imagen vac�a con el ancho y el alto especificados con modo de escritura si fuera necesario.
	/*! 
		A partir del momento en que se devuelva la imagen creada, 
		ser� tarea del programador mantenerla y borrarla cuando no sea necesaria.
		Si la imagen acepta escritura, ocupa m�s en memoria y habr� que manejarla con
		mayor cuidado, aunque s�lo las im�genes que acepten escritura podr�n ser destino
		de los m�todos de renderizado.
		
		\param width Ancho de la imagen
		\param height Alto de la imagen
		\param write Modo de escritura activado para la imagen
		\return Un puntero a la nueva imagen o NULL si ha habido alg�n problema
	*_/
	Image* createImage(int width, int height, bool write = false);
	
	/* No estoy seguro de estas,
		podr�an eliminarse y que se encarge Image::~Image()
	
	//! Elimina la superficie que encapsula la imagen indicada por fname.
	bool deleteImage(string* fname);
	
	//! Elimina la superficie que encapsula la imagen que se le pasa.
	bool deleteImage(Image* image);
	*/
	
	//! Rellena la imagen con el color indicado
	/*!
		\param image Imagen a rellenar
		\param color Color de relleno
	*/
	void clearImage(Image* image, Color color);
};

#endif