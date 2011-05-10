#pragma once

#ifndef __COMPONENTANIM_H__
#define __COMPONENTANIM_H__

#include "Controller.h"
#include "SpriteMap.h"
#include "HelperTypes.h"

class Enemy;

class ComponentAnim
{
	protected:
		std::string gfxPath;
		Game* game;
		
		// Altura del elemento
		int height;

	private:

		struct EnemyMask
		{
			int offsetX; int offsetY;
			int width; int height;
		};

		struct EnemyFrameData
		{
			int frameId;
			EnemyMask collisionMask;
			int hotspotX, hotspotY;
		};

		struct EnemyAnimData
		{
			int animSpeed;
			unsigned char numFrames;
			std::vector<EnemyFrameData> frameData;
		};

		// Animation translator from StandarEnemyAnimation to anim name
		std::map<std::pair<StandardEnemyAnimation, Direction>, std::string> animList;
		std::string getAnimName(StandardEnemyAnimation anim, Direction dir);

		// List of animation data
		std::map<std::pair<StandardEnemyAnimation, Direction>, EnemyAnimData> animDataList;
		EnemyAnimData getAnimationData(StandardEnemyAnimation anim, Direction dir = NONE);

		// Carga las animaciones a partir del archivo de cfg
		bool loadAnimations(std::string fname);
		bool loadAnimation(StandardEnemyAnimation anim, Direction direction, std::string name, FILE* from);
		EnemyFrameData loadAnimationFrame(FILE* from);
		std::string getConfigurationFileName(std::string fname);

		Enemy* e;
		
	public:
		
		friend class EnemyTool;

		// Hace al enemy reproducir la animaci�n indicada
		// Cuando termine volver� al estado anterior al comienzo
		// Devuelve true si el enemy acepta ser manipulado
		bool playAnim(StandardEnemyAnimation anim, int speed = -1, Direction dir = NONE);
		
		ComponentAnim(Game* game, Enemy* e, std::string gfxPath);
		~ComponentAnim();
		
		void onCStep();
		void onCRender();

		void setHeight(int h);
		int getHeight();
		void setShadow(int width);
};
#endif __COMPONENTANIM_H__