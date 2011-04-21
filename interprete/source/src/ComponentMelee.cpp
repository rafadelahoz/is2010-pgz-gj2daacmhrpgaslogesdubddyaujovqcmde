#include "ComponentMelee.h"

#define IMG_WIDTH 16    // ancho de la imagen
#define IMG_HEIGHT 16    // alto de la imagen
#define IMG_PATH "data/graphics/coltest.png"


ComponentMelee::ComponentMelee(Game* game, Controller* cont) : Component()
{
	this->cont = cont;
	this->game = game;
}

void ComponentMelee::onCInit(Enemy* e)
{
	// Comenzamos en una direccion random y estado Normal
	dir = (Direction) ((rand() % 4) +1);
	state = Normal;
	dead = false;
	lastEnemyDirection = UP;  //FIXME ESTO LO ESTABLECERA EL ONDAMAGE

	// Inicializamos damageable
	//FIXME POR AQUI ENTRARA ENEMYDATA DESDE LA DB
	iDamageable::init(hpProv, hpProv, 1, 0xFF);

	// Creamos la m�scara
	e->mask = new MaskBox(e->x, e->y, IMG_WIDTH, IMG_HEIGHT, "meleeFucker", 0, 0); // offsets est�n a cero por defecto

	// Cambiamos la configuraci�n por defecto de los flags que nos interesan
	e->solid = false;

	// Establecemos el gr�fico del enemigo
	e->graphic = new Stamp(IMG_PATH, game->getGfxEngine());
}

void ComponentMelee::onCStep(Enemy* e)
{
	int xtemp, ytemp;
	Player* player;
	int chasePlayerid = 0;
	int chaseDirX, chaseDirY;

	switch (state)
	{
		case Normal:
			/* ********************** Normal ************************* */
			
			if (rand()%100 < turnRatio){
				dir = (Direction) ((rand() % 4) +1);
			}
			moveInDir(e, moveSpeed);

			// TODO mirar por delante a ver si vemos el player y cambia de estado
			for (int i= 0; i<cont->getNumPlayers(); i++){
				player = cont->getPlayer(i);
				if (checkPlayerNear(player, e, searchDist)){
					state = Chasing;
					e->setTimer(4, chaseTime); // Ponemos un timer para el tiempo que busca
					chasePlayerid = i;
				}
			}

			break;

		case Damaged:
			/* ********************** Damaged ************************* */
			xtemp = e->x; 
			ytemp = e->y;

			// Bounce del enemy
			if (lastEnemyDirection == UP) ytemp += e->getTimer(1)/2;
			else if (lastEnemyDirection == DOWN) ytemp -= e->getTimer(1)/2;
			else if (lastEnemyDirection == LEFT) xtemp += e->getTimer(1)/2;
			else if (lastEnemyDirection == RIGHT) xtemp -= e->getTimer(1)/2;
			else if (lastEnemyDirection == UPLEFT) ytemp += e->getTimer(1)/2, xtemp += e->getTimer(1)/2;
			else if (lastEnemyDirection == UPRIGHT) ytemp += e->getTimer(1)/2, xtemp -= e->getTimer(1)/2;
			else if (lastEnemyDirection == DOWNLEFT) ytemp -= e->getTimer(1)/2, xtemp += e->getTimer(1)/2;
			else if (lastEnemyDirection == DOWNRIGHT) ytemp -= e->getTimer(1)/2, xtemp -= e->getTimer(1)/2;

			// Actualizamos posici�n
			if (e->world->place_free(e->x, ytemp, e))
				e->y = ytemp; 
			else
				e->world->moveToContact(e->x,ytemp, e);

			if (e->world->place_free(xtemp, e->y, e))
				e->x = xtemp; 
			else
				e->world->moveToContact(xtemp,e->y, e);

			break;
		/* ********************** Attacking ************************* */
		case Attacking:
			
			currentAnim = Attack;
			
			break;
		/* ********************** Chasing ************************* */
		case Chasing:
			//dir = cont->getPlayer(chasePlayerid)->dir;
			chaseDirX = cont->getPlayer(chasePlayerid)->x - e->x;
			chaseDirY = cont->getPlayer(chasePlayerid)->y - e->y;
			
			if (abs(chaseDirX) - abs(chaseDirY) >= 0)
				// Tiene prioridad movimiento horizontal
				chaseDirX > 0 ? dir = RIGHT : dir = LEFT;
			else 
				// Tiene prioridad movimiento vertical
				chaseDirY > 0 ? dir = DOWN : dir = UP;
			
			moveInDir(e, moveSpeed);
			break;
		
		case Dead:
			/* ********************** Dead ************************* */
			break;

		case Animation:
			/* ********************** Animation ************************* */
			if (((SpriteMap*) e->graphic)->animFinished())
			{
				state = savedState;
			}
			break;
	};
	
	e->graphic->setColor(Color::White);
	e->graphic->setAlpha(1);

	// Graphic settings
	switch (state)
	{
	case Normal:
		switch (currentAction)
		{
		case aWalk:
			//((SpriteMap*) e->graphic)->playAnim(getAnimName(Walk, dir));
			currentAnim = Walk;
			break;
		}
		break;
	case Attacking:
		e->graphic->setColor(Color::Yellow);
		e->graphic->setAlpha(0.7f);
		break;
	case Damaged:
		e->graphic->setColor(Color::Red);
		e->graphic->setAlpha(0.7f);
		break;
	case Chasing:
		e->graphic->setColor(Color::Green);
		break;
	case Dead:
		e->graphic->setColor(Color(30, 30, 30));
		break;
	case Animation:
		if (savedState == Damaged)
			e->graphic->setColor(Color::Red);
		if (savedState == Attacking)
			e->graphic->setColor(Color::Yellow);
		break;
	}
}

