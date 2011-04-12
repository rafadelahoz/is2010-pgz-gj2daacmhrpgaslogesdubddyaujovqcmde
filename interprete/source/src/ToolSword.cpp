#include "ToolSword.h"

ToolSword::ToolSword(int x, int y, Game* game, GameState* world) : Tool(x, y, game, world)
{
}

void ToolSword::init(bool passive, Player* p, string graphicpath, int ncol, int nrow)
{
	Tool::init(passive, p);

	// creamos el gr�fico de la espada??
	graphic = new SpriteMap(graphicpath, ncol, nrow, game->getGfxEngine());

	// cargamos las diferentes animaciones de la espada
	loadAnimations(getConfigurationFileName(graphicpath));

	// Habr� que crear la m�scara acorde con el gr�fico para que colisione chupi guay con los enemigos
	// TEMA HOTSPOT Y DEM�S
}

//
string ToolSword::getConfigurationFileName(string fname)
{
	// El fname debe tener extensi�n
	if (fname.length() < 5)
		return "";

	std::string cfgname = fname;
	// Se quita la extensi�n del fname (.png)
	for (int i = 0; i < 3; i++)
		cfgname.pop_back();
	cfgname.append("cfg");

	return cfgname;
};

bool ToolSword::loadAnimations(string fname)
{
	SpriteMap* gfx = ((SpriteMap*) graphic);
	int sprW = 0, sprH = 0;

	// Carga el archivo de config y lee
	FILE* f = fopen(fname.c_str(), "r");

	// Si el archivo es inv�lido, no se puede hacer nada
	if (f == NULL)
		return false;

	// 1. Ancho y alto de imagen (?)
	if (fscanf(f, "%d %d", &sprW, &sprH) < 2)
		return false;

	// 2. Leer las animaciones
	loadAnimation(Direction::UP, "up", f);
	loadAnimation(Direction::DOWN, "down", f);
	loadAnimation(Direction::LEFT, "left", f);
	loadAnimation(Direction::RIGHT, "right", f);
}

bool ToolSword::loadAnimation(Direction dir, std::string name, FILE* from)
{
	// Cargar animaci�n indicada de from
	if (from == NULL || name == "")
		return false;

	int numFrames, speed;
	int* frameList;
	SwordAnimData animData;
	FrameData frameData;

	// Leemos datos de la animaci�n

	// 0.Speed
	if (fscanf(from, "%d", &speed) < 1)
		return false;

	// 1.N� frames
	if (fscanf(from, "%d", &numFrames) < 1)
		return false;

	animData.animSpeed = speed;
	animData.numFrames = numFrames;
	animData.dir = dir;

	// Se instancia el contenedor de frames
	frameList = new int[numFrames];

	// Se carga cada frame
	for (int i = 0; i < numFrames; i++)
	{
		frameData = loadAnimationFrame(from);
		frameList[i] = frameData.frameId;
		animData.frameData.push_back(frameData);
	}

	// Se a�ade la animaci�n al graphic con el nombre indicado
	SpriteMap* gfx = ((SpriteMap*) graphic);
	gfx->addAnim(name, frameList, numFrames, speed, true);

	// Se mappea la anim con la direcci�n
	// Y se agrega su info
	animList.insert(make_pair(name, animData));
	delete frameList;

	return true;
}

// De momento solo devuelve el id del frame
ToolSword::FrameData ToolSword::loadAnimationFrame(FILE* from)
{
	FrameData fd;

	// De momento rellenamos con valores vac�os el resto de campos del framdata
	fd.hotspotX = 0;
	fd.hotspotY = 0;
	fd.offsetX = 0;
	fd.offsetY = 0;
	fd.height = 0;
	fd.width = 0;

	// El archivo debe ser v�lido
	if (from == NULL)
		return fd;

	// Se lee el frameId
	if (fscanf(from, "%d", &fd.frameId) < 1)
		return fd;

	// Y por ahora ya
	return fd;
}

bool ToolSword::playAnim(std::string name)
{
	// Si la animaci�n no tiene datos, algo va mal
	SwordAnimData data = animList.at(name);
	if (data.numFrames < 0)
		return false;

	// Establecer nueva animaci�n
	((SpriteMap*) graphic)->playAnim(name, data.animSpeed, false, false);

	return true;
}

void ToolSword::activate()
{
	Direction dir = player->getDir();
	
	// Ejecutamos la animaci�n correspondiente en funci�n de la direcci�n a la que mira el player
	switch(dir){
	case Direction::UP:
		/* FALTA: coger coordenadas del player para ejecutar la animaci�n */
		playAnim("up");
		break;
	case Direction::DOWN:
		/* FALTA: coger coordenadas del player para ejecutar la animaci�n*/
		playAnim("down");
		break;
	case Direction::LEFT:
		/* FALTA: coger coordenadas del player para ejecutar la animaci�n*/
		playAnim("left");
		break;
	case Direction::RIGHT:
		/* FALTA: coger coordenadas del player para ejecutar la animaci�n*/
		playAnim("right");
		break;
	}
}