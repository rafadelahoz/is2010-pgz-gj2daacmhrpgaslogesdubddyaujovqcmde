#include "WorldAutoTiler.h"

WorldAutoTiler::WorldAutoTiler(std::string tileSetPath){

	FILE* f;
	f = loadWorldConfig(loadTilesetConfig(tileSetPath));
	fclose(f);
}

WorldAutoTiler::~WorldAutoTiler(){
	AutoTiler::~AutoTiler();
}

FILE* WorldAutoTiler::loadWorldConfig(FILE* file)
{
	// comrprobamos que existe el archivo
	if (file == NULL)
		return NULL;

	// n� Decoraciones
	int nDeco;
	if (fscanf_s(file, "%d", &nDeco) < 1)
		return NULL;

	// Cargamos los campos auxiliares de cada decoraci�n
	for (int i = 0; i < nDeco; i++)
		if (!loadWorldDeco(file))
			return NULL;

}

bool WorldAutoTiler::loadWorldDeco(FILE* file)
{
	// comrprobamos que existe el archivo
	if (file == NULL)
		return NULL;

	// idDecoraci�n, proximidad
	int idDeco, near;
	if (fscanf_s(file, "%d %d", &idDeco, &near) < 2)
		return NULL;

	// a�andimos los campos a la decoraci�n
	decorationList.at(idDeco).near = (Decoration::DecorationNear) near;
}