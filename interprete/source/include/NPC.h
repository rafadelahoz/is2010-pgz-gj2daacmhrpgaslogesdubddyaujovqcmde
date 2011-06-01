/*----------------------------------------------------------------------------------*/
//This code is part of a larger project whose main purpose is to entretain either	//
//by working on it and helping it be better or by playing it in it's actual state	//																			
//																					//
//Copyright (C) 2011 Three Legged Studio											//
//																					//
//    This program is free software; you can redistribute it and/or modify			//
//    it under the terms of the GNU General Public License as published by			//
//    the Free Software Foundation; either version 1, or (at your option)			//
//    any later version.															//
//																					//
//    This program is distributed in the hope that it will be useful,				//
//    but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the					//
//    GNU General Public License for more details.									//
//																					//
//    You should have received a copy of the GNU General Public License				//
//    along with this program; if not, write to the Free Software Foundation,		//
//    Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA					//
//																					//
//	  You can contact us at projectpgz.blogspot.com									//
/*----------------------------------------------------------------------------------*/

#pragma once
#ifndef __NPC_H__
#define __NPC_H__

#include "Player.h"
#include "iInteractable.h"
#include "Controller.h"

class NPC : public GameEntity, public iInteractable {
	private:

		enum State {move, idle, interact};
		enum NPCAnim {Stand, Walk};
		enum Type {oldMan, crazy, young};

		Controller* controller;		/* Puntero a Controller */
		Direction dir;
		int ox, oy;
		int sp;
		State state;
		Type t;
		bool flag;					/* Determina si el NPC se encuentra en una conversaci�n o no */

		int idText;

		struct NPCMask {
			int offsetX; int offsetY;
			int width; int height;
		};

		struct NPCFrameData {
			int frameId;
			int speed;
			NPCMask walkMask;
			NPCMask collisionMask;
		};

		struct NPCAnimData {
			int animSpeed;
			unsigned char numFrames;
			std::vector<NPCFrameData> frameData;
		};

		// Animation translator from PlayerAnim to anim name
		std::map<std::pair<NPCAnim, Direction>, std::string> animList;
		std::string getAnimName(NPCAnim anim, Direction dir);

		// List of animation data
		std::map<std::pair<NPCAnim, Direction>, NPCAnimData> animDataList;
		NPCAnimData getAnimationData(NPCAnim anim, Direction dir = NONE);

		// --------------------- M�todos de auxiliares ----------------------------

		bool playAnim(NPCAnim anim, int speed = -1, Direction dir = NONE);

		/* Carga una animaci�n de la herramienta (si es una herramienta animada) */
		bool loadAnimation(NPCAnim anim,Direction dir, std::string name, FILE* from);

		/* Coge el identificador de un frame del archivo de configuraci�n */
		NPCFrameData loadAnimationFrame(FILE* from);

		/* Devuelve el nombre del fichero de configuraci�n de una imagen */
		std::string getConfigurationFileName(std::string fname);

		/* Carga todas las animaciones de una imagen */
		bool loadAnimations(string graphicpath, string fname);

	public:

		NPC(int x, int y, Game* game, GameState* world);
		~NPC();

		void init(string graphicpath,int type, int textId, Controller* c, Direction d = DOWN);
		bool animFinished();
		void onTimer(int timer);
		void onInteract(Player* p);
		void onEndInteract();
		void onStep();

};
#endif