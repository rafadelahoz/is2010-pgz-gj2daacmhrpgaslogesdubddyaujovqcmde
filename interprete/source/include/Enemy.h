#pragma once

#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "GameEntity.h"
#include "ComponentAnim.h"
#include "EnemyTool.h"
#include "Component.h"
#include "iNotificable.h"

#include <vector>

class EnemyTool;
class ComponentAnim;
class Component;

class Enemy : public GameEntity, public iDamageable
{
	public:
		enum EnemyDifficulty {EASY, NORMAL, DIFFICULT, HARD};

	private:
		vector<Component*>* components;
		iNotificable* toNotify;

	public:

		// STATIC INFO
			// Personal info
			EnemySpawnData spawnData;
			// General info (from DB)
			// maxHp from iDamageable
			// defense from iDamageable
			// typeWeakness from iDamageable
			int mpMax;
			int strength;
			EnemyDifficulty difficulty;

		// DYNAMIC INFO
			// hp from iDamageable
			int mp;
			Direction dir;
			bool dead;

		ComponentAnim* cAnim;
		StandardEnemyAnimation currentAnim;

		void init(EnemySpawnData, vector<Component*>* components, ComponentAnim* cAnim, int hpMax, int mpMax, int strength, int defense, iNotificable* toNotify = NULL);

		void damagePlayer(Player* p, int damage, short damageType);
		std::vector<Component*>* getComponents();

		friend class Component;
		friend class EnemyTool;

		Enemy(Game* game, GameState* world);
		virtual ~Enemy();
		virtual void onInit();
		virtual void onStep();
		virtual void onRender();
		virtual void onTimer(int timer);
		virtual void onCollision(CollisionPair other, Entity* e);
		virtual void onDestroy();
		virtual void onCustomEvent(int event);
		virtual void onInitStep();
		virtual void onEndStep();
		virtual void onEndWorld();


		void onDamage(int damage, short damageType);
		void onHeal(int healthPoints);
		void onDeath();
};

#endif __ENEMY_H__
