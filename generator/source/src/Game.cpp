/*----------------------------------------------------------------------------------*/
//This code is part of a larger project whose main purpose is to entretain either	//
//by working on it and helping it be better or by playing it in it's actual state	//																			
//																					//
//Copyright (C) 2011 Three Legged Studio											//
//																					//
//    This program is free software; you can redistribute it and/or modify			//
//    it under the terms of the GNU General Public License as published by			//
//    the Free Software Foundation; either version 2, or (at your option)			//
//    any later version.															//
//																					//
//    This program is distributed in the hope that it will be useful,				//
//    but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the					//
//    GNU General Public License for more details.									//
//																					//
//    You should have received a copy of the GNU General Public License				//
//    along with this program; if not, write to the Free Software Foundation,		//
//    Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.			//
//																					//
//	  You can contact us at projectpgz.blogspot.com									//
/*----------------------------------------------------------------------------------*/

#include "Game.h"

Game::Game(){}

void Game::genGame(DBManager* myDB){
	/* ---- Decidator obtiene los datos para los generadores ---- */
	// la GUI guardar� el archivo que posteriormente leer� decidator para obtener la informaci�n
	decidator = new Decidator(myDB, "./file.txt");
	int wSize = decidator->getWorldSize();
	int numDungeons = decidator->getNumDungeons();
	int numZones = decidator->getNumZones();
	int diff = decidator->getDifficulty();
	int ratioDungeon = decidator->getRatio();
	int numSafeZones = decidator->getNumSafeZones();
	int numEnemies = decidator->getNumEnemies();

	zones = new vector<GenZone*>();
	ow = new Overworld(wSize, diff, numZones, numDungeons, numSafeZones);
	GenDungeon* genDungeon = new GenDungeon();
	Decorator* decorator = new Decorator();
	int numDungeon = 1; 
	int idTool = 1;   //params para la dungeon
	vector<SafeZoneInfo>* safeZones = NULL;//new vector<SafeZoneInfo>();
	string zoneTheme;
	for (int zoneNumber = 1; zoneNumber <= numZones; zoneNumber++)
	{
		switch(zoneNumber){ 
		case 1:{
				zoneTheme = "Forest";
				GenZone* myGenZone = new GenForestZone("theme-default", zoneTheme, zoneNumber, NULL, ow, numEnemies, genDungeon, /*numDungeon*/zoneNumber, idTool, ratioDungeon, safeZones, decorator, myDB);
				zones->push_back(myGenZone);
			break;}
		case 2:{
				zoneTheme = "Field";
				GenZone* myGenZone = new GenFieldZone("theme-default", zoneTheme, zoneNumber, NULL, ow, numEnemies, genDungeon, /*numDungeon*/zoneNumber, idTool, ratioDungeon, safeZones, decorator, myDB);
				zones->push_back(myGenZone);
			break;}
		case 3:{
				zoneTheme = "Swamp";  //pantano
				//GenZone* myGenZone = new GenWormZone("theme-default", zoneTheme, zoneNumber, NULL, ow, numEnemies, genDungeon, /*numDungeon*/zoneNumber, idTool, ratioDungeon, safeZones, decorator, myDB);
				GenZone* myGenZone = new GenLagoonZone("theme-default", zoneTheme, zoneNumber, NULL, ow, numEnemies, genDungeon, /*numDungeon*/zoneNumber, idTool, ratioDungeon, safeZones, decorator, myDB);
				zones->push_back(myGenZone);
			break;}
		default:{
				zoneTheme = "Desert";  
				//GenZone* myGenZone = new GenWormZone("theme-default", zoneTheme, zoneNumber, NULL, ow, numEnemies, genDungeon, /*numDungeon*/zoneNumber, idTool, ratioDungeon, safeZones, decorator, myDB);
				GenZone* myGenZone = new GenForestZone("theme-default", zoneTheme, zoneNumber, NULL, ow, numEnemies, genDungeon, /*numDungeon*/zoneNumber, idTool, ratioDungeon, safeZones, decorator, myDB);
				zones->push_back(myGenZone);
			break;}
		}
	}
	
	// Decidator obtiene de la base de dator el generador de mundo a utilizar
	/*switch (decidator->getWorldGen()){   Lo comento de momento porque la consulta no devuelve na y estalla esto x.x
	case 1:
		genOw = new GenVoroWorld(ow, zones, myDB);	
		break;
	}*/
	genOw = new GenVoroWorld(ow, zones, myDB);	

	world = new World(diff, genOw, myDB);

	world->buildOverworld();
	ow->save(); //ahora aqu� se hace el guardado
	for (int i = 0; i < genDungeon->getNumDungeons(); i++) //guardamos todas las dungeons
		genDungeon->getDungeon(i)->save();

	// Decidator guarda la informaci�n que necesita el int�rprete (como n�mero de piezas de coraz�n, etc...)
	decidator->save();

	delete genDungeon; 
	genDungeon = NULL;

	delete decorator;
	decorator = NULL;
}

Game::~Game(){

	vector<GenZone*>::iterator it;
    for(it = zones->begin(); it != zones->end(); it++)
        if ((*it) != NULL)
        {
			delete (*it);
			(*it) = NULL;
        }
	delete zones;
	zones = NULL;
	
	delete ow;			// SOLO LO BORRA ESTE PORQUE ES EL CLIENTE DICTADOR
	ow = NULL;

	delete genOw;		// As� nos aseguramos que llama al delete del GenVoronoiOW
	genOw = NULL;

	delete world; 
	world = NULL;

	delete decidator;
	decidator = NULL;
}
