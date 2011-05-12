#include "Decidator.h"

Decidator::Decidator(DBManager* myDB, string path){
	db = myDB;
	// Abrimos el archivo de Decidator
	ifstream f(path,ifstream::in);
	/* Declaramos un array con suficiente tama�o para leer las l�neas */
	char cadena[500], aux[100];
	for (int i = 0; i < 100; i++){
		aux[i] = ' '; 
	}

	if (!f) {
		printf("No pudo leerse el archivo");
		exit(1);   // parar
	}

	/* Hacemos una primera lectura */
	f >> cadena;
	printf(cadena);
	strcpy(aux, cadena);
	while (!f.eof()){
		/* Leemos la siguiente l�nea */
		f >> cadena;	
		strcat(aux,cadena);
	}
	f.close();

	// DEBUG
	printf(aux);
	printf("\n");

	for (int i = 0; i < 100; i++){
		cadena[i] = ' ';
	}

	int i = 0; string s = "";
	vector<string>* datos = new vector<string>();

	while (i < strlen(aux)){
		if (aux[i] != ';'){
			s += aux[i];
		}
		else{
			datos->push_back(s);
			s = "";
		}
		i++;
	}

	fillDates(datos);
	checkNumKeyObj();
	checkNumSafeZones();
	checkNumZones();
	checkTeleportsOpt();
	checkEnemies();
	checkNumTools();
	checkNumDungeons();
	checkRatio();
	checkConsistency();
	completeDates();	
	numPigeons = 89;

	
}

void Decidator::fillDates(vector<string>* datos){
	string s = "";
	consistency = atoi(datos->back().c_str());
	datos->pop_back();

	/*dungeonsSizeColumn = atoi(datos->back().c_str());
	datos->pop_back();

	dungeonsSizeRow = atoi(datos->back().c_str());
	datos->pop_back();*/

	this->ratio = atoi(datos->back().c_str());
	datos->pop_back();

	this->numDungeons = atoi(datos->back().c_str());
	datos->pop_back();

	// ToolSet
	toolsSet = new vector<short>();
	for (int i = 0; i < strlen(datos->back().c_str()); i++){
		if (datos->back().c_str()[i] != ',')
			s += datos->back().c_str()[i];
		else{
			toolsSet->push_back(atoi(s.c_str()));
			s = "";
		}
	}
	toolsSet->push_back(atoi(s.c_str()));
	datos->pop_back();
	s = "";

	this->numTools = atoi(datos->back().c_str());
	datos->pop_back();

	// EnemiesSet
	enemiesSet = new vector<short>();
	for (int i = 0; i < strlen(datos->back().c_str()); i++){
		if (datos->back().c_str()[i] != ',')
			s += datos->back().c_str()[i];
		else{
			this->enemiesSet->push_back(atoi(s.c_str()));
			s = "";
		}
	}
	enemiesSet->push_back(atoi(s.c_str()));
	datos->pop_back();
	s = "";

	this->teleports = atoi(datos->back().c_str());
	datos->pop_back();

	// ZonesSet
	zonesSet = new vector<short>();
	for (int i = 0; i < strlen(datos->back().c_str()); i++){
		if (datos->back().c_str()[i] != ',')
			s += datos->back().c_str()[i];
		else{
			this->zonesSet->push_back(atoi(s.c_str()));
			s = "";
		}
	}
	zonesSet->push_back(atoi(s.c_str()));
	datos->pop_back();
	s = "";

	this->numZones = atoi(datos->back().c_str());
	datos->pop_back();

	this->numSafeZones = atoi(datos->back().c_str());
	datos->pop_back();

	this->worldSize = atoi(datos->back().c_str());
	datos->pop_back();

	/*this->worldSizeColumn = atoi(datos->back().c_str());
	datos->pop_back();

	this->worldSizeRow = atoi(datos->back().c_str());
	datos->pop_back();*/

	this->difficulty = atoi(datos->back().c_str());
	datos->pop_back();

	this->playerName = datos->back();
	datos->pop_back();

	this->player = atoi(datos->back().c_str());
	datos->pop_back();

	this->thematic = datos->back();
	datos->pop_back();

	this->worldName = datos->back();
	datos->pop_back();

	/*this->initialMoney = atoi(datos->back().c_str());
	datos->pop_back();*/

	this->numKeyObj = atoi(datos->back().c_str());
	datos->pop_back();

	/*this->numPieces = atoi(datos->back().c_str());
	datos->pop_back();

	this->initialMap = atoi(datos->back().c_str());
	datos->pop_back();

	this->initialMaxLife = atoi(datos->back().c_str());
	datos->pop_back();*/

	/* Ya no hace falta conservar datos porque se han leido */
	delete datos; datos = NULL;
}

