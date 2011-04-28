#include "DoorOpenClose.h"


DoorOpenClose::DoorOpenClose(Game* g, GameState* gs) : DoorOpener(g, gs)
{
}


DoorOpenClose::~DoorOpenClose()
{
}

void DoorOpenClose::onUnsolve()
{
	// Se a�aden todas las entidades
	for (std::list<Door*>::iterator it = toOpen.begin(); it != toOpen.end(); it++)
	{
		if ((*it) != NULL)
			(*it)->close();
	}
}

void DoorOpenClose::onSolve()
{
	// Se a�aden todas las entidades
	for (std::list<Door*>::iterator it = toOpen.begin(); it != toOpen.end(); it++)
	{
		if ((*it) != NULL)
			(*it)->open();
	}
}
