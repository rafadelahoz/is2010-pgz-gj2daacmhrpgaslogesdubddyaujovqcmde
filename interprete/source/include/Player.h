#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "GameEntity.h"
#include "SpriteMap.h"
#include "HelperTypes.h"

class Player : public GameEntity 
{

private:

	// info correspondiente a las tools equipdas ( <idtool, idtool>, de forma abstracta ).
	//

public:

	int hp, mp;

	// crea el objeto Player, puede haber m�s par�metros
	Player(int x, int y, Game* game, GameState* world); 
    ~Player();


	bool init(std::string gfxpath, int ncol, int nrow, int hp, int mp);

    // M�todos virtuales de la clase padre
    void onStep(); // Realiza el movimiento del player en funci�n del input


    // M�todos propios
	Dir getDir();

};

#endif