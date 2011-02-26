#pragma once
#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__

#include "Color.h"

//! Clase virtual de la que heredar�n todos los elementos gr�ficos
/*!
	Ofrece funciones de update (actualizaci�n con el transcurso de un tick 
	del juego) y render (momento en que se pinta por pantalla), as� como 
	modificadores de color, transparencia, escala y rotaci�n de la imagen.
*/
class Graphic {
	protected:
		//! Color que va a tener el gr�fico.
		Color* color;
		//! Transparencia del gr�fico.
		float alpha;
		//! Escala en la que se representar� el gr�fico.
		float scale;
		//! Valor del �ngulo que rotar� el gr�fico.
		float rotation;

	public:
	
	//! Construye el elemento gr�fico
	Graphic() 
	{
		color = new Color(255,255,255);
		alpha = 1;
		scale = 1;
		rotation = 0;
	};
	
	//! Destructora
	virtual ~Graphic()
	{	
		delete color;
		color = NULL;
	}
		
	//! Realiza la actualizaci�n en cada paso del juego.
	virtual void update() {}
	
	//! Realiza el renderizado de la imagen en las coordenadas dadas de la ventana de juego.
	/*!
		\param x Coordenada x de reenderizado.
		\param y Coordenada y de reenderizado.
	*/
	virtual void render(int x, int y){}
		
	//! Tinta el gr�fico del color dado
	/*!
		\param color Color con el que se tintar� la imagen.
	*/
	virtual void setColor(Color color)
	{
		*this->color = color;
	}
	
	//! Cambia la transparencia del gr�fico a un valor alpha.
	/*!
		\param alpha Valor de la nueva transparencia del gr�fico.
	*/
	virtual void setAlpha(float alpha)
	{
		this->alpha = alpha;
	}
	
	//! Cambia la escala con que ser� renderizado el gr�fico a un valor scale.
	/*!
		\param scale Valor de la nueva escala del gr�fico.
	*/
	virtual void setScale(float scale)
	{
		this->scale = scale;
	}
	
	//! Rota la imagen un angulo rotation.
	/*!
		\param rotation Valor del �ngulo que rotar� el gr�fico.
	*/
	virtual void setRotation(float rotation)
	{
		this->rotation = rotation;
	}
		
};

#endif
