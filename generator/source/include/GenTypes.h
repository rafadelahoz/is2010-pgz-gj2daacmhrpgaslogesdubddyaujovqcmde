#pragma once

#ifndef _GENTYPES_H_
#define _GENTYPES_H_

#include "GenMath.h"

using namespace std;

#define screenWidth 14
#define screenHeight 12
#define zoneSpacing 10

typedef enum {Nothing, OWPortal, DUNPortal} EntType;

// Paquete de informaci�n sobre una Zona
struct DungeonInfo {
	// Herramienta que contiene la mazmorra.
	int toolId;
	// N�mero de orden de mazmorra, relacionado con su dificultad
	int order;
	// Habria que ver que m�s quiere el genDungeon: bool boss, objetoClave ... ?
};

// Paquete de informaci�n sobre una Zona
struct ZoneInfo
{
	// Tema de la zona, si es NULL significa que no la especificamos y genOverworld la elige.
	int themeId;
	ZoneInfo(int tid){ themeId = tid;}
	ZoneInfo(){ themeId = -1;}
};

// Paquete de informaci�n sobre una Zona
struct SafeZoneInfo {
	// Especifica los edificios de utilidad que contiene (armeria, recuperaVida, objetos especiales)
	bool inhabited;
};

struct GenEntity
{
	EntType type; // Nos indica si es una entrada a mazmorra, un portal, (mas en un futuro)
	string dungeonPath; // archivo a cargar de mazmorra
	GPoint dest; // destino del portal

	GenEntity(){ type=OWPortal; dungeonPath=""; dest.x=0; dest.y=0;}
};

#endif
