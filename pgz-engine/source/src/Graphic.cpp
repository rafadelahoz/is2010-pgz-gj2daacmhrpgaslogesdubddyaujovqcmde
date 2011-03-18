#include "Graphic.h"

Graphic::Graphic() 
{
	color = new Color(255,255,255);
	alpha = 1;
	scaleH = 1;
	scaleV = 1;
	rotation = 0;
	originX = 0;
	originY = 0;
};
	
//! Destructora
Graphic::~Graphic()
{	
	delete color;
	color = NULL;
}
		
//! Realiza la actualizaci�n en cada paso del juego.
void Graphic::update() {}
	
//! Realiza el renderizado de la imagen en las coordenadas dadas de la ventana de juego.
/*!
	\param x Coordenada x de reenderizado.
	\param y Coordenada y de reenderizado.
*/
 void Graphic::render(int x, int y){}
		
//! Tinta el gr�fico del color dado
/*!
	\param color Color con el que se tintar� la imagen.
*/
 void Graphic::setColor(Color color)
{
	*this->color = color;
}
	
//! Cambia la transparencia del gr�fico a un valor alpha.
/*!
	\param alpha Valor de la nueva transparencia del gr�fico.
*/
 void Graphic::setAlpha(float alpha)
{
	this->alpha = alpha;
}
	
//! Cambia la escala con que ser� renderizado el gr�fico a un valor scale.
/*!
	\param scale Valor de la nueva escala del gr�fico.
*/
 void Graphic::setScale(float scaleH, float scaleV)
{
	this->scaleH = scaleH;
	this->scaleV = scaleV;
}
	
//! Rota la imagen un angulo rotation.
/*!
	\param rotation Valor del �ngulo que rotar� el gr�fico.
*/
 void Graphic::setRotation(float rotation)
{
	this->rotation = rotation;
}

//! Establece la posici�n horizontal del origen de rotaci�n.
/*!
	\param x Posici�n horizontal del origen
*/
 void Graphic::setOriginX(int x)
{
	this->originX = x;
}

//! Establece la posici�n vertical del origen de rotaci�n.
/*!
	\param x Posici�n vertical del origen
*/
 void Graphic::setOriginY(int y)
{
	this->originY = y;
}

/* Uncomented zone of weirdness */

float Graphic::getAlpha() 
{ 
	return alpha; 
};

Color Graphic::getColor() 
{ 
	return Color(color->r, color->g, color->b); 
};

float Graphic::getRotation() 
{ 
	return rotation; 
};

float Graphic::getScaleH() 
{ 
	return scaleH; 
};

float Graphic::getScaleV() 
{ 
	return scaleV; 
};

int Graphic::getOriginX() 
{ 
	return originX; 
};

int Graphic::getOriginY() 
{ 
	return originY; 
};