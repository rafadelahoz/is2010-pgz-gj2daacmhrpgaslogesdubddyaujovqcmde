#pragma once
#ifndef __DUNGEONMAPSTATE_H__
#define __DUNGEONMAPSTATE_H__

#include "MapState.h"

class DungeonMapState : public MapState
{
	private:
		bool bossDefeated;	// Boss derrotado?
		bool gotBossKey;	// Llave de boss cogida?
		bool gotCompass;	// Comp�s cogido?
		bool gotMap;		// Mapa cogido?
		bool gotPowerUp;	// PowerUp cogido?

	public:

		//Instancia el DungeonMapState
		DungeonMapState();

		//!Instancia e inicializa el DungeonMapState
		DungeonMapState(std::map<int, bool> collectables,	std::map<int, bool> doors, std::map<int, bool> puzzles,	
						std::map<int, bool> minibosses, bool bossDefeated, bool gotBossKey,
						bool gotCompass, bool gotMap, bool gotPowerUp);

		//!Destruye el DungeonMapState
		~DungeonMapState();

		//!Inicializa los atributos de DungeonMapState
		/*!
			\param bossDefeated flag de Boss derrotado.
			\param gotBossKey flag de llave del Boss cogida.
			\param gotCompass flag de comp�s cogido.
			\param gotMap flad de mapa cogido.
			\param gotPowerUp flag de PowerUp cogido.
		*/
		void init(std::map<int, bool> collectables,	std::map<int, bool> doors, std::map<int, bool> puzzles,	
				  std::map<int, bool> minibosses, bool bossDefeated, bool gotBossKey, bool gotCompass, 
				  bool gotMap, bool gotPowerUp);

		//!Getter del flag de boss derrotado
		/*!
			\return Est� el boss derrotado o no
		*/
		bool isBossDefeated();

		//!Setter del flag de boss derrotado
		/*!
			\param bossDefeated nuevo flag que vamos a asignar
		*/
		void setBossDefeated(bool bossDefeated);

		//!Getter del flag de llave del boss
		/*!
			\return Est� cogida la llave del boss o no
		*/
		bool isGotBossKey();

		//!Setter del flag de llave del boss
		/*!
			\param gotBosskey nuevo flag que vamos a asignar
		*/
		void setGotBossKey(bool gotBossKey);

		//!Getter del flag del compas
		/*!
			\return Est� cogido el comp�s o no
		*/
		bool isGotCompass();

		//!Setter del flag del comp�s
		/*!
			\param gotBosskey nuevo flag que vamos a asignar
		*/
		void setGotCompass(bool gotCompass);

		//!Getter del flag del mapa
		/*!
			\return Est� el mapa cogido o no
		*/
		bool isGotMap();

		//!Setter del flag del mapa
		/*!
			\param gotBosskey nuevo flag que vamos a asignar
		*/
		void setGotMap(bool gotMap);

		//!Getter del flag del PowerUp
		/*!
			\return Est� cogido el PowerUp o no
		*/
		bool isGotPowerUp();

		//!Setter del flag del PowerUp
		/*!
			\param gotBosskey nuevo flag que vamos a asignar
		*/
		void isGotPowerUp(bool gotPowerUp);
};

#endif