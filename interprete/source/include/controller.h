#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__


#include "Entity.h"
#include "iInteractable.h"

class Controller : public Entity {

	private:
	
		State state;
		TransEffect transitionEffect;

	public:
		// CONSTRUCORES Y DESTRUCTORES
		Controller();
		~Controller();
		
		// M�TODOS PERTENECIENTES A ENTITY
		void onStep();

		// M�TODOS PROPIOS
		void change_screen(Dir dir);
		void change_map();
		void attack(int idtool, objPlayer* player);
		
		void setState();
		void setTransitionEffect(TransEffect te);
}


#endif