#include "ComponentBatMovement.h"

// Esto tendra que recibirlo de algun lado
#define IMG_WIDTH 16    // ancho de la imagen
#define IMG_HEIGHT 16    // alto de la imagen

ComponentBatMovement::ComponentBatMovement(Game* game, Controller* cont): Component() {
	this->cont = cont;
	this->game = game;
}

ComponentBatMovement::~ComponentBatMovement() {

}

void ComponentBatMovement::onCInit(Enemy* e) {
	// Comenzamos en una direccion random y estado Normal
	e->dir = (Direction) ((rand() % 4) +1);
	state = savedState = Standing;
	resting = false;
	
	// Creamos la m�scara
	e->mask = new MaskBox(e->x, e->y, IMG_WIDTH, IMG_HEIGHT, "enemy", 0, 0);

	// Cambiamos la configuraci�n por defecto de los flags que nos interesan
	e->solid = false;

}

void ComponentBatMovement::onCStep(Enemy* e) {
	int xtemp, ytemp;
	Player* player;
	int chasePlayerId = 0;
	int chaseDirX, chaseDirY;
	int collDist;
	
	switch (state) {
		/* ********************** Standing ************************* */
		case Standing:
			if(!resting){
				e->setTimer(5, rand()%15 + 15);
				resting = true;
			}
			break;

		/* ********************** Walking ************************* */
		case Walking:
			if (rand()%100 < turnRatio){
				e->dir = getDifDir(e->dir);
			}
			moveInDir(e, moveSpeed);

			// Miramos en nuestra direccion a ver si vemos el player y cambiar al estado Chasing
			for (int i= 0; i<cont->getNumPlayers(); i++){
				player = cont->getPlayer(i);
				if (checkPlayerNear(player, e, searchDist)){
					state = Chasing;
					e->setTimer(4, chaseTime); // Ponemos un timer para el tiempo que busca
					chasePlayerId = i;
				}
			}
			break;

		/* ********************** Damaged ************************* */
		case ReceivingDamage:
			xtemp = e->x; 
			ytemp = e->y;

			// Bounce del enemy 
			if (e->getLastHitDirection() == UP) ytemp += e->getTimer(1)/2;
			else if (e->getLastHitDirection() == DOWN) ytemp -= e->getTimer(1)/2;
			else if (e->getLastHitDirection() == LEFT) xtemp += e->getTimer(1)/2;
			else if (e->getLastHitDirection() == RIGHT) xtemp -= e->getTimer(1)/2;
			else if (e->getLastHitDirection() == UPLEFT) ytemp += e->getTimer(1)/2, xtemp += e->getTimer(1)/2;
			else if (e->getLastHitDirection() == UPRIGHT) ytemp += e->getTimer(1)/2, xtemp -= e->getTimer(1)/2;
			else if (e->getLastHitDirection() == DOWNLEFT) ytemp -= e->getTimer(1)/2, xtemp += e->getTimer(1)/2;
			else if (e->getLastHitDirection() == DOWNRIGHT) ytemp -= e->getTimer(1)/2, xtemp -= e->getTimer(1)/2;

			// Actualizamos posici�n
			//if (e->world->place_free(e->x, ytemp, e))
				e->y = ytemp; 
			//else
				//e->world->moveToContact(e->x,ytemp, e);

			//if (e->world->place_free(xtemp, e->y, e))
				e->x = xtemp; 
			//else
				//e->world->moveToContact(xtemp,e->y, e);
			if (e->dead)
				state = Dying;

			break;

		/* ********************** Chasing ************************* */
		case Chasing:
			player = cont->getPlayer(chasePlayerId);
			chaseDirX = player->x - e->x;
			chaseDirY = player->y - e->y;
			
			if (abs(chaseDirX) - abs(chaseDirY) >= 0)
				// Tiene prioridad movimiento horizontal
				chaseDirX > 0 ? e->dir = RIGHT : e->dir = LEFT;
			else 
				// Tiene prioridad movimiento vertical
				chaseDirY > 0 ? e->dir = DOWN : e->dir = UP;
			
			// Nos movemos en esa direccion
			moveInDir(e, moveSpeed);

		/* ********************** Dead ************************* */
		case Dying:
			break;
	}
	
	e->graphic->setColor(Color::White);
	e->graphic->setAlpha(1);

	// Graphic settings
	switch (state) 	{
		case Standing:
			e->currentAnim = STAND;
			savedState = Standing;
			break;
		case Walking:
			e->currentAnim = WALK;
			savedState = Walking;
			break;
		case ReceivingDamage:
			e->currentAnim = DAMAGED;
			savedState = ReceivingDamage;
			break;
		case Chasing:
			e->currentAnim = WALK;
			e->graphic->setColor(Color::Green);
			savedState = Chasing;
			break;
		case Dying:
			e->currentAnim = DEAD;
			e->graphic->setAlpha(0.8f);
			savedState = Dying;
			break;
		}
}

