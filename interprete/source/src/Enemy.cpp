#include "Enemy.h"
#include "GameItem.h"

Enemy::Enemy(Game* game, GameState* world):GameEntity(-1,-1,game,world), iDamageable()
{
	// POSSIBLE DEBRIS
	currentAnim = NOTHING;
	cAnim = NULL;

	arena = NULL;
}

Enemy::~Enemy()
{
	for (vector<Component*>::iterator it = components->begin(); it != components->end(); it++) 
	{
		delete (*it);
		(*it) = NULL;
	}
	delete components;
	components = NULL;

	if (cAnim != NULL)
		delete cAnim;
	cAnim = NULL;
};

void Enemy::init(EnemySpawnData spawnData, vector<Component*>* components, ComponentAnim* cAnim, int hpMax, int mpMax, int strength, int defense, iNotificable* toNoty){
	
	this->spawnData.x = this->x = spawnData.x;
	this->spawnData.y = this->y = spawnData.y;
	this->spawnData.id = spawnData.id;
		
	this->maxHp = hpMax;
	this->mpMax = mpMax;
	this->strength = strength;
	this->defense = defense;

	this->components = components;

	iDamageable::init(maxHp, maxHp, defense, typeWeakness);
	dead = false;
	blinking = false;

	// POSSIBLE DEBRIS
	this->cAnim = cAnim;
	toNotify = toNoty;
};

void Enemy::onInit()
{
	if (cAnim != NULL)
		cAnim->onCInit();
	for (vector<Component*>::iterator it = components->begin(); it != components->end(); ++it) 
	{
		(*it)->onCInit(this);
	}
};

void Enemy::onStep()
{
	if (GameEntity::isPaused())
		return;

	if (cAnim != NULL)
		cAnim->onCStep();

	if (blinking && graphic != NULL)
	{
		graphic->setAlpha((getTimer(9) % 2));
	}

	for (vector<Component*>::iterator it = components->begin(); it != components->end(); ++it) 
	{
		(*it)->onCStep(this);
	}
};

void Enemy::onRender()
{
	/* Render de la sombra en la posici�n correcta */
	Graphic* g = graphic;
	graphic = NULL;
	if (visible && enabled)
		GameEntity::onRender();
	graphic = g;

	/* Render del gr�fico elevado si hace falta */
	if (cAnim != NULL)
		cAnim->onCRender();

	/* Dejamos que los componentes lo gocen */
	for (vector<Component*>::iterator it = components->begin(); it != components->end(); ++it) 
	{
		(*it)->onCRender(this);
	}
};

void Enemy::onTimer(int timer)
{
	cAnim->onCTimer(timer);

	if (timer == 9)
	{
		blinking = false;
		graphic->setAlpha(1.f);
	}
	else
	{
		for (vector<Component*>::iterator it = components->begin(); it != components->end(); ++it) 
		{
			(*it)->onCTimer(this, timer);
		}
	}
};

void Enemy::onCollision(CollisionPair other, Entity* e)
{
	for (vector<Component*>::iterator it = components->begin(); it != components->end(); ++it) 
	{
		(*it)->onCCollision(this, other, e);
	}
};

void Enemy::onDestroy()
{
	if (dead)
	{
		if (rand()%20 > 1)
		{
			DataBaseInterface* dbi = ((PGZGame*) game)->controller->getDataBaseInterface();
			GameItem* gi = new GameItem(x, y, game, world);
			world->add(gi);
			DataBaseInterface::ItemData idata = dbi->getItemData(rand()%dbi->getItemNumber());
			gi->init(dbi->getImagePath(idata.gfxId), (GameItem::ItemType) idata.effect, idata.power);
		}
	}

	for (vector<Component*>::iterator it = components->begin(); it != components->end(); ++it) 
	{
		(*it)->onCDestroy(this);
	}
};

void Enemy::onCustomEvent(int event)
{
	for (vector<Component*>::iterator it = components->begin(); it != components->end(); ++it) 
	{
		(*it)->onCCustomEvent(this, event);
	}
};

void Enemy::onInitStep()
{
	for (vector<Component*>::iterator it = components->begin(); it != components->end(); ++it) 
	{
		(*it)->onCInitStep(this);
	}
};

void Enemy::onEndStep()
{
	depth = y-cAnim->getHeight();

	for (vector<Component*>::iterator it = components->begin(); it != components->end(); ++it) 
	{
		(*it)->onCEndStep(this);
	}
};

void Enemy::onEndWorld()
{
	for (vector<Component*>::iterator it = components->begin(); it != components->end(); ++it) 
	{
		(*it)->onCEndWorld(this);
	}
};

std::vector<Component*>* Enemy::getComponents()
{
	return components;
};

void Enemy::damagePlayer(Player* p, int damage, short damageType)
{
	p->setLastHitDirection(p->computeHitDirection(this, p));
	p->onDamage(damage, damageType);
};

void Enemy::onDamage(int damage, short damageType)
{
	iDamageable::onDamage(damage, damageType);
	blinking = true;
	setTimer(9, 30);
}
void Enemy::onHeal(int healthPoints)
{
	iDamageable::onHeal(healthPoints);
}

void Enemy::onDeath()
{
	dead = true;
	instance_destroy();
}



/*
void Enemy::setLastDmgDirection(Direction dir)
{
	lastEnemyDirection = dir;
};

Direction Enemy::getLastDmgDirection()
{
	return lastEnemyDirection;
};
*/

/*
OLD SHIT

void Enemy::onDamage(int damage, short damageType)
{
	iDamageable::onDamage(damage, damageType);
};

void Enemy::onDeath()
{
	dead = true;
};
*/