void Decidator::checkNumKeyObj(){
	if (this->numKeyObj == -1){
		switch (difficulty){
			case 1:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 2:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 3:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 4:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			default:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
		}
	}
}

void Decidator::checkNumSafeZones(){
	if (this->numSafeZones == -1){
		switch (difficulty){
			case 1:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 2:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 3:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 4:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			default:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
		}	
	}
}

void Decidator::checkNumZones(){
	if (this->numZones == -1){
		switch (difficulty){
			case 1:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 2:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 3:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 4:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			default:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
		}	
	}
}

void Decidator::checkTeleportsOpt(){
	if (this->teleports == -1){
		switch (difficulty){
			case 1:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 2:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 3:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 4:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			default:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
		}	
	}
}

void Decidator::checkEnemies(){
	if (this->enemiesSet->front() == -1){
		switch (difficulty){
			case 1:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 2:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 3:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 4:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			default:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
		}	
	}
}

void Decidator::checkNumTools(){
	if (this->numTools == -1){
		switch (difficulty){
			case 1:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 2:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 3:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 4:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			default:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
		}	
	}
}

void Decidator::checkNumDungeons(){
	if (this->numDungeons == -1){
		switch (difficulty){
			case 1:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 2:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 3:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 4:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			default:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
		}	
	}
}

void Decidator::checkRatio(){
	if (this->ratio == -1){
		switch (difficulty){
			case 1:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 2:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 3:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 4:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			default:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
		}	
	}
}

void Decidator::checkConsistency(){
	if (this->consistency == -1){
		switch (difficulty){
			case 1:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 2:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 3:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			case 4:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
			default:
				if (worldSize < 50){ // Mundo peque�o
				
				}
				else if (worldSize >= 50 && worldSize < 200){ // Mundo mediano
				
				}
				else{ // Mundo peque�o
				
				}
				break;
		}	
	}
}

/* Toma las decisiones oportunas para completar los par�metros restantes */
void Decidator::completeDates(){
	/*
	Los par�metros siguientes se seleccionan en funci�n de la dificultad:
		- initMaxLife
		- numPieces
		- numKeyObj
		- initialMoney
	
	*/
	initialMap = 0;
	switch (this->difficulty){
		case 0: // f�cil
			initialMaxLife = 24; 
			numPieces = 3;
			numKeyObj = 4;
			initialMoney = 600;
			numEnemies = 2;
			break;
		case 1: // normal
			initialMaxLife = 20;
			numPieces = 4;
			numKeyObj = 5;
			initialMoney = 500;
			numEnemies = 3;
			break;
		case 2: // dif�cil
			initialMaxLife = 16;
			numPieces = 5;
			numKeyObj = 6;
			initialMoney = 400;
			numEnemies = 4;
			break;
		case 3: // muy dif�cil
			initialMaxLife = 12;
			numPieces = 6;
			numKeyObj = 7;
			initialMoney = 300;
			numEnemies = 5;
			break;
		case 4: // ca�tico
			initialMaxLife = 8;
			numPieces = 7;
			numKeyObj = 8;
			initialMoney = 200;
			numEnemies = 6;
			break;
		default: // para todo lo dem�s mastercard
			initialMaxLife = 4;
			numPieces = 8;
			numKeyObj = 9;
			initialMoney = 100;
			numEnemies = 7;
			break;
	}

	initialMap = 0;
	numMaps = 3;
}

ZoneInfo Decidator::getWorldGen(){
	//return db->getWorldGen(this->thematic);
	return db->getZone();
}

short Decidator::getDungeonGen(){
	// return db->getWorldGen(this->thematic);
	// Pongo esto para que no estalle
	return db->getDungeon("Zelda");
}

