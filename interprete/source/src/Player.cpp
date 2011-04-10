#include "Player.h"

#define IMG_WIDTH 14    // ancho de la imagen del player
#define IMG_HEIGHT 15    // alto de la imagen del player

// Suponemos que mask y graphic no est�n inicialmente creados, sino dejar�a basura
Player::Player(int x, int y, Game* game, GameState* world) : GameEntity(x, y, game, world) {

   // Creamos la m�scara
   mask = new MaskBox(x, y, IMG_WIDTH, IMG_HEIGHT, "player", 1, 7); // offsets est�n a cero por defecto

   // Cambiamos la configuraci�n por defecto de los flags que nos interesan
   solid = true;
};

Player::~Player()
{
	// Delete todo
	animList.clear();
	animDataList.clear();
};

bool Player::init(std::string gfxpath, int ncol, int nrow, int hp, int mp, Controller* c)
{
	// Asignamos el gr�fico a la entidad player, de momento una imagen est�tica
	graphic = new SpriteMap(gfxpath, ncol, nrow, game->getGfxEngine());
	this->hp = hp;
	this->mp = mp;

	controller = c;

	loadAnimations(getConfigurationFileName(gfxpath));

	state = Normal;
	dir = DOWN;

	return true;
}

bool Player::getNewPos(int& xtemp, int& ytemp)
{
	int move_pixels = 3; // n�mero de p�xeles que se mueve el player

	if	(((game->getInput()->key(Input::kRIGHT)) || (game->getInput()->key(Input::kLEFT)))
			&&
		 ((game->getInput()->key(Input::kUP)) || (game->getInput()->key(Input::kDOWN))))
		 move_pixels = 2;


	if (game->getInput()->key(Input::kRIGHT)) {xtemp += move_pixels; dir = RIGHT;}
	if (game->getInput()->key(Input::kLEFT)) {xtemp -= move_pixels; dir = LEFT;}
		// movimento vertical
	if (game->getInput()->key(Input::kUP)) {ytemp -= move_pixels; dir = UP;}
	if (game->getInput()->key(Input::kDOWN)) {ytemp += move_pixels; dir = DOWN;}

	return ((xtemp != x) || (ytemp != y));
}

void Player::onStep()
{
	int xtemp, ytemp;
	bool moved;

	switch (state)
	{
		case Normal:
			/* ********************** Normal ************************* */
			xtemp = x;
			ytemp = y;

			// Comprobamos entrada para mover al player

			moved = getNewPos(xtemp, ytemp);

			if (game->getInput()->key(Input::kLCTRL))
				collidable = false;
			else
				collidable = true;

			if (moved)
			{
				if (abs(xtemp - x) >= abs(ytemp - y))
					if ((xtemp - x) >= 0)
						dir = RIGHT;
					else
						dir = LEFT;
				else
					if ((ytemp - y) >= 0)
						dir = DOWN;
					else
						dir = UP;

				currentAction = aWalk;
			}
			else
			{
				currentAction = aStand;
			}

			if (place_free(x, ytemp))
			{    
				y = ytemp; 
			}
			else
			{   
				world->moveToContact(x,ytemp, this);
			}

			if (place_free(xtemp, y))
			{    
				x = xtemp; 
			}
			else
			{   
				world->moveToContact(xtemp,y, this); 
			}

			if (game->getInput()->key(Input::kA))
				playAnim(Slash);
			if (game->getInput()->key(Input::kS))
				playAnim(Thrust);

			break;
		case Animation:
			/* ********************** Animation ************************* */
			if (((SpriteMap*) graphic)->animFinished())
			{
				state = savedState;
			}
			break;
		case Damaged:
			/* ********************** Damaged ************************* */
			break;
		case Cutscene:
			/* ********************** Cutscene ************************* */
			break;
		case Dead:
			/* ********************** Dead ************************* */
			break;
	};
	
	// Graphic settings
	switch (state)
	{
	case Normal:
		graphic->setColor(Color::White);
		switch (currentAction)
		{
		case aStand:
			((SpriteMap*) graphic)->playAnim(getAnimName(Stand, dir));
			break;
		case aWalk:
			((SpriteMap*) graphic)->playAnim(getAnimName(Walk, dir));
			break;
		case aPush:
			graphic->setColor(Color::Green);
			break;
		}
		break;
	case Attack:
		graphic->setColor(Color::Red);
		break;
	case Animation:
		if (savedState == Attack)
			graphic->setColor(Color::Red);
		break;
	case Damaged:
		break;
	case Cutscene:
		break;
	case Dead:
		break;
	}
};

Dir Player::getDir()
{
	return dir;
}

std::string Player::getAnimName(PlayerAnim anim, Dir dir)
{
	if (dir == NONE) dir = this->dir;
	// Se obtiene el nombre de la animaci�n a partir del enum
	std::map<std::pair<PlayerAnim, Dir>, std::string>::iterator it;
	it = animList.find(make_pair(anim, dir));
	// Si el iterador alcanca el final de la lista, no est� la anim
	if (it == animList.end())
		return "";
	else
		return (*it).second;
};

