#include "EnemyTool.h"

// Inicializaci�n por defecto
EnemyTool::EnemyTool(int x, int y, Game* game, GameState* world) : GameEntity(x, y, game, world)
{
	enemy = NULL;
}

EnemyTool::~EnemyTool()
{
}

void EnemyTool::init(Enemy* e, int idTool)
{
	this->enemy = e;
	this->idEnemyTool = idTool;
}

std::string EnemyTool::getConfigurationFileName(string fname)
{
	// El fname debe tener extensi�n
	if (fname.length() < 5)
		return "";

	std::string cfgname = fname;
	// Se quita la extensi�n del fname (.png)

#ifdef _VS2008_
	cfgname.substr(0, cfgname.size()-3);
#endif
#ifdef _VS2010_
	for (int i = 0; i < 3; i++)
		cfgname.pop_back();
#endif

	cfgname.append("cfg");

	return cfgname;
}

bool EnemyTool::loadAnimation(Direction dir, std::string name, FILE* from)
{
	// Cargar animaci�n indicada de from
	if (from == NULL || name == "")
		return false;

	int numFrames, speed;
	int* frameList;
	EnemyToolAnimData animData;
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

EnemyTool::FrameData EnemyTool::loadAnimationFrame(FILE* from)
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

	// Se lee el hotspot
	if (fscanf(from, "%d %d", &fd.hotspotX, &fd.hotspotY) < 1)
		return fd;

	// Se lee la m�scara
	if (fscanf(from, "%d %d %d %d", &fd.offsetX, &fd.offsetY, &fd.width, &fd.height) < 1)
		return fd;

	// Y por ahora ya
	return fd;
}

bool EnemyTool::playAnim(std::string name)
{
	// Si la animaci�n no existe, algo va mal
	std::map<std::string, EnemyToolAnimData>::iterator it;
	it = animList.find(name);
	if (it == animList.end())
		return false;
	// Si la animaci�n no tiene datos, algo va mal
	EnemyToolAnimData data = (*it).second;
	if (data.numFrames < 0)
		return false;

	// Establecer nueva animaci�n
	if(graphic != NULL) ((SpriteMap*) graphic)->playAnim(name, data.animSpeed, false, false);

	return true;
}

bool EnemyTool::animFinished()
{
	if(graphic != NULL)
		return ((SpriteMap*) graphic)->animFinished();
	else
		return true;
}

void EnemyTool::placeTool()
{
	// Comprobamos qu� animaci�n es la que se est� ejecutando
	std::string name;
	if(graphic != NULL)
		name = ((SpriteMap*) graphic)->getCurrentAnim();
	else
		return; // si no existe gr�fico no se puede colocar

	// Actualizamos la posici�n de la entidad si hay alguna animaci�n en curso
	if (name != "none")
	{

		// hotspot actual del player
		pair<int, int> hotPlayer;
		//hotPlayer = enemy->getCurrentHotSpot();
#ifdef _VS2010_
		EnemyToolAnimData animData = animList.at(name); // cogemos la informaci�n de la animaci�n actual
#endif
#ifdef _VS2008_
		// Si la animaci�n no existe, algo va mal
		std::map<std::string, ToolAnimData>::iterator it;
		it = animList.find(name);
		if (it == animList.end())
			return;
		// Si la animaci�n no tiene datos, algo va mal
		ToolAnimData animData = (*it).second;
#endif

		int frame = ((SpriteMap*) graphic)->getCurrentFrame(); // cogemos el frame actual
		FrameData fd = animData.frameData[frame];

		// Actualizamos la posici�n en funci�n del hotspot del player y del hotspot del frame actual de la espada
		x = enemy->x + hotPlayer.first - fd.hotspotX;
		y = enemy->y + hotPlayer.second - fd.hotspotY;

		// Actualizamos la m�scara
		if (mask != NULL) delete mask; // borramos la antigua
		mask = new MaskBox(x, y, fd.width, fd.height, "tool", fd.offsetX, fd.offsetY); // creamos la nueva en la posici�n actual

		// Actualizamos la profundidad del gr�fico
		depth = y;
	}
}