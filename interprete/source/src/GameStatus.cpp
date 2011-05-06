#include "GameStatus.h"

GameStatus::GameStatus()
{
}

GameStatus::~GameStatus()
{
};

//Inicializa los atributos de GameStatus haciendo copias de los mapas entrantes
void GameStatus::init(int numKeyItems, int maxLife, std::map<int,ToolInfo> tools, int actualMoney,
			MapLocation actualScreen, std::pair<int,int> lastPos, int numPlayers, int numPidgeons,
			int numHeartPieces, int barterProgress, int gameProgress)
{
	this->numKeyItems = numKeyItems;
	this->maxLife = maxLife;
	this->currentMoney = actualMoney;
	this->currentMapLocation = actualScreen;
	this->lastPlayerPosition = lastPos;
	this->numPlayers = numPlayers;
	this->numPidgeons = numPidgeons;
	this->currentHeartPieces = numHeartPieces;
	this->barterProgress = barterProgress;
	this->gameProgress = gameProgress;

	//Copiamos la tabla de tools
	std::map<int,ToolInfo>::iterator it;	//Iterador para recorrer la lista de collectables entrante
	std::pair<int,ToolInfo> aux;			//Par para almacenar los valores que vamos recorriendo

	for(it = tools.begin(); it != tools.end(); it++)
	{
		aux.first = (*it).first;			//Copiamos la clave a un par auxiliar
		aux.second = (*it).second;			//Copiamos el valor a un par auxiliar
		
		this->tools.insert(aux);

	}//end for
}

int GameStatus::getNumKeyItems()
{
	return numKeyItems;
}
		
void GameStatus::setNumKeyItems(int numKeyItems)
{
	this->numKeyItems = numKeyItems;
}

int GameStatus::getMaxLife()
{
	return maxLife;
}

void GameStatus::setMaxLife(int maxLife)
{
	this->maxLife = maxLife;
}

std::map<int, ToolInfo> GameStatus::getTools()
{
	return tools;
}

void GameStatus::setTools(std::map<int, ToolInfo> tools)
{
	std::map<int,ToolInfo>::iterator it;	//Iterador para recorrer la lista de collectables entrante
	std::pair<int,ToolInfo> aux;			//Par para almacenar los valores que vamos recorriendo

	//Borramos el mapa anterior
	this->tools.clear();

	for(it = tools.begin(); it != tools.end(); it++)
	{
		aux.first = (*it).first;			//Copiamos la clave a un par auxiliar
		aux.second = (*it).second;			//Copiamos el valor a un par auxiliar
		
		this->tools.insert(aux);

	}//end for
}

ToolInfo GameStatus::getToolInfo(int idTool)
{
	return tools[idTool];
}

//Se parte de la base que el map no admite repeticiones, sino faltar�a comprobar si ese elemento ya existe y realizar los computos pertinentes
void GameStatus::setTool(int idTool, bool available, int idAmmo, int ammoQuantity)
{
	std::pair<int,ToolInfo> aux;

	aux.second.available = available;
	aux.second.idAmmo = idAmmo;
	aux.second.ammoQuantity = ammoQuantity;

	tools.insert(aux);
}

bool GameStatus::isToolAvailable(int idTool)
{
	return tools[idTool].available;
}

void GameStatus::setToolAvailable(int idTool, bool available)
{
	std::pair<int,ToolInfo> aux;
	aux.second = tools[idTool];
	aux.second.available = available;

	tools.insert(aux);
}

int GameStatus::getToolAmmoId(int idTool)
{
	return tools[idTool].idAmmo;
}

int GameStatus::getToolAmmoQuantity(int idTool)
{
	return tools[idTool].ammoQuantity;
}

void GameStatus::setToolAmmo(int idTool, int idAmmo)
{
	std::pair<int,ToolInfo> aux;
	aux.second = tools[idTool];
	aux.second.idAmmo = idAmmo;

	tools.insert(aux);
}

//!Establece la cantidad de munici�n de una herramienta dada
/*!
	\param idTool identificador de la herramienta a actualizar
	\param ammoQuantity cantidad de munici�n que tendr� dicha herramienta
*/
void GameStatus::setToolAmmoQuantity(int idTool, int ammoQuantity)
{
	std::pair<int,ToolInfo> aux;
	aux.second = tools[idTool];
	aux.second.ammoQuantity = ammoQuantity;

	tools.insert(aux);
}

