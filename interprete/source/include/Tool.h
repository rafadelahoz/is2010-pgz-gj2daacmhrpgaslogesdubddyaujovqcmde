#pragma once

#ifndef __TOOl_H__
#define __TOOL_H__

#include "Player.h"
#include "GameEntity.h"

class Tool: public GameEntity
{
protected:

	//------------------------- Tipos auxiliares -----------------------------------------

	struct FrameData
	{
		int frameId;
		int hotspotX, hotspotY;
		int offsetX, offsetY, width, height; // datos de la m�scara
	};

	struct ToolAnimData
	{
		int animSpeed;
		unsigned char numFrames;
		Direction dir;
		std::vector<FrameData> frameData;
	};

	// --------------------------- Atributos --------------------------------------

	bool passive;	// true: herramienta pasiva, false: herramienta activa
	Player* player;		// Puntero al player
	std::map<std::string, ToolAnimData> animList;  // mapa que guarda la informaci�n de cada animaci�n

	// --------------------- M�todos de auxiliares ----------------------------

	// Carga una animaci�n de la herramienta (si es una herramienta animada)
	bool loadAnimation(Direction dir, std::string name, FILE* from);

	// Coge el identificador de un frame del archivo de configuraci�n
	FrameData loadAnimationFrame(FILE* from);

	// Devuelve el nombre del fichero de configuraci�n de una imagen
	std::string getConfigurationFileName(std::string fname);

	// Realiza la animaci�n seleccionada
	bool playAnim(std::string name);

	/* Coloca la herramienta en el sitio indicado teniendo en cuenta el hotspot actual del player,
	y el frame actual de la animaci�n */
	void placeTool();

public:

	Tool(int x, int y, Game* game, GameState* world);
	~Tool();

	// inicia la herramienta
	void init(bool passive, Player* p);

	// getter de si la herramienta es pasiva o no
	bool isPassive();

	// Indica si ha acabado la animaci�n de la herramienta
	bool animFinished();

	// acci�n a realizar cuando se active la herramienta
	virtual void activate() = 0;
	
};
#endif
