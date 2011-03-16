#pragma once

#ifndef _GENTYPES_H_
#define _GENTYPES_H_

using namespace std;


typedef enum {portal_w, portal_d} type_t;


// Paquete de informaci�n sobre una Zona
struct DungeonInfo {
	// Herramienta que contiene la mazmorra.
	string toolId;
	// N�mero de orden de mazmorra, relacionado con su dificultad
	int order;
	// Habria que ver que m�s quiere el genDungeon: bool boss, objetoClave ... ?
};

// Paquete de informaci�n sobre una Zona
struct ZoneInfo
{
	// Tema de la zona, si es NULL significa que no la especificamos y genOverworld la elige.
	string themeId;
	// M�ssssssssssss ?
};

// Paquete de informaci�n sobre una Zona
struct SafeZoneInfo {
	// Especifica los edificios de utilidad que contiene (armeria, recuperaVida, objetos especiales)
	bool inhabited;
};

struct entity_t
{
	type_t type;
	string dungeon_path;
	pair<int, int> dest;
};

struct Point
{
	int x;
	int y;
};





#endif
