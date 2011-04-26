#include "ToolAmmo.h"

ToolAmmo::ToolAmmo(int x, int y, Game* game, GameState* world) : Tool(x, y, game, world) {}

ToolAmmo::~ToolAmmo() {}

void ToolAmmo::init(bool passive, Player* p, int idTool, std::string graphicpath, Direction dir)
{
	Tool::init(passive, p, idTool);		// init del padre

	this->dir = dir;

	// Cargar imagen de munici�n

	// Colocarla munici�n en funci�n del arma que nos ha creado (acordarse de ajustar profundidad)

	// Crear la m�scara
}



void ToolAmmo::onInit()
{
	activate();
}

void ToolAmmo::activate()
{
	// m... algo habr� que hacer digo yo
}

void ToolAmmo::onStep()
{
	int xtmp, ytmp;

	// Movimiento de la munici�n en funci�n de la direcci�n y de la velocidad
	switch (dir)
	{
	case UP:
		ytmp -= speed;
		break;
	case DOWN:
		ytmp += speed;
		break;
	case LEFT:
		xtmp -= speed;
		break;
	case UPLEFT:
		xtmp -= speed;
		ytmp -= speed;
		break;
	case DOWNLEFT:
		xtmp -= speed;
		ytmp += speed;
		break;
	case RIGHT:
		xtmp += speed;
		break;
	case UPRIGHT:
		xtmp += speed;
		ytmp -= speed;
		break;
	case DOWNRIGHT:
		xtmp += speed;
		ytmp += speed;
		break;
	}

	// De momento no hacemos ninguna comprobaci�n
	x = xtmp; y = ytmp;

	// Actualizamos la m�scara
	if (mask != NULL) delete mask; // borramos la antigua
	mask = new MaskBox(x, y, width, height, "ammo", 0, 0); // creamos la nueva en la posici�n actual

	// Actualizamos la profundidad del gr�fico
	depth = y;
}