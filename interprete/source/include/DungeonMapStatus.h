#ifndef _DUNGEONMAPSTATE_H
#define _DUNGEONMAPSTATE_H

#include "MapStatus.h"

class DungeonMapStatus : public MapStatus 
{
private:
	// Boss derrotado?
	bool bossDefeated;

	// Llave de boss cogida?
	bool gotBossKey;

	// Comp�s cogido?
	bool gotCompass;

	// Mapa cogido?
	bool gotMap;

	// PowerUp cogido?
	bool gotPowerUp;


public:
	//Instancia el DungeonMapStatus
	DungeonMapStatus();

	//!Destruye el DungeonMapStatus
	~DungeonMapStatus();

	//!Inicializa los atributos de DungeonMapStatus
	//!
	//			\param bossDefeated flag de Boss derrotado.
	//			\param gotBossKey flag de llave del Boss cogida.
	//			\param gotCompass flag de comp�s cogido.
	//			\param gotMap flad de mapa cogido.
	//			\param gotPowerUp flag de PowerUp cogido.
	void init(/*std::map<int, bool> collectables, std::map<int, bool> doors, std::map<int, bool> puzzles, std::map<int, bool> minibosses, bool bossDefeated, bool gotBossKey, bool gotCompass, bool gotMap, bool gotPowerUp*/);

	//!Getter del flag de boss derrotado
	//!
	//			\return Est� el boss derrotado o no
	bool isBossDefeated();

	//!Setter del flag de boss derrotado
	//!
	//			\param bossDefeated nuevo flag que vamos a asignar
	void setBossDefeated(bool bossDefeated);

	//!Getter del flag de llave del boss
	//!
	//			\return Est� cogida la llave del boss o no
	bool isBossKeyGot();

	//!Setter del flag de llave del boss
	//!
	//			\param gotBosskey nuevo flag que vamos a asignar
	void setBossKeyGot(bool gotBossKey);

	//!Getter del flag del compas
	//!
	//			\return Est� cogido el comp�s o no
	bool isCompassGot();

	//!Setter del flag del comp�s
	//!
	//			\param gotBosskey nuevo flag que vamos a asignar
	void setCompassGot(bool gotCompass);

	//!Getter del flag del mapa
	//!
	//			\return Est� el mapa cogido o no
	bool isMapGot();

	//!Setter del flag del mapa
	//!
	//			\param gotBosskey nuevo flag que vamos a asignar
	void setMapGot(bool gotMap);

	//!Getter del flag del powerup
	//!
	//			\return Est� el powerup cogido o no
	bool isPowerUpGot();

	//!Setter del flag del powerup
	//!
	//			\param gotPowerUp nuevo flag que vamos a asignar
	void setPowerUpGot(bool gotPowerUp);

	void save(FILE* f);

};

#endif
