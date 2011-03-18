#include "ScreenMap.h"

ScreenMap::ScreenMap(int width, int height, int tileW, int tileH, int x, int y, GfxEngine* gfxEngine) : Map(tileW, tileH, gfxEngine)
{
	//Llamo a la constructora de Map con los parametros que me pasan
	//Pongo los atributos de la clase al valor que me dan
	this->width = width;
	this->height = height;
	this->x = x;
	this->y = y;
	this->entidades = NULL;
}

ScreenMap::~ScreenMap(){};

vector<CollisionPair>* ScreenMap::checkColision(Entity* e)
{
	//Cojo el solidGrid de mi Map, y le pido que me de el par de colision con la entidad que me han pasado
	return this->solidGrid->collide(e->mask);
}

bool ScreenMap::isInBounds(Entity* e)
{
	//Cojo el solidGrid de mi Map y le pregunto si la entidad se encuentra dentro del mismo
	return this->solidGrid->isMaskInbounds(e->mask);
}

void ScreenMap::setEntities(vector<Entity*>* entities)
{
	this->entidades = entities;
}

//Su funcion seria la de mantener actualizados los tiles animados, pero por ahora no hay
void ScreenMap::update(){};

//
void ScreenMap::render()
{
	//Avisa al tileMap de que se renderize
	this->tileMap->render(x,y);
}



Dir ScreenMap::relative_position(Entity* p)
{
	//Calculamos los vertices superior izquierdo e inferior derecho
	// V�rtice superior izquierdo
	int x1 = p->mask->x + p->mask->xoffset;
	int y1 = p->mask->y + p->mask->yoffset;
	// V�rtice inferior derecho
	int x2 = p->mask->x + p->mask->xoffset + p->mask->width;
	int y2 = p->mask->y + p->mask->yoffset + p->mask->height;

	if (x1 <= x) return LEFT;				// Se sale por la izquierda
	if (x2 >= x + width) return RIGHT;		// Se sale por la derecha
	if (y1 <= y) return UP;					// Se sale por la izquierda
	if (y2 <= y + height) return DOWN;		// Se sale por la izquierda
	return NONE;							// No se est� saliendo
}