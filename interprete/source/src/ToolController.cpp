#include "ToolController.h"

#include "ToolMelee.h"
#include "ToolShoot.h"
#include "ToolAmmo.h"

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
	for (int i = 0; i < MAX_EQUIPPED_TOOLS; i++)
		if (equippedTools[i].tool != NULL)
			equippedTools[i].tool->instance_destroy();
}

// de momento preparado s�lo para un player (habr� que tener m�s listas para m�s players)
// key indica lo mismo que en attack
bool ToolController::equip(int idTool, Player* player, short slot){

/*

	1. Modificar la info del player correspondiente a las tools con la nueva.
	2. Instanciar la tool si es necesario (activa de efecto pasivo)

	3. Comprobando siempre el estado de la misma (si est� us�ndose por otro player no se podr� equipar).

*/
	// De momento no se consideran herramientas pasivas

	// comprobamos que se puede equipar la herramienta en el slot solicitado
	if (slot < 0 || slot >= MAX_EQUIPPED_TOOLS) return false;

	// comprobamos que se pueda equipar la herramienta pedida
	map<int, tData>::iterator it = equippableTools.find(idTool);
	if (it == equippableTools.end())	// no existe la herramienta
		return false;
	else
		if (!it->second.equippable)	// existe pero no es equipable
			return false;

	// si la herramienta actual est� en uso no se puede equipar
	if (equippedTools[slot].inUse) return false;
	
	// Desequipamos la actual
	if (equippedTools[slot].tool != NULL) equippedTools[slot].tool->instance_destroy();

	// equipamos la herramienta nueva
	ToolData td = createToolData(idTool);
	equippedTools[slot] = td;
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
			td.damageType = PHYSICAL;
			td.gfxPath = "data/graphics/weapon-slashsword.png"; // habr� que cogerlo de la base de datos
			td.idTool = idTool;
			td.strength = 3;
			td.usable = true;
			td.type = ToolType::tool_Melee;
		}
		else if (idTool == 2) // Espada Thurst
		{
			td.damageType = PHYSICAL;
			td.gfxPath = "data/graphics/weapon-sword.png";	// habr� que cogerlo de la base de datos
			td.idTool = idTool;
			td.strength = 3;
			td.usable = true;
			td.type = ToolType::tool_Melee;
		}
		else if (idTool == 3)	// arco
		{
			// buscamos el arma para aumentarle la munici�n
			std::map<int, tData>::iterator it = equippableTools.find(idTool);
			// si todav�a no se ha buscado la munici�n del arma, se coge de la base de datos l� m�xima
			if (it->second.ammo == -1) it->second.ammo = 100;
			if (it->second.maxAmmo == -1) it->second.maxAmmo = 100;

			td.ammoGfxpath =  "data/graphics/arrow.png";
			td.gfxPath = "data/graphics/weapon-arc.png";	// habr� que cogerlo de la base de datos
			td.idTool = idTool;
			td.strength = 3;
			td.usable = true;
			td.type = ToolType::tool_Shoot;
		}
		else if (idTool == 4){ // bast�n m�gico
			// buscamos el arma para aumentarle la munici�n
			std::map<int, tData>::iterator it = equippableTools.find(idTool);
			// si todav�a no se ha buscado la munici�n del arma, se coge de la base de datos l� m�xima
			if (it->second.ammo == -1) it->second.ammo = 100;
			if (it->second.maxAmmo == -1) it->second.maxAmmo = 100;

			td.ammoGfxpath =  "data/graphics/fireBall.png";
			td.gfxPath = "data/graphics/magicStick.png";	// habr� que cogerlo de la base de datos
			td.idTool = idTool;
			td.usable = true;
			td.type = ToolType::tool_Shoot;			
		}
		return td;
	}
}

