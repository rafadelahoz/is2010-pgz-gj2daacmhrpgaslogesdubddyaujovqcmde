#include "ToolController.h"

ToolController::ToolController(Controller* controller)
{
	this->controller = controller;

	// creamos una informaci�n de herramienta vac�a
	ToolData td = createToolData(-1);

	// metemos herramientas vac�as en todas las posibles teclas para equipar
	for (int i=0; i<MAX_EQUIPPED_TOOLS; i++)
		equippedTools[i] = td;
}

ToolController::~ToolController()
{
	// borramos las herramientas que no hayan sido borradas todav�a
	for (int i=0; i<MAX_EQUIPPED_TOOLS; i++)
		if (equippedTools[i].tool != NULL)
			delete equippedTools[i].tool;
}

// de momento preparado s�lo para un player (habr� que tener m�s listas para m�s players)
// key indica lo mismo que en attack
bool ToolController::equip(int idTool, Player* player, short slot){

/*

	1. Modificar la info del player correspondiente a las tools con la nueva.
	2. Instanciar la tool si es necesario (activa de efecto pasivo)

	3. Comprobando siempre el estado de la misma (si est� us�ndose por otro player no se podr� equipar).

*/

	// Cuando est� lista la base datos, interactuar con ella para conseguir los datos de la herramienta
	// De momento no se consideran herramientas pasivas

	// comprobamos que se puede equipar la herramienta en el slot solicitado
	if (slot < 0 || slot >= MAX_EQUIPPED_TOOLS) return false;

	// cogemos la informaci�n de la base de datos
	ToolData td = createToolData(idTool);

	// la metemos en el array de armas equipadas
	if (equippedTools[slot].inUse) return false;	// si la herramienta est� en uso no se puede equipar
	
	equippedTools[slot] = td;	// equipamos la herramienta
	return true;
}


void ToolController::attack(Player* player, short slot){
	
/* ---------------------------------------------------------------------
	1. Interactuar si es necesario:
		1.1. obtener la posici�n frente al player
--------------------------------------------------------------------- */
			
	// Coordenadas del objeto frente al player
	int nx, ny;
	int up, left;
	up = left = 0;

	switch (player->getDir()) 
	{
		case UP: up = -1; break;
		case DOWN: up = 1; break;
		case LEFT:  left = -1; break;
		case RIGHT:  left = 1; break;
	}

	nx = (player->x) + left * D_INTERACT;
	ny = (player->y) + up * D_INTERACT;

/* ---------------------------------------------------------------------
		1.2. Si el objeto es interactable, interactuar con �l
--------------------------------------------------------------------- */
			
	// Objeto frente al player
	Entity* e = player->world->place_meeting(nx, ny, player, "npc");
				
	if (e != NULL)
	{
		// si es un objeto con el se puede interactuar, se har� lo propio
		if (iInteractable* i = dynamic_cast<iInteractable*>(e))
			i->onInteract(player);
	}
				
/* ---------------------------------------------------------------------
	2. Si no tiene con qui�n interactuar, efect�a la acci�n 
	de la herramienta.
---------------------------------------------------------------------*/
		
	else
	{
        // comprobamos que existe el slot soloicitado
		if (slot >= 0 && slot < MAX_EQUIPPED_TOOLS)
			toolAttack(slot, player);	// atacamos con la herramienta seleccionada
	}
}

