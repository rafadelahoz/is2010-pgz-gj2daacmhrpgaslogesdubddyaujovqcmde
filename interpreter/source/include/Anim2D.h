#pragma once

#ifndef __ANIM2D_H__
#define __ANIM2D_H__

using namespace std;

//! Miembro de la familia de Anim que representa una animaci�n en dos dimensiones.
/*!
	Almacena los frames o marcos de imagen en que consiste la animaci�n, 
	la velocidad de reproducci�n y si debe repetirse al terminar.
	
	Cada frame se representa como un entero que indica la posici�n del frame
	en la imagen bidimensional ligada a la animaci�n.
*/
class Anim2D : public Anim 
{
	private:
		//! Atributos de la clase Anim2D.
		/*!
			\param frameCount contar� el n�mero de frames que han pasado.
			\param framesPerStep contiene la informaci�n del n�mero de frames que se har�n en un step.
			\param frameList contiene la lista de frames que se van a utilizar para la animaci�n dada
			\param loop indica si la animaci�n ha de repetirse o no.
		*/
		int frameCount;
		int framesPerStep;
		vector<int>* frameList;
		bool loop;
		
	public:

		//! Construye la Anim2D
		/*!
			\param name Nombre de la animaci�n.
			\param frames Vector de frames de la animaci�n
			\param framesPerStep Pasos del juego que dura cada frame
			\param loop Indica si la animaci�n ha de repetirse o no
		*/
		Anim2D(string name, vector<int>* frames, int framesPerStep, bool loop);

		//! Destruye la animaci�n
		~Anim2D();
		
		/* Getters & Setters */
		
		//! Obtiene el n�mero de frames por step de la animaci�n
		/*!
			\return N�mero de pasos del juego que dura cada frame
		*/
		int getFramesPerStep();

		//! Establece el n�mero de frames por step de la animaci�n
		/*!
			\param framesPerStep Pasos del juego que dura cada frame
		*/
		void setFramesPerStep(int framesPerStep);
		
		//! Obtiene si la animaci�n se ha de repetir
		/*!
			\return True si la animaci�n se repite
		*/
		bool getLoop();

		//! Establece si la animaci�n ha de repetirse
		/*!
			\param loop True si debe repetirse
		*/
		void setLoop(bool loop);
		
		//! Obtiene el nombre de la animaci�n
		/*!
			\return Nombre de la animaci�n
		*/
		string getName();
		
		//! Obtiene el n�mero de frames de la animaci�n
		/*!
			\return N�mero de frames
		*/
		int getFrameCount();

		//! Obtiene el vector con los frames de la animaci�n
		/*!
			\return Vector de frames
		*/
		vector<int>* getFrameList();
};

#endif