void ToolController::toolAttack(short slot, Player* player)
{
	Tool* t = NULL;
	// realizamos la acci�n correspondiente dependiendo del tipo del arma
	switch(equippedTools[slot].type)
	{
	case tool_Melee:
		if (equippedTools[slot].usable) // si la podemos usar
		{
			// creamos la herramienta correspondiente y la iniciamos
			t = new ToolMelee(player->x, player->y, controller->game, controller->game->getGameState());
			((ToolMelee*) t)->init(false, player, equippedTools[slot].idTool, equippedTools[slot].strength, equippedTools[slot].damageType, equippedTools[slot].gfxPath);
		}
		break;
	case tool_Shoot:
	{
		// buscamos la herramienta en el mapa (para modificar su munici�n)
		std::map<int, tData>::iterator it = equippableTools.find(equippedTools[slot].idTool);
		if (equippedTools[slot].usable && it->second.ammo > 0)
		{
			// decrementamos la munici�n del arma
			it->second.ammo--;

			int i = it->second.ammo;

			// creamos la herramienta y la iniciamos
			t = new ToolShoot(player->x, player->y, controller->game, controller->game->getGameState());
			((ToolShoot*) t)->init(false, player, equippedTools[slot].idTool, equippedTools[slot].strength, equippedTools[slot].damageType, equippedTools[slot].gfxPath, equippedTools[slot].ammoGfxpath);
		}
		break;
	}
	default: // si no existe el tipo, salimos
		return;
	}

	if (t != NULL)	// si todo ha salido bien
	{
		// A�adimos la entidad al gameState
		controller->game->getGameState()->add(t);
		// cambiamos los datos de la herramienta
		equippedTools[slot].inUse = true;
		equippedTools[slot].usable = false;
		equippedTools[slot].tool = t;
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

	// dependiendo del tipo de herramienta habr� que realizar una acci�n distinta (aunque de momento no)
/*	switch(equippedTools[slot].type)
	{
	case ToolType::tool_Melee:
		break;
	case ToolType::tool_Shoot:
		break;
	default:
		break;
	}
*/
	// Destruimos la herramienta y adi�s muy buenas
	equippedTools[slot].inUse = false;
	equippedTools[slot].tool->instance_destroy(); // eliminamos la herramienta
	equippedTools[slot].tool = NULL;
	equippedTools[slot].usable = true;
}

void ToolController::init(std::vector<int> tools)
{
	// a�adimos las herramientas del vector como par�metro, de momento las ponemos como no equipables
	tData td;
	td.ammo = -1; td.equippable = false; td.maxAmmo = -1;
	for (int i = 0; i < tools.size(); i++)
		equippableTools.insert(make_pair(tools[i], td));
}

void ToolController::setEquippable(int idTool, bool equippable)
{
	std::map<int, tData>::iterator it = equippableTools.find(idTool);	// buscamos la herramienta
	if (it != equippableTools.end())	// si existe
		(*it).second.equippable = equippable;	// hacemos el set
}

std::vector<int> ToolController::getEquippableTools()
{
	std::vector<int> v;
	std::map<int, tData>::iterator it;

	// vamos metiendo todas las armas equipables en el vector
	for (it = equippableTools.begin(); it != equippableTools.end(); it++)
		if (it->second.equippable)	// si es equipable
			v.push_back((*it).first);	// lo metemos en el vector

	return v;
}

void ToolController::increaseAmmo(int idTool, short ammo)
{
	std::map<int, tData>::iterator it = equippableTools.find(idTool); // buscamos la herramienta
	if (it != equippableTools.end())	// si existe aumentamos su munici�n
	{
		it->second.ammo += ammo;
		if (it->second.ammo > it->second.maxAmmo)	// si excede el m�ximo posible de al munici�n
			it->second.ammo = it->second.maxAmmo;	// ponemos el m�ximo
	}
}

short ToolController::getToolAmmo(int idTool)
{
	std::map<int, tData>::iterator it = equippableTools.find(idTool);
	
	if (it == equippableTools.end()) // si no existe la herramienta
		return -1;

	if (it->second.ammo == -1)	// puede ser porque el arma no tenga munici�n o porque todav�a no se haya cargado de la base de datos
		// aqu� habr�a que mirar si este arma tiene munici�n en la base de datos
		if (idTool == 3 || idTool == 4) // armas de disparo
			it->second.ammo = 100, it->second.maxAmmo = 100;

	// devolvemos la munici�n disponible del arma
	return it->second.ammo;
}

Graphic* ToolController::getToolGraphic(int idTool)
{
	FriendlyTileMap* grafico = new FriendlyTileMap(14,18,controller->game->getGfxEngine());
	ToolData td = createToolData(idTool);

	//Asigno el tileset
	grafico->setTileSet(td.gfxPath);
	
	//Creo el mapa del tileset
	int**map = (int**) malloc(sizeof(int*));
	map[0] = (int*) malloc(sizeof(int));
	//La asignaci�n de este tile ha sido arbitraria, cuando est� decidida su posici�n en el mismo, habr�
	//que guardarla en cada ToolData y aqui leerlo
	map[0][0] = 0;
	grafico->setMap(map, 1, 1);

	return grafico;
}