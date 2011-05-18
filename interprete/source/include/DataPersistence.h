#pragma once
#ifndef __DATAPERSISTENCE_H__
#define __DATAPERSISTENCE_H__

#include <vector>

#include "GameData.h"
#include "MapData.h"

class DataPersistence
{
private:
	GameData* gameData;
	std::vector<MapData*> mapDataList;
	int numMapas;

public:
	DataPersistence();
	~DataPersistence();
	bool addMapData(int mapId, char type, int w, int h, const int** layout, int numPuzzles, int numDoors, int numMinibosses, int numCollectables);

	GameData* getGameData();
	MapData* getMapData(int mapId);
	int getMapNumber();

	void save(string path);
	bool load(FILE* f);

	int getCurrentHeartPieces();
	int getCurrentMoney();
	int getGameProgress();
	int getNumPigeons();
};

#endif