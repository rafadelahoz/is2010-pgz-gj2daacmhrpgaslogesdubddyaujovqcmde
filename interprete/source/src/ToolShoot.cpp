#include "ToolShoot.h"

ToolShoot::ToolShoot(int x, int y, Game* game, GameState* world) : Tool(x, y, game, world)
{
}

ToolShoot::~ToolShoot()
{
}

void ToolShoot::onInit()
{
	if (player->changeState(Player::Attack))
		activate();

	else 
		// hay que avisar a toolController de que no se puede atacar
		player->getController()->getToolController()->toolFinished(idTool);
}

void ToolShoot::activate()
{
	// debemos crear una instancia del arma y de la munici�n que dispara

	ToolAnimData data;
	std::string name;
	Direction dir = player->getDir();

	// Ejecutamos la animaci�n correspondiente en funci�n de la direcci�n a la que mira el player
	switch(dir){
	case UP:
		name = "up";
		break;
	case DOWN:
		name = "down";
		break;
	case LEFT:
		name = "left";
		break;
	case UPLEFT:
		name = "left";
		break;
	case DOWNLEFT:
		name = "left";
		break;
	case RIGHT:
		name = "right";
		break;
	case UPRIGHT:
		name = "right";
		break;
	case DOWNRIGHT:
		name = "right";
		break;
	}

	data = animList.at(name);						// cogemos los datos de la animaci�n
	playAnim(name);									// ejecutamos la animaci�n
	player->playAnim(playeranim, data.animSpeed);	// Hacemos que el player ejecute la animaci�n

	placeTool();	// Colocamos el arma en funci�n de la animaci�n actual

	// creamos la munici�n (en pruebas)
/*	ammo = new ToolAmmo(this->x, this->y, this->game, this->world);
	ammo->init(false, this->player, this->idTool, "path gr�fico de la munici�n", dir);
	game->getGameState()->add(ammo);*/

}

void ToolShoot::init(bool passive, Player* p, Player::PlayerAnim playeranim, int idTool, std::string graphicpath)
{
	Tool::init(passive, p, idTool);

	this->playeranim = playeranim;

	// cargamos las diferentes animaciones de la herramienta y su munici�n (quiz�s haga falta otra ruta para la munici�n)
	loadAnimations(graphicpath, getConfigurationFileName(graphicpath));
}

bool ToolShoot::loadAnimations(std::string graphicpath, std::string fname)
{
	// cargamos las animaciones del arma y su munici�n de forma similar a la de ToolMelee
	return true;
}

void ToolShoot::onInitStep()
{
	// Si la herramienta a acabado, se lo indicamos a ToolController
	if (((SpriteMap*)graphic)->animFinished())
	{
		player->changeState(Player::PlayerState::Normal);
		player->getController()->getToolController()->toolFinished(idTool);
	}
}

void ToolShoot::onEndStep()
{
	placeTool();
}