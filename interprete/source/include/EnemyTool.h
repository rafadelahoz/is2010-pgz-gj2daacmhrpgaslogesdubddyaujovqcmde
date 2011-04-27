#pragma once

#ifndef __ENEMYTOOl_H__
#define __ENEMYTOOL_H__

#include "Enemy.h"

class EnemyTool: public GameEntity
{
protected:

	//------------------------- Tipos auxiliares -----------------------------------------

	struct FrameData
	{
		int frameId;
		int hotspotX, hotspotY;
		int offsetX, offsetY, width, height; // datos de la m�scara
	};

	struct EnemyToolAnimData
	{
		int animSpeed;
		unsigned char numFrames;
		Direction dir;
		std::vector<FrameData> frameData;
	};

	// --------------------------- Atributos --------------------------------------

	int idEnemyTool;		// Identificadr de la herramienta
	Enemy* enemy;		// Puntero al enemy
	std::map<std::string, EnemyToolAnimData> animList;  // mapa que guarda la informaci�n de cada animaci�n

	// --------------------- M�todos de auxiliares ----------------------------

	// Carga una animaci�n de la herramienta (si es una herramienta animada)
	bool loadAnimation(Direction dir, std::string name, FILE* from);

	// Coge el identificador de un frame del archivo de configuraci�n
	FrameData loadAnimationFrame(FILE* from);

	// Devuelve el nombre del fichero de configuraci�n de una imagen
	std::string getConfigurationFileName(std::string fname);

	// Realiza la animaci�n seleccionada
	bool playAnim(std::string name);

	/* Coloca la herramienta en el sitio indicado teniendo en cuenta el hotspot actual del enemy,
	y el frame actual de la animaci�n */
	void placeTool();

public:

	EnemyTool(int x, int y, Game* game, GameState* world);
	~EnemyTool();

	// inicia la herramienta
	void init(Enemy* e, int idTool);
	
	// Indica si ha acabado la animaci�n de la herramienta
	bool animFinished();

	// acci�n a realizar cuando se active la herramienta
	void activate();
	
};
#endif
