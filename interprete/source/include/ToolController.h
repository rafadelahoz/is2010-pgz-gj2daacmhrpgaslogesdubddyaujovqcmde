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
		int idTool;				// Identificador de la herramienta
		ToolType type;			// Tipo de la herramienta
		short damageType;		// Tipo de da�o de la herramienta
		int strength;			// Fuerza de la herramienta (da�o)
		Tool* tool;				// puntero a la herramienta
		bool inUse;				// Indica si se est� usando o no
		bool usable;			// Indica si se puede usar o no
		std::string gfxPath;	// Ruta con el gr�fico de la herramienta
		std::string ammoGfxpath;// Ruta de la munici�n de la herramienta ("" si no tiene)
	};

private:

	Controller* controller;			// puntero a Controller
	
	/* Guarda la informaci�n de las armas equipadas en cada slot
	posici�n 0 -> herramienta equipada en slot 0
	posici�n 1 -> herramienta equipada en slot 1 */
	ToolData equippedTools[MAX_EQUIPPED_TOOLS];

	// Herramientas disponibles en el juego <idTool, equippable>
	std::map<int, bool> equippableTools;

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

		// inicia el ToolController con los identificadores de las herramientas que va a haber disponbles en el juego
		void init(std::vector<int> tools);

		// setter de si una herramienta es equipable o no
		void setEquippable(int idTool, bool equippable);

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