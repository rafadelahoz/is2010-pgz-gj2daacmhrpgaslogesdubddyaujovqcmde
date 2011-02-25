#pragma once

#ifndef __ANIM_H__
#define __ANIM_H__

using namespace std;

//! Anim es el elemento de animaci�n de mayor alto nivel.
/*!
	De �l derivar�n los distintos tipos de animaciones con que pueda trabajar el programador.
*/
class Anim {
	protected:
		//! Atributo de la clase anim.
		/*!
			\param _name nombre de la animaci�n.
		*/
		string name;
	public:

		//! Construye una animaci�n indicando su nombre
		/*! \param name Nombre de la animaci�n */
		Anim(string name);
		//! Destruye la animaci�n
		virtual ~Anim();
		
		//! Obtiene el nombre de la animaci�n
		/*!
			\return Nombre de la animaci�n
		*/
		virtual string getName();
};

#endif