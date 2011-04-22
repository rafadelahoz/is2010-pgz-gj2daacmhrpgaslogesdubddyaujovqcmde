#pragma once
#ifndef __NPC_H__
#define __NPC_H__

#include "Player.h"
#include "iInteractable.h"


class NPC : public GameEntity, public iInteractable {
	private:

		enum State {move, idle};
		enum NPCAnim {Stand, Walk};

		Direction dir;
		int ox, oy;
		int sp;
		State state;

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
		bool loadAnimations(std::string fname);

	public:

		NPC(int x, int y, Game* game, GameState* world);
		~NPC();

		void init(string graphicpath, int ncol, int nrow);
		bool animFinished();
		void onTimer(int timer);
		void onInteract(Player* p);
		void onStep();

};
#endif