ToolController::ToolData ToolController::createToolData(int idTool)
{
	// De momento creamos una informaci�n de herramienta vac�a
	ToolData td;
	td.ammoGfxpath = "";
	td.damageType = -1;
	td.gfxPath = "";
	td.idTool = -1;
	td.inUse = false;
	td.strength = -1;
	td.tool = NULL;
	td.type = ToolType::none;
	td.usable = false;

	// Si no existe el identificador lo devolvemos vac�o
	if (idTool == -1) return td;
	else	// Devolvemos la informaci�n dada por la base de datos
	{
		// td = DameDatosDBi(idTool)!!!!; return td; (cuando la base de datos est� lista)
		// mientras tanto....
		if (idTool == 1)	// Espada slash
		{
			td.damageType = 1;
			td.gfxPath = "data/graphics/weapon-slashsword.png"; // habr� que cogerlo de la base de datos
			td.idTool = idTool;
			td.strength = 3;
			td.usable = true;
			td.type = ToolType::tool_Melee;
		}
		else if (idTool == 2) // Espada Thurst
		{
			td.damageType = 1;
			td.gfxPath = "data/graphics/weapon-sword.png";	// habr� que cogerlo de la base de datos
			td.idTool = idTool;
			td.strength = 3;
			td.usable = true;
			td.type = ToolType::tool_Melee;
		}
		else	// arco
		{
			td.ammoGfxpath =  "data/graphics/arrow.png";
			td.gfxPath = "data/graphics/weapon-arc.png";	// habr� que cogerlo de la base de datos
			td.idTool = idTool;
			td.usable = true;
			td.type = ToolType::tool_Shoot;
		}
		return td;
	}
}

void ToolController::toolAttack(short slot, Player* player)
{
	// realizamos la acci�n correspondiente dependiendo del tipo del arma
	switch(equippedTools[slot].type)
	{
	case tool_Melee:
		if (equippedTools[slot].usable) // si la podemos usar
		{
			// creamos la herramienta correspondiente y la iniciamos
			ToolMelee* meleeTool;
			meleeTool = new ToolMelee(player->x, player->y, controller->game, controller->game->getGameState());
			meleeTool->init(false, player, equippedTools[slot].idTool, equippedTools[slot].strength, equippedTools[slot].damageType, equippedTools[slot].gfxPath);
			// A�adimos la entidad al gameState
			controller->game->getGameState()->add(meleeTool);
			// cambiamos los datos de la herramienta
			equippedTools[slot].inUse = true;
			equippedTools[slot].usable = false;
			equippedTools[slot].tool = meleeTool;
		}
		break;
	case tool_Shoot:
		if (equippedTools[slot].usable)
		{
			ToolShoot* shootingTool;
			shootingTool = new ToolShoot(player->x, player->y, controller->game, controller->game->getGameState());
			shootingTool->init(false, player, equippedTools[slot].idTool, equippedTools[slot].strength, equippedTools[slot].damageType, equippedTools[slot].gfxPath, equippedTools[slot].ammoGfxpath);
			controller->game->getGameState()->add(shootingTool);
			equippedTools[slot].inUse = true;
			equippedTools[slot].usable = false;
			equippedTools[slot].tool = shootingTool;
		}
		break;
	default: // si no existe el tipo, salimos
		return;
	}
}

short ToolController::findEquippedTool(int idTool)
{
	short slot = 0;
	
	// buscamos en el array de armas equipadas
	while (equippedTools[slot].idTool != idTool && slot < MAX_EQUIPPED_TOOLS)
		slot++;

	if (slot == MAX_EQUIPPED_TOOLS)
		return -1;	// no lo hemos encontrado
	else
		return slot;	// devolvemos la posici�n en la que se encuentra	
}

int ToolController::equippedToolAt(short slot)
{
	if (slot < 0 || slot > MAX_EQUIPPED_TOOLS)
		return equippedTools[slot].idTool;
	else
		return -1;
}

void ToolController::toolFinished(int idTool)
{
	short slot = findEquippedTool(idTool);	// buscamos la herramienta que ha finalizado

	// dependiendo del tipo de herramienta habr� que realizar una acci�n distinta
	switch(equippedTools[slot].type)
	{
	case ToolType::tool_Melee:
		equippedTools[slot].inUse = false;
		controller->gamePlayState->remove(equippedTools[slot].tool); // eliminamos la herramienta
		equippedTools[slot].tool = NULL;
		equippedTools[slot].usable = true;
		break;
	case ToolType::tool_Shoot:
		equippedTools[slot].inUse = false;
		controller->gamePlayState->remove(equippedTools[slot].tool); // eliminamos la herramienta
		equippedTools[slot].tool = NULL;
		equippedTools[slot].usable = true;
		break;
	default:
		break;
	}
}