int GameStatus::getCurrentMoney()
{
	return currentMoney;
}

void GameStatus::setCurrentMoney(int actualMoney)
{
	this->currentMoney = actualMoney;
}

MapLocation GameStatus::getCurrentMapLocation()
{
	return currentMapLocation;
}

void GameStatus::setCurrentMapLocation(MapLocation actualScreen)
{
	this->currentMapLocation = actualScreen;
}

//Devolvera un par con la ultima posici�n del jugador en la pantalla actual
std::pair<int,int> GameStatus::getLastPlayerPosition()
{
	std::pair<int,int> aux;
	aux.first = currentMapLocation.screenX;
	aux.second = currentMapLocation.screenY;
	return aux;
}

void GameStatus::setLastPlayerPosition(std::pair<int,int> lastPos)
{
	lastPlayerPosition.first = lastPos.first;
	lastPlayerPosition.second = lastPos.second;
}

int GameStatus::getNumPlayers()
{
	return numPlayers;
}

void GameStatus::setNumPlayers(int numPlayers)
{
	this->numPlayers = numPlayers;
}

int GameStatus::getNumPidgeons()
{
	return numPidgeons;
}

void GameStatus::setNumPidgeons(int numPidgeons)
{
	this->numPidgeons = numPidgeons;
}

int GameStatus::getCurrentHeartPieces()
{
	return currentHeartPieces;
}
		
void GameStatus::setCurrentHeartPieces(int heartPieces)
{
	this->currentHeartPieces = heartPieces;
}

int GameStatus::getBarterProgress()
{
	return barterProgress;
}

void GameStatus::setBarterProgress(int barterProgress)
{
	this->barterProgress = barterProgress;
}

int GameStatus::getGameProgress()
{
	return gameProgress;
}

//!Setter de gameProgress
void GameStatus::setGameProgress(int gameProgress)
{
	this->gameProgress = gameProgress;
}

void GameStatus::save(FILE* f){
	int* buffer = new int[15];
	// Escribimos los datos
	// N�mero de llave
	buffer[0] = numKeyItems;
	// M�xima vida
	buffer[1] = maxLife;
	// Dinero actual
	buffer[2] = currentMoney;
	// N�mero de jugadores
	buffer[3] = numPlayers;
	// N�mero de pidgeons
	buffer[4] = numPidgeons;
	// N�mero de piezas de coraz�n actuales
	buffer[5] = currentHeartPieces;
	
	buffer[6] = barterProgress;
	// Progreso del juego
	buffer[7] = gameProgress;
	// �ltima posici�n del player
	buffer[8] = lastPlayerPosition.first;
	buffer[9] = lastPlayerPosition.second;

	// Datos del mapa donde se encuentra el player
	buffer[10] = currentMapLocation.id;
	buffer[11] = currentMapLocation.positionX;
	buffer[12] = currentMapLocation.positionY;
	buffer[13] = currentMapLocation.screenX;
	buffer[14] = currentMapLocation.screenY;

	fwrite(buffer, sizeof(int), 15, f);
	delete buffer; buffer = new int[1];
	// Datos de la tabla de herramientas
	// N�mero de herramientas
	buffer[0] = tools.size();
	fwrite(buffer, sizeof(int), 1, f);
	delete buffer; buffer = new int[4];
	bool* buffer1 = new bool[1];

	for (map<int,ToolInfo>::iterator it = tools.begin(); it != tools.end(); it++){
		/* key */
		buffer[0] = (*it).first;
		/* ToolInfo
					int idTool;
					int idAmmo;
					int ammoQuantity;
					bool available;
		*/
		buffer[1] = (*it).second.idTool;
		buffer[2] = (*it).second.idAmmo;
		buffer[3] = (*it).second.ammoQuantity;
		fwrite(buffer, sizeof(int), 4, f);
		buffer1[0] = (*it).second.available;
		fwrite(buffer, sizeof(bool), 1, f);
	}
	delete buffer; buffer = NULL;
	delete buffer1; buffer1 = NULL;
}