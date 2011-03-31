#pragma once

#ifndef _DUNGEONM_H_
#define _DUNGEONM_H_

#include "Dungeon.h"
#include "Screen.h"

class DungeonM : public Dungeon {
    private:
        short n_areas;                  // N�mero de �reas en las que se divide la mazmorra
        Screen*** layout;               // Matriz de pantallas de la mazmorra, para trabajar con mayor comodidad
        vector<Screen*>** areas;         // Array de vectores de pantallas, que representa a qu� zona pertenece cada pantalla

        void calculate_size();          // Seg�n la dificultad y el n�mero de la mazmorra a generar, calcula el tama�o del layout y el n�mero de zonas
        void create_rooms();            // Instancia las habitaciones de la mazmorra, la matriz layout
        void divide_into_areas();       // Establece �reas aisladas en la mazmorra (rectangulares)
        void allocate_goodies();        // Coloca boss, minibosses, llaves, llave del boss y herramienta de la mazmorra
        void connect_rooms(short area); // Conecta las habitaciones de una determinada �rea
        void connect_areas();           // Conecta las zonas de la mazmorra, colocando los bloqueos oportunos entre ellas
        // Distribuye las llaves y la llave del jefe por la mazmorra
        void allocate_keys(short boss_area);
        // Intenta hacer una puerta que conecte dos habitaciones de la misma zona
        Screen* make_door(Screen* s, int* connected, int* current_room, int dir_or, short area);
        // Intenta colocar un bloqueo entre dos habitaciones de zonas contiguas
        Screen* make_lock(Screen* s1, short area);

    public:
        DungeonM(string zone, string theme, short gameDiff, short dungNumber, short ratio, short tool, DBManager* db);
        ~DungeonM();

        void generate();

		void print_dungeon();	// DEBUG
};

#endif
