#pragma once

#ifndef __TOOLSWORD_H__
#define __TOOLSWORD_H__

#include "Tool.h"
#include "SpriteMap.h"
#include "HelperTypes.h"

class ToolSword : public Tool
{
private:
	//------------------------- Tipos auxiliares -----------------------------------------
	struct FrameData
	{
		int frameId;
		int hotspotX, hotspotY;
		int offsetX, offsetY, width, height; // datos de la m�scara
	};

	struct SwordAnimData
	{
		int animSpeed;
		unsigned char numFrames;
		Direction dir;
		std::vector<FrameData> frameData;
	};

	//------------------------- M�todos auxiliares ---------------------------------------

	// Devuelve el nombre del fichero de configuraci�n de una imagen
	string getConfigurationFileName(string fname);

	// Carga todas las animaciones de una imagen
	bool loadAnimations(string fname);

	// Carga una animaci�n de la espada
	bool loadAnimation(Direction dir, string name, FILE* from);

	// Coge el identificador de un frame del archivo de configuraci�n
	FrameData loadAnimationFrame(FILE* from);

	// Realiza la animaci�n seleccionada
	bool playAnim(std::string name);

	// Coloca la espada en el sitio indicado teniendo en cuenta el hotspot actual del player, y el frame actual de la animaci�n
	void placeSword();

	// ----------------------------- Atributos ------------------------------------
	std::map<std::string, SwordAnimData> animList;

public:
	ToolSword(int x, int y, Game* game, GameState* world);
	~ToolSword();

	void init(bool passive, Player* p, string graphicpath, int ncol, int nrow);
	void activate();

	bool animFinished();

	void onRender();
	void onStep();
	void onCollision();
	
};
#endif