bool Decidator::save() {

	printMainInfo();

	// Abrimos el archivo de decidator
	char fname[100];
	sprintf(fname, "decidator", 0);
	FILE* file = fopen (fname, "w");
	// Guardamos la informaci�n de decidator para el int�rprete
	if (file != NULL) {
		short* buffer = new short[7];
		buffer[0] = initialMaxLife;
		buffer[1] = initialMap;
		buffer[2] = numMaps;
		buffer[3] = numPieces;
		buffer[4] = initialMoney;
		buffer[5] = numKeyObj;
		buffer[6] = toolsSet->size();
		fwrite(buffer, sizeof(short), 7, file);	
		delete buffer; buffer = NULL;

		// guardamos el conjunto de herramientas
		vector<short>::iterator it;
		buffer = new short[toolsSet->size()];
		int i = 0;
		for (it = toolsSet->begin(); it < toolsSet->end(); it++){
			buffer[i] = *it;
			i++;
		}
		fwrite(buffer, sizeof(short), toolsSet->size(), file);
		delete buffer; buffer = NULL;

		fclose(file);

		return true;
	}
	return false;
}

Decidator::~Decidator(){
	delete zonesSet; zonesSet = NULL;
	delete enemiesSet; enemiesSet = NULL;
	delete toolsSet; toolsSet = NULL;
}

/**------------------------- GETTERS & SETTERS ------------------------------------*/

short Decidator::getInitialMaxLife(){
	return this->initialMaxLife;
}

short Decidator::getInitialMap(){
	return this->initialMap;
}		
short Decidator::getNumMaps(){
	return this->numMaps;
}		
short Decidator::getNumPieces(){
	return this->numPieces;
}

short Decidator::getNumKeyObj(){
	return this->numKeyObj;
}

short Decidator::getInitialMoney(){
	return this->initialMoney;
}

string Decidator::getWorldName(){
	return this->worldName;
}

string Decidator::getThematic(){
	return this->thematic;
}

short Decidator::getPlayer(){
	return this->player;
}

string Decidator::getPlayerName(){
	return this->playerName;
}

short Decidator::getDifficulty(){
	return this->difficulty;
}

short Decidator::getWorldSize(){
	return this->worldSize;
}

/*short Decidator::getWorldSizeRow(){
	return this->worldSizeRow;
}

short Decidator::getWorldSizeColumn(){
	return this->worldSizeColumn;
}*/

short Decidator::getNumSafeZones(){
	return this->numSafeZones;
}

short Decidator::getNumZones(){
	return this->numZones;
}

vector<short>* Decidator::getZonesSet(){
	return this->zonesSet;
}

bool Decidator::getTeleports(){
	return this->teleports;
}

short Decidator::getNumEnemies(){
	return this->numEnemies;
}

vector<short>* Decidator::getEnemiesSet(){
	return this->enemiesSet;
}

short Decidator::getNumTools(){
	return this->numTools;
}

vector<short>* Decidator::getToolsSet(){
	return this->toolsSet;
}

short Decidator::getNumDungeons(){
	return this->numDungeons;
}

short Decidator::getRatio(){
	return this->ratio;
}

/*short Decidator::getDungeonsSizeRow(){
	return this->dungeonsSizeRow;
}

short Decidator::getDungeonsSizeColumn(){
	return this->dungeonsSizeColumn;
}*/

short Decidator::getConsistency(){
	return this->consistency;
}


void Decidator::printMainInfo()
{
	// Carga el archivo y se lee
	FILE* f = std::fopen("./data/maininfo", "w");

	// Si el archivo es inv�lido, no se puede hacer nada
	if (f == NULL)
		; // cry

	std::fprintf(f, "%d\n", numMaps);
	std::fprintf(f, "%d\n", numKeyObj);

	std::fprintf(f, "%d\n", initialMaxLife);

	std::fprintf(f, "%d\n", initialMoney);

	std::fprintf(f, "%d\n", numTools);

	for (vector<short>::iterator it = toolsSet->begin(); it != toolsSet->end(); it++)
		std::fprintf(f, "%d ", (*it));

	std::fprintf(f, "\n%d", numPigeons);

	std::fclose(f);
}