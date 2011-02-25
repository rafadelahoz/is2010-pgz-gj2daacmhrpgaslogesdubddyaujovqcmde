#pragma once
#ifndef __CANVAS_H__
#define __CANVAS_H__

#include "Image.h"
#include "Graphic.h"

//! Canvas es un miembro de la familia de Graphic sobre el que se puede dibujar.
/*!
	Permite mostrar de forma c�moda una imagen compuesta por varias otras.
	
	El programador ha de tener cuidado de no liberar las im�genes que componen
	el Canvas mientras dure la vida de este.
	
	\sa Graphic
*/
class Canvas : Graphic
{
private:
	GfxEngine* gfxEngine;
	Image* img;
	int w, h;

public:

	//! Posici�n horizontal
	int x;
	
	//! Posici�n vertical
	int y;
	
	//! Construye el Canvas con los par�metros indicados
	/*! 
		\param x posici�n horizontal
		\param y posici�n vertical
		\param width ancho del lienzo
		\param height alto del lienzo
		\param gfxEngine puntero al GfxEngine del sistema
	*/
	Canvas(int x, int y, int width, int height, GfxEngine* gfxEngine);

	//! Destruye el Canvas (no los gr�ficos que lo forman)
	~Canvas();

	//! Obtiene el ancho del Canvas
	/*! \return ancho del Canvas */
	int getW() { return w; };
	//! Obtiene el alto del Canvas
	/*! \return alto del Canvas */
	int getH() { return h; };

	//! Dibuja un  objeto en el lienzo, en la posici�n indicada
	/*!
		Se puede dibujar parte del objeto gr�fico especificando
		las coordenadas del rect�ngulo que la contiene.
		\param gfx objeto gr�fico a dibujar
		\param x posici�n horizontal
		\param y posici�n vertical
		\param xOrigin coordenada horizontal de la esquina superior derecha de la parte a dibujar
		\param yOrigin coordenada vertical de la esquina superior derecha de la parte a dibujar
		\param w ancho de la parte a dibujar
		\param h alto de la parte a dibujar
	*/
	void draw(Graphic* gfx, int x, int y, int xOrigin = 0, int yOrigin = 0, int w = 0, int h = 0);

	//! Renderiza el Canvas
	/*!
		\param x Posici�n horizontal
		\param y Posici�n vertical
	*/
	void render(int x, int y);
};

#endif