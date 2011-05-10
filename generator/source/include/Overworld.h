#pragma once

#ifndef _OVERWORLD_H_
#define _OVERWORLD_H_

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <vector>
#include <string>
#include "GenMath.h"
#include "Screen.h"
#include "MapTile.h"
#include "OwScreen.h"

using namespace std;

// Clase encargada de almacenar los datos que genera la clase GenOverworld y de posteriormente crear los archivos de mapa que cargar� el motor.
/*
	Recibimos informaci�n de Decidator sobre el tama�o del mundo, zonas, mazmorras y zonas seguras.
	Una instancia de GenOverworld trabaja sobre nosotros y modifica mapTileMatrix, startLocation, prizePoints. Para ello, esta se instancia pas�ndole un puntero al propio objeto (this).
	Con toda esta informaci�n el generador de vida aportar� sus modificaciones adicionales.
	El Overwolrd finalizado llama a su operaci�n genScreens para generar los archivos de mapa.
*/

class Overworld{
	private:
		// Tama�o de la matriz mapTileMatrix
		int tileWorldSizeH;	// Alto en tiles
		int tileWorldSizeW;	// Ancho en tiles
		
		int worldSizeH; // Alto en pantallas
		int	worldSizeW; // Ancho en pantallas

		// Indicador del tama�o del overworld, genOverworld lo interpreta para asignar un ancho y alto adecuado.
		int worldSize;
		// Dificultad del juego
		short worldDiff; 
		// Numero de zonas
		short numZones;
		// Numero de mazmorras
		short numDungeons;
		// Numero de SafeZones
		short numSafeZones;

		// Para el int�rprete y su persistencia.
		short n_puzzles;
		short n_collectables;
		short n_blockades;
		
		// Punto donde comienza el personaje al iniciar por pirmera vez el juego, lo establece genOverworld y lo leer� genLife
		GPoint startLocation;

	public:

		// Puntos del overworld donde colocar premios/secretos, lo establece genOverworld y lo leer� genLife
		vector<GPoint> prizePoints;

		// Vector de la posicion X e Y de la pantalla que contiene una entrada a una mazmorra
		vector<GPoint> dungeonPoints;

		// Mega-matriz sobre la que trabaja genOverworld
		vector<MapTile*>* mapTileMatrix;

		//Lista de todas las screen que hay en el mundo
		vector<OwScreen*>* screenList;

		// Constructora: recibe la informaci�n de Decidator y la almacena en sus atributos pertinentes.
		Overworld(int wSize, int wDiff, int numZones, int numDungeons, int numSafeZones);

		// Destructora
		~Overworld();

		bool save();

		void addScreen(OwScreen* screen);

		// Getters utiles:
		int getNumZones();
		int getNumDungeons();
		int getNumSafeZones();
		int getWorldDiff();
		GPoint getStartLocation();
		void setStartLocation(int x,int y);

		int getTileWorldSizeH();
		int getTileWorldSizeW();
		int getWorldSizeH();
		int getWorldSizeW();
		MapTile* getMapTile(int x, int y);
		MapTile* getMapTile(int pos);

		//DEBUG
		void guardameSolids(string path);
		void guardameZonas(string path);
		void guardamePant(string path);
		void guardameAutotile(string path);
};


#endif
