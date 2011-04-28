#include "EnemyTool.h"

// Inicializaci�n por defecto
EnemyTool::EnemyTool(int x, int y, Game* game, GameState* world) : GameEntity(x, y, game, world)
{
	enemy = NULL;
	compAnim = NULL;
	// nos ponemos invisibles por defecto
	game->getGameState()->add(this);
	setVisible(false);
}

EnemyTool::~EnemyTool()
{
}

void EnemyTool::activate()
{
	// Si no estamos ya activados
	if(!active){
		EnemyToolAnimData data;
		std::string name;

		// nos activamos y preparamos el timer para desactivarnos
		active = true;
		setTimer(1, atkSpeed);
		// seteamos la direccion
		dir = enemy->dir;

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
		case RIGHT:
			name = "right";
			break;
		}

		setVisible(true);
		data = animList.at(name);						// cogemos los datos de la animaci�n
		playAnim(name);									// ejecutamos la animaci�n

		placeTool();	// Colocamos el arma en funci�n de la animaci�n actual
	}
}

void EnemyTool::onStep()
{
	if (active){
		int xtmp = x;
		int ytmp = y;

		// Movimiento de la munici�n en funci�n de la direcci�n y de la velocidad
		switch (dir)
		{
		case UP:
			ytmp -= travelSpeed;
			break;
		case DOWN:
			ytmp += travelSpeed;
			break;
		case LEFT:
			xtmp -= travelSpeed;
			break;
		case RIGHT:
			xtmp += travelSpeed;
			break;
		}

		// De momento no hacemos ninguna comprobaci�n
		x = xtmp; y = ytmp;

		// Actualizamos la profundidad del gr�fico
		depth = y;
	}
}

void EnemyTool::init(Enemy* e, int idTool, std::string graphicpath)
{
	this->enemy = e;
	this->idEnemyTool = idTool;
	dir = NONE;
	active = false;

	// cargamos las diferentes animaciones de la herramienta
	loadAnimations(graphicpath, getConfigurationFileName(graphicpath));
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
		for (vector<Component*>::iterator it = enemy->components->begin(); it != enemy->components->end(); ++it) 
		{
			if (compAnim = dynamic_cast<ComponentAnim *> (*it))
			break;
		}

		if(compAnim != NULL){
			int hotEnemyX, hotEnemyY;
		
			int eframe = ((SpriteMap*) enemy->graphic)->getCurrentFrame();
			if (eframe >= compAnim->getAnimationData(enemy->currentAnim, dir).frameData.size()){
				hotEnemyX = 0;
				hotEnemyY = 0;
			}
			else 
			{
				ComponentAnim::EnemyFrameData d = compAnim->getAnimationData(enemy->currentAnim, dir).frameData.at(eframe);
				hotEnemyX = d.hotspotX;
				hotEnemyY = d.hotspotY;
			}

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

			// Actualizamos la posici�n en funci�n del hotspot del enemy y del hotspot del frame actual de la espada
			x = enemy->x + hotEnemyX;//enemy->x + hotEnemyX - fd.hotspotX;
			y = enemy->y + hotEnemyY;//enemy->y + hotEnemyY - fd.hotspotY;

			
			// Actualizamos la m�scara
			if (mask != NULL) delete mask; // borramos la antigua
			mask = new MaskBox(x, y, fd.width, fd.height, "enemyTool", fd.offsetX, fd.offsetY); // creamos la nueva en la posici�n actual
			

			// Actualizamos la profundidad del gr�fico
			depth = y;
		}
	}
}

bool EnemyTool::loadAnimations(std::string graphicpath, std::string fname)
{
	SpriteMap* gfx = ((SpriteMap*) graphic);
	int nCols = 0, nRows = 0;

	// Carga el archivo de config y lee
	FILE* f = fopen(fname.c_str(), "r");

	// Si el archivo es inv�lido, no se puede hacer nada
	if (f == NULL)
		return false;

	// 1. Ancho y alto de imagen (?)
	if (fscanf(f, "%d %d", &nCols, &nRows) < 2)
		return false;

	// creamos el gr�fico de la herramienta
	graphic = new SpriteMap(graphicpath, nCols, nRows, game->getGfxEngine());

	// 2. Leer las animaciones
	loadAnimation(UP, "up", f);
	loadAnimation(DOWN, "down", f);
	loadAnimation(LEFT, "left", f);
	loadAnimation(RIGHT, "right", f);

	fclose(f);

	return true;
}

void EnemyTool::onRender()
{
	// TESTEO: Dibuja la m�scara del frame actual
	//game->getGfxEngine()->renderRectangle(x+fd.offsetX, y+fd.offsetY, fd.width, fd.height, Color::Blue);

	GameEntity::onRender();
}

bool EnemyTool::isActive(){
	return active;
}

void EnemyTool::onTimer(int timer)
{
	if (timer == 1){
		active = false;
		setVisible(false);
	}
}

void EnemyTool::onCollision(CollisionPair pair, Entity* other)
{
	if (pair.b == "player"){
		((Player*) other)->setLastEnemyDirection(dir);
		((Player*) other)->onDamage(enemy->strength, 0x1);
	}

	if (pair.b == "coltest")
	{
		this->instance_destroy();
	}
};

void EnemyTool::setAtkSpeed(int sp)
{
	atkSpeed = sp;
}

void EnemyTool::setAtkRange(int rg)
{
	atkRange = rg;
}

void EnemyTool::setTravelSpeed(int ts)
{
	travelSpeed = ts;
}

void EnemyTool::setDamage(int dmg)
{
	damage = dmg;
}