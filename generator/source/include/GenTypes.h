#pragma once

#ifndef _GENTYPES_H_
#define _GENTYPES_H_

// Paquete de informaci�n sobre una Zona
struct DungeonInfo {
	// Herramienta que contiene la mazmorra.
	string toolId;
	// N�mero de orden de mazmorra, relacionado con su dificultad
	int order;
	// Habria que ver que m�s quiere el genDungeon: bool boss, objetoClave ... ?
};

// Paquete de informaci�n sobre una Zona
struct SafeZoneInfo {
	// Especifica los edificios de utilidad que contiene (armeria, recuperaVida, objetos especiales)
	bool inhabited;
};



#endif