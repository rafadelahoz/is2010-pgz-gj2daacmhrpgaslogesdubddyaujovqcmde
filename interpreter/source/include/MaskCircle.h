#pragma once
#ifndef __MASK_CIRCLE_H__
#define __MASK_CIRCLE_H__

//! M�scara de colisi�n �nica de forma circular.
/*!
	Se trata de un hijo de Mask, representando la m�scara de colisi�n
	de un objeto de forma circular.
*/

class MaskCircle : public Mask
{
	public:
	
	//! Coordenada x del centro de la m�scara circular.
	int xcenter;
	//! Coordenada y del centro de la m�scara circular.
	int ycenter;
	
	//! Radio de la m�scara circular.
	float ratio;
	
	//! Construye la m�scara a partir del centro y radio del c�rculo y las dimensiones globales.
	/*!
		\param x Coordenada x de la m�scara.
		\param y Coordenada y de la m�scara.
		\param width Ancho global de la m�scara.
		\param height Alto global de la m�scara.
		\param type Tipo de colisi�n de la m�scara.
		\param xc Coordenada x del centro de la m�scara circular.
		\param yc Coordenada y del centro de la m�scara circular.
		\param r Radio de la m�scara circular.
	*/
	MaskCircle(int x, int y, int width, int height, string type, int xc, int yc, float r);
	
	//! Destructora
	~MaskCircle();
		
	//! Comprueba si la m�scara circular dada colisiona con otra m�scara.
	/*!
		\param other M�scara con la que se comprueba la colisi�n.
		\return Lista de pares de colisi�n entre las 2 m�scaras.
	*/
	list<CollisionPair>* collide(Mask* other);
};
	
#endif