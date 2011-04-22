#include "GamePlayState.h"
#include "CollisionTester.h"
#include "PushableTester.h"
#include "GameItem.h"
#include "TiledEntity.h"
#include "NPC.h"
#include "FireBall.h"

GamePlayState::GamePlayState(int w, int h, Game* g) : GameState(g, w, h)
{
	localEntities = new list<Entity*>();

	offsetX = 0;
	offsetY = 16;

	hudController = NULL;
};

GamePlayState::~GamePlayState()
{
	// Ya se ocupa la destructora padre de borrarlas
	delete localEntities;
};

// Sobrecarga del m�todo padre para notificar al mapa de cambios
bool GamePlayState::_add(Entity* e)
{
	((ScreenMap*) map)->notify(ScreenMap::eCREATE, e);

	return GameState::_add(e);
}

// Sobrecarga del m�todo padre para notificar al mapa de cambios
bool GamePlayState::_remove(Entity* e)
{
	((ScreenMap*) map)->notify(ScreenMap::eDESTROY, e);
	localEntities->remove(e);

	return GameState::_remove(e);
}

// Sobrecarga del m�todo del padre para utilizar la lista de entidades locales
bool GamePlayState::add(Entity* e, bool local)
{
	if (e != NULL)
	{
		if (local)
			add_single(localEntities, e);

		return GameState::add(e);
	}

	return false;
};

bool GamePlayState::add(Entity* e)
{
	return GamePlayState::add(e, true);
};

bool GamePlayState::remove(Entity* e)
{
	if (e != NULL)
	{
		int oldsize = localEntities->size();
		localEntities->remove(e);
		GameState::remove(e);
		return oldsize > localEntities->size();
	}
	return false;
};

// Sobrecarga del m�todo del padre para utilizar la lista de eliminables
bool GamePlayState::removeLocals()
{
	list<Entity*>::iterator it = localEntities->begin();
	while (it != localEntities->end())
	{
        if ((*it) != NULL)
        {
			Entity* a = (*it);
			if (!(*it)->persistent)
			{
				localEntities->erase(it);
				_remove(a);
			}
        }
		it = localEntities->begin();
	}

	return (localEntities->size() == 0);
};

void GamePlayState::renderBG()
{
	// Se dibuja un rect�ngulo para pruebas de gr�ficos de forma temporal
	game->getGfxEngine()->renderRectangle(0, 0, roomw, roomh, Color(255, 20, 20));
};

void GamePlayState::renderFG()
{
	// Huds y tal
	if (offsetX == 0 && offsetY == 16)
	{
		game->getGfxEngine()->renderRectangle(0, 0, roomw, 16, Color(50, 50, 50));
		game->getGfxEngine()->renderRectangle(0, roomh-16, roomw, 16, Color(50, 50, 50));

		if (hudController != NULL)
			hudController->onRender();
	}
};

void GamePlayState::onStep()
{
	if (game->getInput()->keyPressed(Input::kN))
		add(new CollisionTester(14*16, rand()%12*16, game, this));

	if (game->getInput()->keyPressed(Input::kT))
		add(new PushableTester(64, 64, game, this));

	if (game->getInput()->keyPressed(Input::kI))
	{
		GameItem* it = new GameItem(16*(2+rand()%10), 16*(2+rand()%8), game, this);
		int n = rand()%4;
		if (n == 0)
			it->init("data/graphics/heart.png", GameItem::ieHP, 4);
		else if (n == 1)
		{
			it->init("data/graphics/heart.png", GameItem::ieMP, 7);
			it->graphic->setColor(Color::Blue);
		}
		else if (n == 2)
			it->init("data/graphics/rupee.png", GameItem::ieMONEY, 5);
		else
			it->init("data/graphics/key.png", GameItem::ieKEY, 1);

		add(it);
	}

	if (hudController != NULL)
		hudController->onStep();

	
	/*if (game->getInput()->keyPressed(Input::kH))
	{
		int xx = (2+rand()%10)*16, yy = (2+rand()%8)*16;
		TiledEntity* te = new TiledEntity(xx, yy, game, this);
		te->init(map->getTileset(), 2);
		add(te);
		te = new TiledEntity(xx, yy-16, game, this);
		te->init(map->getTileset(), 3, true);
		add(te);
	}*/
	
	if (game->getInput()->keyPressed(Input::kB)) {
		NPC* npc = new NPC(16*(2+rand()%10), 16*(2+rand()%8), game, this);
		npc->init("data/graphics/npc.png", 3, 4);
		add(npc);
	}

/*	if (game->getInput()->keyPressed(Input::kF)) {
		int d = rand()%9;
		add(new FireBall((Direction)d, 16*(2+rand()%10), 16*(2+rand()%8), game, this));
	}*/
};

void GamePlayState::onRender()
{
    // Si se desea, se pintar�n elementos por debajo de los dem�s
    renderBG();

    // Si hay mapa, se pinta
    if (map != NULL)
        map->render(offsetX,offsetY);

    list<Entity*>::iterator i;
    // Pintamos todas las entidades visibles
	if (renderable->size() > 0)
		for(i = renderable->begin(); i != renderable->end(); i++)
			if ((*i) != NULL)
			{
				(*i)->x += offsetX;
				(*i)->y += offsetY;
				(*i)->onRender();
				(*i)->x -= offsetX;
				(*i)->y -= offsetY;
			}

    // Si se desea, se pintar�n elementos por encima de los dem�s
    renderFG();
};

void GamePlayState::setOffset(int x, int y)
{
	offsetX = x;
	offsetY = y;
};

std::pair<int, int> GamePlayState::getOffset()
{
	return make_pair(offsetX, offsetY);
}