void ComponentBatMovement::onCCollision(Enemy* enemy, CollisionPair other, Entity* e)
{
	if (other.b == "coltest") {
		enemy->instance_destroy();
	}

	else if (other.b == "player") {
		// Mover al player
		Direction d;
		int ocx, ocy, mcx, mcy, vunit, hunit;

		mcx = enemy->x+enemy->mask->xoffset;
		mcy = enemy->y+enemy->mask->yoffset;

		ocx = e->x+e->mask->xoffset+(e->mask->width/2);
		ocy = e->y+e->mask->yoffset+(e->mask->height/2);

		vunit = enemy->mask->height/3;
		hunit = enemy->mask->width/3;

		if (ocx < mcx+hunit) {
			if (ocy < mcy+vunit) d = DOWNRIGHT;
			else if (ocy >= mcy+vunit && ocy < mcy+vunit*2) d = RIGHT;
			else d = UPRIGHT;
		}
		else if (ocx >= mcx+hunit && ocx < mcx+hunit*2)	{
			if (ocy < mcy+vunit) d = DOWN;
			else if (ocy >= mcy+vunit && ocy < mcy+vunit*2) d = NONE;
			else d = UP;
		}
		else {
			if (ocy < mcy+vunit) d = DOWNLEFT;
			else if (ocy >= mcy+vunit && ocy < mcy+vunit*2) d = LEFT;
			else d = UPLEFT;
		}

		((Player*) e)->setLastEnemyDirection(d);
		((Player*) e)->onDamage(5, 0x1);
	}

	else if (other.b == "tool")	{
		if (state != ReceivingDamage) {
			// Este da�o lo har� el arma que nos pega
			state = ReceivingDamage;
			enemy->setTimer(1, 10);
		}
	}
	else if (other.b == "enemy") {
		switch(enemy->dir) {
			case UP:
				enemy->y += moveSpeed;
				break;
			case DOWN:
				enemy->y -= moveSpeed;
				break;
			case LEFT:
				enemy->x += moveSpeed;
				break;
			case RIGHT:
				enemy->x -= moveSpeed;
				break;
		}
	}
}

void ComponentBatMovement::onCTimer(Enemy* e, int timer) {
	// timer de bounce al ser damageado
	if (timer == 1)	{
		if (state == ReceivingDamage)
			if (!e->dead)
				state = Standing;
		if (state == Dying)
				e->setTimer(2,15); // esto sera en el futuro esperar a fin de animacion
	}

	// timer de desaparecer
	if (timer == 2)
		e->instance_destroy();

	// timer de la animacion al colisionar con player
	if (timer == 3)
		state = Standing;

	// timer de estar persiguiendo
	if (timer == 4)
		if (state == Chasing)
			state = Standing;

	// timer de estar walking
	if (timer == 5){
		state = Walking;
		e->setTimer(6, rand()%25 + 15);		
	}

	// timer de estar standing
	if (timer == 6){
		state = Standing;
		resting = false;
	}
}

bool ComponentBatMovement::checkPlayerNear(Player* p, Enemy* e, int dist) {
	// Solo comprobamos si estamos mirando hacia el player nos ahorramos sqrt
	switch (e->dir) {
		// Le sumo mask height|width para el ancho y el alto del player
		case UP: 
			if (p->y - p->mask->height <= e->y)
				return getDistance(e->x, e->y, p->x, p->y) < dist;
			break;
		case DOWN:
			if (p->y + p->mask->height >= e->y)
				return getDistance(e->x, e->y, p->x, p->y) < dist;
			break;
		case LEFT:
			if (p->x - p->mask->width <= e->x)
				return getDistance(e->x, e->y, p->x, p->y) < dist;
			break;
		case RIGHT:
			if (p->x + p->mask->height >= e->x)
				return getDistance(e->x, e->y, p->x, p->y) < dist;
			break;
	}
	return false;
};

int ComponentBatMovement::getDistance(int x1, int y1, int x2, int y2) {
	int sqr1, sqr2;
	sqr1 = (x2-x1)*(x2-x1);
	sqr2 = (y2-y1)*(y2-y1);
	return (int)sqrt((double)(sqr1+sqr2));
}

// Mueve al enemigo en la direccion en la que este mirando, devuelve si se choc� con algo
bool ComponentBatMovement::moveInDir(Enemy* e, int speed){
	int xtemp = e->x;
	int ytemp = e->y;
	bool outOfScreen = true, collided = false;

	// Miramos a ver si seguimos en territorio pantallil
	cont->getScreenMap()->relative_position(e,outOfScreen);
	
	// Y corregimos apropiadamente
	if (outOfScreen)
		if (e->dir == RIGHT){
			e->x -= speed;
			e->dir = LEFT;
		}
		else if(e->dir == LEFT){
			e->x += speed;
			e->dir = RIGHT;
		}
		else if(e->dir == UP){
			e->y += speed;
			e->dir = DOWN;
		}
		else if(e->dir == DOWN){
			e->y -= speed;
			e->dir = UP;
		}
	
	// Coord donde intentaremos movernos
	if (e->dir == RIGHT) xtemp += speed;
	if (e->dir == LEFT) xtemp -= speed;
	if (e->dir == UP) ytemp -= speed;
	if (e->dir == DOWN) ytemp += speed;

	// Nos intentamos mover (el orden da igual), probamos placeFree si falla hacemos moveToContact
	// Primero vertical
	//e->world->place_free(e->x, ytemp, e) ? 
	e->y = ytemp;// : e->world->moveToContact(e->x, ytemp, e), collided = true;
	// Luego horizontal
	//e->world->place_free(xtemp, e->y, e) ? 
	e->x = xtemp;// : e->world->moveToContact(xtemp, e->y, e), collided = true; 
	
	return collided;
}

Direction ComponentBatMovement::getDifDir(Direction direc){
	Direction newDir = (Direction) ((rand() % 4) +1);
	while (direc == newDir)
		newDir = (Direction) ((rand() % 4) +1);
	return newDir;
}