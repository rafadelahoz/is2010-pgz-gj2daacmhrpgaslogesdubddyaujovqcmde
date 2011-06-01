/*----------------------------------------------------------------------------------*/
//This code is part of a larger project whose main purpose is to entretain either	//
//or by playing it in its actual state.												//
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

#include "GenGame.h"

GenGame::GenGame(){}

void GenGame::genGame(DBManager* myDB){
	// Se prepara el directorio de salida
	outputPath = ".\\";//..\\..\\interprete\\bin\\";

	string command = "mkdir ";
	// Creamos el dir ppal
	command.append(outputPath);
	if (system(NULL)) system(command.c_str());
	// Dir de datos
	command = "mkdir " + outputPath + DATA_PATH;
	if (system(NULL)) system(command.c_str());
	// Dir de mapas
	command.append(MAPS_PATH);
	if (system(NULL)) system(command.c_str());
	// Dir de recursos
	command = "mkdir " + outputPath + DATA_PATH;
	command.append(RES_PATH);
	if (system(NULL)) system(command.c_str());

	/* ---- Decidator obtiene los datos para los generadores ---- */
	// la GUI guardar� el archivo que posteriormente leer� decidator para obtener la informaci�n
	decidator = new Decidator(myDB, "./input.dat");
	int wSize = decidator->getWorldSize();
	int numDungeons = decidator->getNumDungeons();
	//int numZones = decidator->getNumZones();
	int numZones = 4;
	int diff = decidator->getDifficulty();
	int ratioDungeon = decidator->getRatio();
	int numSafeZones = decidator->getNumSafeZones();
	//LO QUITO PORQUE 5 ES BASTANTE BASTANTE!
	int numEnemies = 3; //decidator->getNumEnemies();
	int numPigeons = decidator->getNumPigeons();
	int numBigHearts = decidator->getNumBigHearts();

	/* Pedimos cosas a la DB */
	myDB->getPlayer();
	myDB->getKey();
	myDB->getBossKey();
	myDB->getDoors();
	short keyObj = myDB->getKeyObj();
	//pillamos 25 items para que pueda tenerlos el juego XD!
	myDB->getPowUp();
	for (int i = 0; i < 25; i++)
		myDB->getItem();

	// Debug code sexy sexy
	decidator->clearInitialTools();
	for (int i = 0; i < decidator->getNumTools(); i++)
	{
		int tries = 100;
		bool ok = false;
		while (!ok && tries > 0)
		{
			int idTool = myDB->getTool();
			if (!toolSelected(idTool))
			{
				decidator->addInitialTool(idTool);
				addTool(idTool);
				ok = true;
			}
			tries--;
		}
	}

	zones = new vector<GenZone*>();

	//int numHearts = decidator->getInitialMaxLife();
	ow = new Overworld(wSize, diff, numZones, numDungeons, numSafeZones, numBigHearts, numPigeons);
	GenDungeon* genDungeon = new GenDungeon();
	Decorator* decorator = new WorldDecorator(myDB);
	int numDungeon = 1; 
	int idTool = 1;   //params para la dungeon
	vector<SafeZoneInfo>* safeZones = NULL;//new vector<SafeZoneInfo>();
	ZoneInfo zInfo;
	int zoneNumber = 1;
	string lastzone = "";
	while((int)zones->size() < numZones)
	{
		while(strcmp(zInfo.gen.c_str(),lastzone.c_str()) == 0)
			zInfo = myDB->getZone();

		int idTool;
		bool ok = false;
		int tries = 100;
		while (!ok && tries > 0)
		{
			idTool = myDB->getTool();
			if (!toolSelected(idTool))
			{
				addTool(idTool);
				ok = true;
			}
			tries--;
		}

		if(strcmp(zInfo.gen.c_str(), "Forest") == 0)
		{ 
				GenZone* myGenZone = new GenForestZone(zInfo.gen, zoneNumber, zInfo.tileSetId, ow, numEnemies, /*dificultad*/diff, genDungeon, numDungeon, idTool, ratioDungeon, safeZones, decorator, myDB, keyObj);
				zoneNumber++;
				zones->push_back(myGenZone);
		}
		else if(strcmp(zInfo.gen.c_str(), "Prairie") == 0)
		{
				GenZone* myGenZone = new GenFieldZone(zInfo.gen, zoneNumber, zInfo.tileSetId, ow, numEnemies,diff,  genDungeon, numDungeon, idTool, ratioDungeon, safeZones, decorator, myDB, keyObj);
				zoneNumber++;
				zones->push_back(myGenZone);
		}
		else if(strcmp(zInfo.gen.c_str(), "Lake") == 0)
		{
				//GenZone* myGenZone = new GenWormZone("theme-default", zoneTheme, zoneNumber, NULL, ow, numEnemies, genDungeon, /*numDungeon*/zoneNumber, idTool, ratioDungeon, safeZones, decorator, myDB);
				GenZone* myGenZone = new GenLagoonZone(zInfo.gen, zoneNumber, zInfo.tileSetId, ow, numEnemies, diff, genDungeon, numDungeon, idTool, ratioDungeon, safeZones, decorator, myDB, keyObj);
				zoneNumber++;
				zones->push_back(myGenZone);
		}
		else if(strcmp(zInfo.gen.c_str(), "Desert") == 0)
		{
				//GenZone* myGenZone = new GenWormZone("theme-default", zoneTheme, zoneNumber, NULL, ow, numEnemies, genDungeon, /*numDungeon*/zoneNumber, idTool, ratioDungeon, safeZones, decorator, myDB);
				GenZone* myGenZone = new GenDesertZone(zInfo.gen, zoneNumber, zInfo.tileSetId, ow, numEnemies, diff, genDungeon, numDungeon, idTool, ratioDungeon, safeZones, decorator, myDB, keyObj);
				zoneNumber++;
				zones->push_back(myGenZone);
		}
		lastzone = zInfo.gen;
		numDungeon++;
	}
	
	// Decidator obtiene de la base de dator el generador de mundo a utilizar
	/*switch (decidator->getWorldGen()){   Lo comento de momento porque la consulta no devuelve na y estalla esto x.x
	case 1:
		genOw = new GenVoroWorld(ow, zones, myDB);	
		break;
	}*/
	genOw = new GenVoroWorld(ow, zones,genDungeon, decorator, myDB);	

	world = new World(diff, genOw, myDB);

	world->buildOverworld();
	ow->save(outputPath); //ahora aqu� se hace el guardado
	for (int i = 0; i < genDungeon->getNumDungeons(); i++) //guardamos todas las dungeons
		genDungeon->getDungeon(i)->save(outputPath);

	// mazmorra final no se cuenta
	numDungeon++;

	decidator->setNumMaps(numDungeon);
	// Decidator guarda la informaci�n que necesita el int�rprete (como n�mero de piezas de coraz�n, etc...)
	decidator->save();

	// De momento copia prologo y textos aunque lo har�n los decidator historiator y dem�s
	outputPath = ".\\data\\";
	command = "copy " + outputPath + "prologue.txt " + DATA_PATH;
	if (system(NULL)) system(command.c_str()); 
	command = "copy " + outputPath + "textos.txt " + DATA_PATH;
	if (system(NULL)) system(command.c_str());
	// Copia configs
	command = "copy " + outputPath + "config-p1 " + DATA_PATH;
	if (system(NULL)) system(command.c_str());
	command = "copy " + outputPath + "config-p1-template " + DATA_PATH;
	if (system(NULL)) system(command.c_str());
	command = "copy " + outputPath + "config-p1-joy " + DATA_PATH;
	if (system(NULL)) system(command.c_str());
	// Copia maininfo 
	command = "copy " + outputPath + "maininfo "+ DATA_PATH;
	if (system(NULL)) system(command.c_str());

	delete genDungeon; 
	genDungeon = NULL;

	delete decorator;
	decorator = NULL;
}

GenGame::~GenGame(){

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

void GenGame::addTool(int id)
{
	selectedTools.insert(id);
}
bool GenGame::toolSelected(int id)
{
	return selectedTools.find(id) != selectedTools.end();
}