#pragma once
#ifndef __TOOLCONTROLLER_H__
#define __TOOLCONTROLLER_H__

#include "Player.h"

class ToolController {

	public:
		// debe guardar listas de todas las armas disponibles y su estado actual (usable, no, etc)

		// CONSTRUCORES Y DESTRUCTORES
		ToolController(Game* g, GameState* gs);
		~ToolController();

		// equipa la too correspondiente al player dado
		bool equip(int idTool, Player* player);

		// ejecuta la acci�n de la tool dada del player
		void attack(int idtool, Player* player);

};


#endif