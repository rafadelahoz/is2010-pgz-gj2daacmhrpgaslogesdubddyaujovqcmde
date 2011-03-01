#pragma once
#ifndef __MASK_BOX_H__
#define __MASK_BOX_H__

#include <string>

using namespace std;

//! M�scara de colisi�n �nica de forma rectangular.
/*!
	Se trata de un hijo de Mask, representando la m�scara de colisi�n
	de un objeto de forma rectangular.
*/

class MaskBox : public Mask
{
	public:
	
	//! Construye la m�scara a partir de las dimensiones globales.
	/*!
		Las dimensiones de la m�scara rectangular ser�n las globales.
		\param x Coordenada x de la m�scara.
		\param y Coordenada y de la m�scara.
		\param width Ancho global de la m�scara.
		\param height Alto global de la m�scara.
		\param type Tipo de colisi�n de la m�scara.
	*/
	MaskBox(int x, int y, int width, int height, string type);
	
	//! Destructora
	~MaskBox();
		
	//! Comprueba si la m�scara rectangular dada colisiona con otra m�scara.
	/*!
		\param other M�scara con la que se comprueba la colisi�n.
		\return Lista de pares de colisi�n entre las 2 m�scaras.
	*/
	vector<CollisionPair>* collide(Mask* other);
};
	
#endif