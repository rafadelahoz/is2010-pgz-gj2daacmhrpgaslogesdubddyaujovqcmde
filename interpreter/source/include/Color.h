#pragma once
#ifndef __COLOR_H__
#define __COLOR_H__

//! Representa un color RGB
/*!
	Los colores se representan mediante 3 atributos red, green y blue,
	de 0 a 255 cada uno, correspondientes a los colores primarios luz,
	rojo, verde y azul, que indicar�n el valor del color.
*/

class Color
{
	public:
	
	//! Valor R de la nomenclatura RGB para el color.
	char r;
	//! Valor G de la nomenclatura RGB para el color.
	char g;
	//! Valor B de la nomenclatura RGB para el color.
	char b;
	
	//! Construye el color a partir de unos valores RGB dados
	/*!
		\param red Valor del rojo.
		\param green Valor del verde.
		\param blue Valor del azul.
	*/
	Color(char red, char green, char blue) { r = red; g = green; b = blue;}
	
	
	//! Color predefinido Negro
	static const Color Black;
	//! Color predefinido Blanco
	static const Color White;
	//! Color predefinido Rojo
	static const Color Red;
	//! Color predefinido Amarillo
	static const Color Yellow;
	//! Color predefinido Verde
	static const Color Green;
	//! Color predefinido Cian
	static const Color Cyan;
	//! Color predefinido Azul
	static const Color Blue;
	//! Color predefinido Magenta
	static const Color Magenta;
};
	
#endif