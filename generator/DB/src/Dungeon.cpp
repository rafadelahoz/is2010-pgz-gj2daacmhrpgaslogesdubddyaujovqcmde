#include "Dungeon.h"

Dungeon::Dungeon(string zone, string theme, short gameDiff, short dungNumber, short ratio, short tool, DBManager* db) {
    // Instancia el vector de Screens
	screenList = new vector<Screen*>();
	this->zone = zone;
	this->tool = tool;
	this->db = db;
	srand(time(NULL));
}

Dungeon::~Dungeon() {
    delete screenList; screenList = NULL;
}

bool Dungeon::save() {
	// Abrimos el archivo de mazmorra m<ID>h
	char fname[MAX_STR_LENGTH];
	sprintf(fname, "map\\m%dh", 0);
	FILE* file = fopen (fname, "w");
	// Guardamos la informaci�n de la mazmorra (ahora mismo no me s� el orden)
	if (file != NULL) {
		// Guardamos la informaci�n de la mazmorra
		short* buffer = new short[2];
		buffer[0] = width;
		buffer[1] = height;
		fwrite(buffer, sizeof(buffer), 1, file);	// ancho y alto de la mazmorra en pantallas
		delete buffer; buffer = NULL;

		// layout
		// inicializamos el layout a 0
		bool** layout = new bool*[width];
		for (int i = 0; i < width; i++) {
			layout[i] = new bool[height];
			for (int j = 0; j < height; j++)
				layout[i][j] = false;
		}

		// comprobamos qu� pantallas est�n ocupadas
		vector<Screen*>::iterator it;
		for (it = screenList->begin(); it < screenList->end(); it++)
			layout[(*it)->getPosX()][(*it)->getPosY()] = true;

		// guardamos el layout
		for (int i = 0; i < width; i++)
			fwrite(layout[i], sizeof(layout[i]), 1, file);

        // nos deshacemos de la memoria que hemos usado para guardar el layout
		for (int i = 0; i < width; i++) {
			delete layout[i];
			layout[i] = NULL;
		}
		delete layout; layout = NULL;

		// guardamos la pantalla inicial de la mazmorra
		buffer = new short[2];
		buffer[0] = iniX;
		buffer[2] = iniY;
		fwrite(buffer, sizeof(buffer), 1, file);

		// informaci�n general de la mazmorra
		buffer = new short[4];
		buffer[0] = n_puzzles;
		buffer[1] = n_collectables;
		buffer[2] = n_puertas;
		buffer[3] = n_minibosses;
		fwrite(buffer, sizeof(buffer), 1, file);

		fclose(file);

		// informaci�n de las pantallas
		for (it = screenList->begin(); it < screenList->end(); it++)
			(*it)->save();
		return true;
	}
	return false;
}
