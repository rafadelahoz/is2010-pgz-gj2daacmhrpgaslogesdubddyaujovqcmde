#ifndef _MAPDATA_H
#define _MAPDATA_H

#include <pair>

class MapData 
{
  
private:
    // Ancho en pantallas del mapa
	int width;

    // Alto en pantallas del mapa
	int height;

    // Layout del mapa
	int** layout;

    // N�mero de minibosses de la mazmorra
	int numMiniBosses;

    // N�mero de Puzzles de la mazmorra
	int numPuzzles;

    // N�mero de puertas en la mazmorra
    int numDoors;

    // Identificador del mapa
    int mapId;

    // Pantalla inicial del mapa
    std::pair<int, int> startScreen;


  public:
    MapData(void );

    ~MapData(void );

    //! Obtiene el id del mapa
    int getId();

    //! Obtiene el ancho del mapa (en pantallas)
    int getWidth();

    //! Obtiene el alto del mapa (en pantallas)
    int getHeight();

    //! Obtiene el layout del mapa
    const int** getLayout();

    //! Indica si la pantalla indicada existe o no
    bool hasScreen(int x, int y);

    //! Obtiene el n�mero total de minibosses del mapa
    int getNumMiniBosses();

    //! Obtiene el n�mero total de puzzles del mapa
    int getNumPuzzles();

    //! Obtiene el n�mero total de puertas del mapa
    int getNumDoors();

    // getExit, getStartPoint, ...
    std::pair<int, int> getStartScreen();

};
#endif
