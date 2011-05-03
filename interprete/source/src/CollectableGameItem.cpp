#include "CollectableGameItem.h"

#include "Player.h"

CollectableGameItem::CollectableGameItem(int x, int y, Game* g, GameState* gs) : GameItem(x, y, g, gs)
{
};

void CollectableGameItem::init(int idCollectable, MapStatus* mapstatus, std::string gfxPath, ItemType t, int power, Controller* c, string n)
{
	name = n;
	controller = c;
	myMapStatus = mapstatus;
	this->idCollectable = idCollectable;
	GameItem::init(gfxPath, t, power);
};

void CollectableGameItem::onInit()
{
	if (myMapStatus->getCollectableStatus(idCollectable))
		setVisible(false), instance_destroy();
};

void CollectableGameItem::applyEffect(Entity* target)
{
	// Una vez actuado, ya nunca m�s
	if (idCollectable >= 0)
		myMapStatus->setCollectableStatus(idCollectable, true);
	// Se hace al player reproducir animaci�n de oh yeah!
	((Player*) target)->playGetItem(graphic, 45);
	this->controller->getMessageController()->showItemMessage(name);
	// Pas�ndole el gr�fico y quit�ndonoloslo para que lo borre player
	graphic = NULL;
	// Y se aplica el efecto
	GameItem::applyEffect(target);
};