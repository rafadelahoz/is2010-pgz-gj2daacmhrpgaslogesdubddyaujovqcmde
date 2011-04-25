#pragma once

#ifndef __TOOLCONTROLLER_H__
#define __TOOLCONTROLLER_H__

#include "Player.h"
#include "iInteractable.h"

#include "Controller.h"
#include "ToolMelee.h"

// Distancia (en p�xeles) m�nima necesaria entre el player y una entidad interactuable
// para que el player interact�e con ella al pulsar un bot�n
const int D_INTERACT = 3;

const int MAX_TOOLS_EQUIPED = 2;

class ToolController {

	typedef enum ToolType {none, tool_Melee};	// tipos de herramientas disponibles

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
	std::vector<int> toolsToDelete;	// lista de herramientas que habr� que eliminar cuando finalice su acci�n
	
	/* debe guardar listas de todas las armas disponibles y su estado actual (usable, no, etc)
	posici�n 0 -> herramienta equipada en a
	posici�n 1 -> herramienta equipada en b */
	ToolData equipedTools[MAX_TOOLS_EQUIPED];

	// ------------------------------ M�TODOS AUXILIARES ------------------------------
	
	// interact�a con la base de datos para conseguir los datos de la herramienta
	ToolData createToolData(int idTool);

	// ataca con la herramienta equipada en pos
	void toolAttack(int pos, Player* player);

	// encuentra la herramienta indicada en el array de herramientas equipadas (si no est� devuelve -1)
	int findEquipedTool(int idTool);

	public:

		// CONSTRUCORES Y DESTRUCTORES
		ToolController(Controller* controller);
		~ToolController();

		// equipa la too correspondiente al player dado
		bool equip(int idTool, Player* player, char key);

		// ejecuta la acci�n de la tool dada del player:
		//	key == 'a' -> arma equipada en a
		//	key == 'b' -> arma equipada en b
		void attack(Player* player, char key);

		// La herramienta "idTool" ha finalizado su acci�n
		void toolFinished(int idTool);

		// borra las herramientas que hayan finalizado su ejecui�n
//		void clearTools();
};


#endif