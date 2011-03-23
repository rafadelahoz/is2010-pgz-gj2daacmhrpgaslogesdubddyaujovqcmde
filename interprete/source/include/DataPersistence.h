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

public:
	DataPersistence();

	bool addMapData(int mapId, char type, int w, int h, const int** layout, int numPuzzles, int numDoors, int numMinibosses);

	GameData* getGameData();
	MapData* getMapData(int mapId);
};

#endif