#pragma once

#ifndef __COMPONENTMELEE_H__
#define __COMPONENTMELEE_H__

#include "Enemy.h"
#include "Stamp.h"
#include "HelperTypes.h"
#include "iDamageable.h"
#include <cmath>

class Enemy;

class ComponentMelee : public Component, public iDamageable
{
	private:
		Direction dir;
		Direction lastEnemyDirection;
		enum EnemyState { Normal, Chasing, Attacking, Damaged, Dead, Animation };
		enum EnemyAction { aWalk, aAttack, aDamaged };
		enum EnemyAnim { Walk, Attack, Hit };
		EnemyState state, savedState;
		EnemyAction currentAction;
		EnemyAnim currentAnim;

		void playAnim(EnemyAnim anim, int speed, Direction dir);
		bool checkPlayerNear(Player* p, Enemy* e, int dist);
		int getDistance(int x1, int y1, int x2, int y2);
		void moveInDir(Enemy* e, int speed);
		
		bool dead;
		//TODO Estos podremos cambiarlos en el CInit
		static const int turnRatio = 5;
		static const int searchDist = 55;
		static const int chaseTime = 30;
		static const int moveSpeed = 2; //ovejita que te pillo

		//HP PROVISIONAL
		static const int hpProv = 20;

	public:
		ComponentMelee(Game* game, Controller* cont );
		~ComponentMelee(){};
		
		void onCInit(Enemy* e);
		void onCStep(Enemy* e);
		void onCRender(Enemy* e);
		virtual void onCTimer(Enemy* e, int timer);
		virtual void onCCollision(Enemy* enemy, CollisionPair other, Entity* e);
		virtual void onCDestroy(Enemy* e){};
		virtual void onCCustomEvent(Enemy* e, int event){};
		virtual void onCInitStep(Enemy* e){};
		virtual void onCEndStep(Enemy* e){};
		virtual void onCEndWorld(Enemy* e){};

		void onDeath();
};
#endif __COMPONENTMELEE_H__
