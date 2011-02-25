#pragma once

#ifndef _TILEMAP_H_
#define _TILEMAP_H_

using namespace std;

//! Representa el aspecto gr�fico de un mapa de tiles
/*!
	Encapsula un tileSet que le servir� de base y guarda el grafico resultante de componer los tiles
	que se le indiquen.
	Se compone indicando que n�mero de un tileSet va en cada posici�n de su rejilla
*/

class TileMap : public Graphic {

	public:

		//! Construye un tileMap a partir del alto y ancho de un tile y el motor grafico
		/*!
			\param tileW contiene el ancho de un tile
			\param tileH contiene el alto de un tile
			\param gfxEngine contiene la informaci�n de la gesti�n de los gr�ficos.
		*/
		TileMap(int tileW, int tileH, GfxEngine* gfxEngine);
		
		//! Destructora
		~TileMap();

		//! Establece del tileSet en el que se basar� el mapa
		/*!
			\param tspath contiene la ruta del tileSet
		*/
		void setTileSet(string tspath);

		//! Establece del mapa en si, a partir de otro mapa
		/*!
			\param map contiene el mapa del que ser� copiado el mapa
		*/
		void setMap(int** map);
		
		//! Establece del n�mero de columnas del mapa
		/*!
			\param col contiene el n�mero de columnas del mapa
		*/
		void setCols(int col);
		
		//! Establece del n�mero de filas del mapa
		/*!
			\param row contiene el n�mero de filas del mapa
		*/
		void setRows(int row);

		//! Obtiene de la imagen del mapa ya construdido.
		/*!
			\return la imagen del mapa
		*/
		Image* getMapImage();
		
		//! Establece de uno de los tiles del mapa
		/*!
			\param x contiene la coordenada x del tile que deseamos cambiar
			\param y contiene la coordenada y del tile que queremos cambiar
			\param tile contiene el tile del tileSet que vamos a colocar en la posici�n indicada del mapa
		*/
		void setTile(int x, int y, int tile);
		
		//! Se en carga de dibujar el tileMap en una posici�n en concreto
		/*!
			\param x contiene la coordenada x del tile en el que empezar� el tileMap
			\param y contiene la coordenada y del tile en el que empezar� el tileMap
		*/
		void render(int x, int y);

		//! Obtiene del ancho de cada tile del tileMap
		/*!
			\return el ancho de los tiles del tileMap
		*/
		int getWidth();
		
		//! Obtiene del alto de cada tile del tileMap
		/*!
			\return el alto de los tiles del tileMap
		*/
		int getHeight();
};

#endif // _TILEMAP_H_
