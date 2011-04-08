#include "Tool.h"

// Inicializaci�n por defecto
Tool::Tool(int x, int y, Game* game, GameState* world, string graphicPath) : GameEntity(x, y, game, world)
{
	passive = true;
	player = NULL;
	// hay que crear el gr�fico a partir del graphicPath
	
}

Tool::~Tool()
{
}

void Tool::init(bool passive, Player* p)
{
	this->passive = passive;
	player = p;
}