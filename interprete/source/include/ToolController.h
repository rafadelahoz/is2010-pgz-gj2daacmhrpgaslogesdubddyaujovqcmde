#pragma once

#ifndef __TOOLCONTROLLER_H__
#define __TOOLCONTROLLER_H__

#include "Player.h"
#include "iInteractable.h"

#include "Controller.h"


// Distancia (en p�xeles) m�nima necesaria entre el player y una entidad interactuable
// para que el player interact�e con ella al pulsar un bot�n
const int D_INTERACT = 3;

const int MAX_EQUIPPED_TOOLS = 2;

class ToolMelee;
class ToolShoot;
class Tool;

class ToolController {

	typedef enum ToolType {none, tool_Melee, tool_Shoot, tool_Boomerang};	// tipos de herramientas disponibles

	struct ToolData
	{
		int idTool;				// Identificador de la herramienta
		ToolType type;			// Tipo de la herramienta
		string name;
		short damageType;		// Tipo de da�o de la herramienta
		int strength;			// Fuerza de la herramienta (da�o)
		Tool* tool;				// puntero a la herramienta
		bool inUse;				// Indica si se est� usando o no
		bool usable;			// Indica si se puede usar o no
		std::string gfxPath;	// Ruta con el gr�fico de la herramienta
		std::string ammoGfxpath;// Ruta de la munici�n de la herramienta ("" si no tiene)
		bool equippable;	// Indica si un arma se puede equipar o no
		short ammo;			// Munici�n actual del arma (-1 si es un arma sin munici�n)
		short maxAmmo;		// M�xima munici�n que puede tener el arma (-1 si es un arma sin munici�n)
	};

private:

	Controller* controller;			// puntero a Controller
	
	/* Guarda el identificador las armas equipadas en cada slot
	posici�n 0 -> herramienta equipada en slot 0
	posici�n 1 -> herramienta equipada en slot 1 */
	int equippedTools[MAX_EQUIPPED_TOOLS];

	// Herramientas disponibles en el juego <idTool, equippable>
	std::map<int, ToolData> tools;

	// ------------------------------ M�TODOS AUXILIARES ------------------------------
	
	// interact�a con la base de datos para conseguir los datos de la herramienta
	ToolData createToolData(int idTool);

	// ataca con la herramienta equipada en pos
	void toolAttack(short slot, Player* player);

public:
	friend class ToolBoomerang;

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

	// finaliza la acci�n de la tool (porque player ha sido golpeado, x ej)
	void stopTool(Player* player, short slot = -1);

	// consulta el identificador de la herramienta equipada en la tecla indicada (devuelve -1 si algo va mal)
	int equippedToolAt(short slot);

	// encuentra la herramienta indicada en el array de herramientas equipadas (si no est� devuelve -1)
	short findEquippedTool(int idTool);

	// La herramienta "idTool" ha finalizado su acci�n
	void toolFinished(int idTool);

	// Devuelve un vector con los identificadores de las herramientas que se pueden equipar
	std::vector<int> getEquippableTools();

	// Devuelve el grafico de la imagen de una herramienta
	Graphic* getToolGraphic(int idTool);

	//Devuleve el nombre de una Tool
	string getToolName(int idTool);

	// Aumenta en la cantidad indicada la munici�n del arma indicada
	void increaseAmmo(int idTool, short ammo);

	// Se devuelve la munici�n actual de la herramienta (-1 si la herramienta no tiene munici�n o no existe)
	short getToolAmmo(int idTool);

	std::string getToolGraphicPath(int idTool);
};


#endif