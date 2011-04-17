#pragma once

#ifndef _DUNGEONM_H_
#define _DUNGEONM_H_

#include "Dungeon.h"
#include "DunScreen.h"

class DungeonM : public Dungeon {
    private:
        short n_areas;                  // N�mero de �reas en las que se divide la mazmorra
        DunScreen*** layout;            // Matriz de pantallas de la mazmorra, para trabajar con mayor comodidad
        vector<DunScreen*>** areas;     // Array de vectores de pantallas, que representa a qu� zona pertenece cada pantalla
		int** room_areas;				// Matriz que indica, para cada habitaci�n, la zona de la mazmorra a la que pertenece

        void calculate_size();          // Seg�n la dificultad y el n�mero de la mazmorra a generar, calcula el tama�o del layout y el n�mero de zonas
        void create_rooms();            // Instancia las habitaciones de la mazmorra, la matriz layout
        void divide_into_areas();       // Establece �reas aisladas en la mazmorra (rectangulares)
        void allocate_goodies();        // Coloca boss, minibosses, llaves, llave del boss y herramienta de la mazmorra
        void connect_rooms(short area); // Conecta las habitaciones de una determinada �rea
        void connect_areas();           // Conecta las zonas de la mazmorra, colocando los bloqueos oportunos entre ellas
        // Distribuye las llaves y la llave del jefe por la mazmorra
        void allocate_keys(short boss_area);
        // Intenta hacer una puerta que conecte dos habitaciones de la misma zona
        DunScreen* make_door(DunScreen* s, int* connected, int* current_room, int dir_or, short area);
		// Comprueba si se puede ir de una habitaci�n a otra dentro de la misma �rea
		bool check_room(short x1, short y1, short x2, short y2, short area, short room);
		// A�ade las habitaciones que quedan aisladas a las zonas ya existentes de la mazmorra
		void add_isolated_rooms();
		// A�ade una habitaci�n aislada a una determinada zona de la mazmorra
		bool add_room_to_area(short x, short y, short area);

    public:
        DungeonM(string zone, string theme, short gameDiff, short dungNumber, short ratio, short tool, DBManager* db);
        ~DungeonM();

        void generate();

		void print_dungeon();	// DEBUG
};

#endif