void ComponentMelee::onCCollision(Enemy* enemy, CollisionPair other, Entity* e)
{
	if (other.b == "coltest")
	{
		enemy->instance_destroy();
	}

	else if (other.b == "player")
	{
		// Mover al player
		Direction d;
		int ocx, ocy, mcx, mcy, vunit, hunit;

		mcx = enemy->x+enemy->mask->xoffset;
		mcy = enemy->y+enemy->mask->yoffset;

		ocx = e->x+e->mask->xoffset+(e->mask->width/2);
		ocy = e->y+e->mask->yoffset+(e->mask->height/2);

		vunit = enemy->mask->height/3;
		hunit = enemy->mask->width/3;

		if (ocx < mcx+hunit)
		{
			if (ocy < mcy+vunit) d = DOWNRIGHT;
			else if (ocy >= mcy+vunit && ocy < mcy+vunit*2) d = RIGHT;
			else d = UPRIGHT;
		}
		else if (ocx >= mcx+hunit && ocx < mcx+hunit*2)
		{
			if (ocy < mcy+vunit) d = DOWN;
			else if (ocy >= mcy+vunit && ocy < mcy+vunit*2) d = NONE;
			else d = UP;
		}
		else
		{
			if (ocy < mcy+vunit) d = DOWNLEFT;
			else if (ocy >= mcy+vunit && ocy < mcy+vunit*2) d = LEFT;
			else d = UPLEFT;
		}

		((Player*) e)->setLastEnemyDirection(d);
		((Player*) e)->onDamage(5, 0x1);

		if (state != Attacking)
		{
			state = Attacking;
			enemy->setTimer(3,20); // esto sera en el futuro esperar a fin de animacion
		}
	}

	else if (other.b == "tool")
	{
		if (state != Damaged)
		{
			// Este da�o lo har� el arma que nos pega
			onDamage(5, 0x1);
			state = Damaged;
			enemy->setTimer(1, 10);
		}
	}
}

void ComponentMelee::onCRender(Enemy* e)
{
	e->Entity::onRender();
}

void ComponentMelee::onDeath()
{
	dead = true;
};

void ComponentMelee::onCTimer(Enemy* e, int timer)
{
	if (timer == 1)
	{
		if (state == Damaged)
			if (!dead)
				state = Normal;
			else {
				state = Dead;
				e->setTimer(2,15); // esto sera en el futuro esperar a fin de animacion
			}
	};

	if (timer == 2)
		e->instance_destroy();

	if (timer == 3 || timer == 4)
		state = Normal;
};

void ComponentMelee::playAnim(EnemyAnim anim, int speed, Direction dir)
{
};

bool ComponentMelee::checkPlayerNear(Player* p, Enemy* e, int dist)
{
	// Solo comprobamos si estamos mirando hacia el player nos ahorramos sqrt
	switch (dir)
	{
		case UP: 
			if (p->y <= e->y)
				return getDistance(e->x, e->y, p->x, p->y) < dist;
			break;
		case DOWN:
			if (p->y >= e->y)
				return getDistance(e->x, e->y, p->x, p->y) < dist;
			break;
		case LEFT:
			if (p->x <= e->y)
				return getDistance(e->x, e->y, p->x, p->y) < dist;
			break;
		case RIGHT:
			if (p->y >= e->y)
				return getDistance(e->x, e->y, p->x, p->y) < dist;
			break;
	}
	return false;
}

int ComponentMelee::getDistance(int x1, int y1, int x2, int y2)
{
	int sqr1, sqr2;
	sqr1 = (x2-x1)*(x2-x1);
	sqr2 = (y2-y1)*(y2-y1);
	return (int)sqrt((double)(sqr1+sqr2));
}

void ComponentMelee::moveInDir(Enemy* e, int speed){
	int xtemp = e->x;
	int ytemp = e->y;
	bool outOfScreen = true;
	Direction oldDir;

	// Miramos a ver si seguimos en territorio pantallil
	cont->getScreenMap()->relative_position(e,outOfScreen);
	
	// Y corregimos apropiadamente
	if (outOfScreen)
		if (dir == RIGHT){
			e->x -= speed;
			dir = LEFT;
		}
		else if(dir == LEFT){
			e->x += speed;
			dir = RIGHT;
		}
		else if(dir == UP){
			e->y += speed;
			dir = DOWN;
		}
		else if(dir == DOWN){
			e->y -= speed;
			dir = UP;
		}
	
	// Coord donde intentaremos movernos
	if (dir == RIGHT) xtemp += speed;
	if (dir == LEFT) xtemp -= speed;
	if (dir == UP) ytemp -= speed;
	if (dir == DOWN) ytemp += speed;

	// Nos intentamos mover (el orden da igual), probamos placeFree si falla hacemos moveToContact
	// Primero vertical
	e->world->place_free(e->x, ytemp, e) ? e->y = ytemp : e->world->moveToContact(e->x, ytemp, e);
	// Luego horizontal
	e->world->place_free(xtemp, e->y, e) ? e->x = xtemp : e->world->moveToContact(xtemp, e->y, e); 

}