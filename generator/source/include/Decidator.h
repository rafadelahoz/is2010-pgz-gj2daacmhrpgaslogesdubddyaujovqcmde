#pragma once

#ifndef _DECIDATOR_H_
#define _DECIDATOR_H_

#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <time.h>

using namespace std;

class Decidator{
	private:

		short initialMaxLife;		// vida inicial del player (dificultad) 4 de vida -> 1 coraz�n
		short initialMap;			// Id del mapa en el que comienza al player al comienzo del juego
		short numMaps;				// N�mero de mapas del juego
		short numPieces;			//Num. Piezas coraz�n: n�mero de piezas para conseguir un coraz�n (dificultad).
		short numKeyObj;			//Num. Obj. Clave: n�mero de objetos clave al inicio del juego (dificultad).
		short initialMoney;			//Dinero (dificultad)
		string worldName;			// Nombre del juego
		short thematic;				// Tem�tica del juego
		short player;				// Identificador del player seleccionado
		string playerName;			// Nombre del player
		short difficulty;			// Dificultad del juego
		short worldSize;			// Tama�o del mundo
		//short worldSizeRow;			// Tama�o del mundo alto
		//short worldSizeColumn;		// Tama�o del mundo ancho
		short numZones;				// N�mero de zonas
		vector<short>* zonesSet;	// Conjunto de ids de zonas seleccionadas
		bool teleports;				// si hay teletransporte o no.
		vector<short>* enemiesSet;	// Conjunto de ids de enemigos seleccionados
		short numTools;				// N�mero de herramientas
		vector<short>* toolsSet;	// Conjunto de herramientas seleccionadas
		short numDungeons;			// N�mero de herramientas
		short ratio;				//ratio puzzle/action (seguramente no se tenga en cuenta)
		short dungeonsSizeRow;		// Tama�o de las mazmorras alto
		short dungeonsSizeColumn;	// Tama�o de las mazmorras ancho
		short consistency;			// consistencia (seguramente no se tenga en cuenta)

		void fillDates(vector<string>* datos);
		void completeDates();

	public:
		Decidator(string path);
		~Decidator();

		bool save();

		/* Getters */
		short getInitialMaxLife();		
		short getInitialMap();			
		short getNumMaps();				
		short getNumPieces();			
		short getNumKeyObj();			
		short getInitialMoney();			
		string getWorldName();			
		short getThematic();				
		short getPlayer();				
		string getPlayerName();			
		short getDifficulty();
		short getWorldSize();
		/*short getWorldSizeRow();
		short getWorldSizeColumn();		*/
		short getNumZones();				
		vector<short>* getZonesSet();	
		bool getTeleports();				
		vector<short>* getEnemiesSet();	
		short getNumTools();				
		vector<short>* getToolsSet();	
		short getNumDungeons();			
		short getRatio();				
		short getDungeonsSizeRow();		
		short getDungeonsSizeColumn();	
		short getConsistency();	
};

#endif