bool Player::playAnim(PlayerAnim anim, Dir dir)
{
	if (dir == NONE)
		dir = this->dir;
	// Si la animaci�n no existe, seguro que no se puede
	std::string name = getAnimName(anim, dir);
	if (name == "")
		return false;

	// Si la animaci�n no tiene datos, algo va mal
	PlayerAnimData data = getAnimationData(anim, dir);
	if (data.numFrames < 0)
		return false;

	// 1. Comprobaci�n de estado actual: �permite manipulaci�n?
	if (state == Animation)
		return false;
	// 2. Almacenar estado, animaci�n y cosas actuales
	savedState = state;
	// 3. Establecer nueva animaci�n
	state = Animation;
	((SpriteMap*) graphic)->playAnim(name, data.animSpeed, false, false);

	return true;
};

// Intenta cambiar al player al estado que sea
bool Player::changeState(PlayerState next, bool forced)
{
	// Si estamos obligados, cambiamos el estado sin m�s
	if (forced)
		state = next;
	else
	{
		// Si no, comprobamos que se pueda cambiar
		// To be done
		state = next;
		return true;
	}

	return true;
};

std::string Player::getConfigurationFileName(std::string fname)
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

bool Player::loadAnimations(std::string fname)
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

	// 2. Leer todas las animaciones
	// Stand
	loadAnimation(Stand, UP, "stu", f);
	loadAnimation(Stand, DOWN, "std", f);
	loadAnimation(Stand, LEFT, "stl", f);
	loadAnimation(Stand, RIGHT, "str", f);

	// Walk
	loadAnimation(Walk, UP, "wau", f);
	loadAnimation(Walk, DOWN, "wad", f);
	loadAnimation(Walk, LEFT, "wal", f);
	loadAnimation(Walk, RIGHT, "war", f);

	// Slash
	loadAnimation(Slash, UP, "slu", f);
	loadAnimation(Slash, DOWN, "sld", f);
	loadAnimation(Slash, LEFT, "sll", f);
	loadAnimation(Slash, RIGHT, "slr", f);

	// Thrust
	loadAnimation(Thrust, UP, "thu", f);
	loadAnimation(Thrust, DOWN, "thd", f);
	loadAnimation(Thrust, LEFT, "thl", f);
	loadAnimation(Thrust, RIGHT, "thr", f);

	fclose(f);

	return true;
};

bool Player::loadAnimation(PlayerAnim anim, Dir direction, std::string name, FILE* from)
{
	// Cargar animaci�n indicada de from
	if (from == NULL || name == "")
		return false;

	int numFrames, speed;
	int* frameList;
	PlayerFrameData frameData;
	PlayerAnimData animData;

	// Leemos datos de la animaci�n

	// 0.Speed
	if (fscanf(from, "%d", &speed) < 1)
		return false;

	// 1.N� frames
	if (fscanf(from, "%d", &numFrames) < 1)
		return false;

	animData.animSpeed = speed;
	animData.numFrames = numFrames;

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

	// Se mappea la anim
	animList.insert(make_pair(make_pair(anim, direction), name));
	// Y se agrega su info
	animDataList.insert(make_pair(make_pair(anim, direction), animData));
	delete frameList;

	return true;
};

Player::PlayerFrameData Player::loadAnimationFrame(FILE* from)
{
	// Se carga un frame de from
	// Se prepara un frame vac�o
	PlayerFrameData fd;
	fd.frameId = 0;
	fd.walkMask.offsetX = 0;
	fd.walkMask.offsetY = 0;
	fd.walkMask.width = 0;
	fd.walkMask.height = 0;
	fd.collisionMask.offsetX = 0;
	fd.collisionMask.offsetY = 0;
	fd.collisionMask.width = 0;
	fd.collisionMask.height = 0;
	fd.hotspotX = 0;
	fd.hotspotY = 0;

	// El archivo debe ser v�lido
	if (from == NULL)
		return fd;

	// Se lee el frameId
	if (fscanf(from, "%d", &fd.frameId) < 1)
		return fd;

	// Y por ahora ya
	return fd;
};

Player::PlayerAnimData Player::getAnimationData(PlayerAnim anim, Dir dir)
{
	if (dir == NONE) dir = this->dir;
	// Se obtiene el nombre de la animaci�n a partir del enum
	std::map<std::pair<PlayerAnim, Dir>, PlayerAnimData>::iterator it;
	it = animDataList.find(make_pair(anim, dir));
	// Si el iterador alcanca el final de la lista, no est� la anim
	if (it == animDataList.end())
	{
		PlayerAnimData a; a.animSpeed = -1; a.numFrames = -1;
		return a;
	}
	else
		return (*it).second;
};

Player::PlayerState Player::getState()
{
	return state;
};