#pragma once

#ifndef __TOOLCONTROLLER_H__
#define __TOOLCONTROLLER_H__

#include "Player.h"
#include "iInteractable.h"

#include "Controller.h"
#include "ToolMelee.h"
#include "ToolShoot.h"

// Distancia (en p�xeles) m�nima necesaria entre el player y una entidad interactuable
// para que el player interact�e con ella al pulsar un bot�n
const int D_INTERACT = 3;

const int MAX_EQUIPPED_TOOLS = 2;

class ToolController {

	typedef enum ToolType {none, tool_Melee, tool_Shoot};	// tipos de herramientas disponibles

	struct ToolData
	{
		int idTool;					// Identificador de la herramienta
		ToolType type;				// Tipo de la herramienta
		Player::PlayerAnim pAnim;	// Animaci�n que tendr� que ejecutar el player
		Tool* tool;					// puntero a la herramienta
		bool inUse;					// Indica si se est� usando o no
		bool usable;				// Indica si se puede usar o no
		std::string gfxPath;		// Ruta con el gr�fico de la herramienta
	};

private:

	Controller* controller;			// puntero a Controller
	
	/* debe guardar listas de todas las armas disponibles y su estado actual (usable, no, etc)
	posici�n 0 -> herramienta equipada en slot 0
	posici�n 1 -> herramienta equipada en slot 1 */
	ToolData equippedTools[MAX_EQUIPPED_TOOLS];

	// ------------------------------ M�TODOS AUXILIARES ------------------------------
	
	// interact�a con la base de datos para conseguir los datos de la herramienta
	ToolData createToolData(int idTool);

	// ataca con la herramienta equipada en pos
	void toolAttack(short slot, Player* player);

	// encuentra la herramienta indicada en el array de herramientas equipadas (si no est� devuelve -1)
	short findEquippedTool(int idTool);

	public:

		// CONSTRUCORES Y DESTRUCTORES
		ToolController(Controller* controller);
		~ToolController();

		// equipa la too correspondiente al player dado
		bool equip(int idTool, Player* player, short slot);

		// ejecuta la acci�n de la tool dada del player:
		void attack(Player* player, short slot);

		// consulta el identificador de la herramienta equipada en la tecla indicada (devuelve -1 si algo va mal)
		int equippedToolAt(short slot);

		// La herramienta "idTool" ha finalizado su acci�n
		void toolFinished(int idTool);
};


#endif