/*----------------------------------------------------------------------------------*/
//This code is part of a larger project whose main purpose is to entretain either	//
//by helping making it better or by playing it in its actual state.					//
//																					//
//Copyright (C) 2011 Three Legged Studio											//
//																					//
//    This program is free software; you can redistribute it and/or modify			//
//    it under the terms of the GNU General Public License as published by			//
//    the Free Software Foundation; either version 1, or (at your option)			//
//    any later version.															//
//																					//
//    This program is distributed in the hope that it will be useful,				//
//    but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the					//
//    GNU General Public License for more details.									//
//																					//
//    You should have received a copy of the GNU General Public License				//
//    along with this program; if not, write to the Free Software Foundation,		//
//    Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA					//
//																					//
//	  You can contact us at projectpgz.blogspot.com									//
/*----------------------------------------------------------------------------------*/

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
		//Numero de corazones
		short numHearts;
		//Numero de pigeons
		short numPigeons;

		

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
		vector<GPoint> dungeonScreenPoints;

		//Vector de la posici�n X e Y de las entradas de mazmorra respecto al mundo
		vector<int> dungeonTilesPoints;

		// Mega-matriz sobre la que trabaja genOverworld
		vector<MapTile*>* mapTileMatrix;

		//Lista de todas las screen que hay en el mundo
		vector<OwScreen*>* screenList;

		// Constructora: recibe la informaci�n de Decidator y la almacena en sus atributos pertinentes.
		Overworld(int wSize, int wDiff, int numZones, int numDungeons, int numSafeZones, short numHearts, short numPigeons);

		// Destructora
		~Overworld();

		bool save(std::string path);

		void addScreen(OwScreen* screen);

		// Getters utiles:
		int getNumZones();
		int getNumDungeons();
		int getNumSafeZones();
		int getNumHearts();
		int getNumPigeons();
		int getWorldDiff();
		GPoint getStartLocation();
		void setStartLocation(int x,int y);
		void setN_Collectables(short quantity);

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
