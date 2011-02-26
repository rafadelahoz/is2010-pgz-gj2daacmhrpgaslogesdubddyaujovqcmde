#pragma once
#ifndef __MASK_H__
#define __MASK_H__

#include <string>
#include <vector>

using namespace std;

//! Par de colisi�n que representa los tipos de colisi�n que se han producido.
struct CollisionPair {
	//! Tipo de colisi�n de la m�scara A en la colisi�n.
	string a;
	//! Tipo de colisi�n de la m�scara B en la colisi�n.
	string b;
};


//! M�scara de colisi�n abstracta capaz de comprobar colisiones con otras m�scaras.
/*!
	Una m�scara es un elemento abstracto que representa la estructura f�sica 
	de una entidad y su delimitaci�n a la hora de calcular su contacto con otras entidades.
	
	Contiene adem�s una etiqueta editable que definir� qu� tipo de colisi�n posee 
	la m�scara, �til a la hora de calcular colisiones. Esta etiqueta ser� una cadena 
	de caracteres y deber� ser constru�da por el programador.
*/
class Mask
{
	public:
	
	//! Valor de la anchura de las dimensiones globales de la m�scara de colisi�n.
	/*!
		Toda m�scara hija estar� envuelta en una m�scara abstracta global.
		Debe ser colocada cuidadosamente, ya que dos m�scaras no colisionan 
		si sus m�scaras globales no est�n en contacto, aunque sus 
		correspondientes m�scaras internas s� colisionen.
	*/
	int width;

	//! Valor de la altura de las dimensiones globales de la m�scara de colisi�n.
	/*!
		Toda m�scara hija estar� envuelta en una m�scara abstracta global.
		Debe ser colocada cuidadosamente, ya que dos m�scaras no colisionan 
		si sus m�scaras globales no est�n en contacto, aunque sus 
		correspondientes m�scaras internas s� colisionen.
	*/
	int height;
	
	//! Coordenada x de posici�n de la m�scara.
	int x;
	//! Coordenada y de posici�n de la m�scara.
	int y;
	
	//! Tipo de colisi�n de la m�scara.
	string type;
	
	//! Construye la m�scara a partir de una anchura y altura globales dadas.
	/*!
		\param x Coordenada x de la m�scara.
		\param y Coordenada y de la m�scara.
		\param width Ancho de la m�scara.
		\param height Alto de la m�scara.
		\param type Tipo de colisi�n de la m�scara.
	*/
	Mask(int x, int y, int width, int height, string type);
	
	//! Destructora
	~Mask();
		
	//! Comprueba si la m�scara dada colisiona con otra m�scara.
	/*!
		\param other M�scara con la que se comprueba la colisi�n.
		\return Lista de pares de colisi�n entre las 2 m�scaras.
	*/
	virtual vector<CollisionPair>* collide(Mask* other);
};
	
#endif