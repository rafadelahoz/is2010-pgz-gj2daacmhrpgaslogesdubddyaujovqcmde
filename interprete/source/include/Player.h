#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Stamp.h"

class Player : public Entity 
{
public:
    Player(int x, int y, Game* game, GameState* world); // crea el objeto Player, puede haber m�s par�metros
    ~Player();

    // M�todos virtuales de la clase padre
    void onStep(); // Realiza el movimiento del player en funci�n del input

	/* Los m�todos vac�os no hace falta declararlos */
	// void onRender() {}; // (vac�o de momento, ya se ocupa entity)
	// void onCollision() {}; // Acci�n a realizar en caso de colisi�n (vac�a de momento)
};

#endif