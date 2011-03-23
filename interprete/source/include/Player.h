#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "GameEntity.h"
#include "Stamp.h"
#include "HelperTypes.h"

class Player : public GameEntity 
{

private:

	// info correspondiente a las tools equipdas ( <idtool, idtool>, de forma abstracta ).
	// 

public:
	// crea el objeto Player, puede haber m�s par�metros
	Player(int x, int y, Game* game, GameState* world, std::string gfxpath, int hp, int mp); 
    ~Player();

    // M�todos virtuales de la clase padre
    void onStep(); // Realiza el movimiento del player en funci�n del input


    // M�todos propios
	Dir getDir();

};

#endif