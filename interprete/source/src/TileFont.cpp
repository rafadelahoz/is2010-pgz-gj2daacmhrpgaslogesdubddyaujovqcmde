#include "TileFont.h"




TileFont::TileFont(string path, GfxEngine* gfxEngine):Font()
{
	int tileW,tileH;
	
	// se busca extensi�n
    size_t found = path.find_last_of(".");
        
    //Si la encuentra
    if (found != string::npos){
            //Se descarta 
            path = path.substr(0,found);
    }
	// Se almacena el nombre de archivo sin extensi�n
    string pathconf = path;

    // carga configuracion y tileSet de los ficheros correspondientes
    //nombreArchivo.png
    path = path.append(".png");
    //nombreArchivo.txt
    pathconf= pathconf.append(".txt");
	
	//Abro el archivo para lectura
	FILE* file = NULL;
    file = fopen(pathconf.c_str(), "r");

	// lectura de ancho y alto de tile
    fscanf(file, "%d", &tileW);
    fscanf(file, "%d", &tileH);

	//Voy colocando todos los pares codigo asci, posicion en el tileSet
	for (int i = 0; i < NUM_CHAR; i++)
	{
		int aux;
		fscanf(file, "%d", &aux);
		posicion->insert(make_pair(aux,i));
	}

	tileSet = new TileSet(path,tileW,tileH,gfxEngine);
}




int TileFont::getGlyphId(char c)
{
// Creamos un iterador para map
    map<int, int>::iterator it;
    //Pido que busque el caracter solicitado
    it = posicion->find((int) c);
    if (it == posicion->end())
            //Si no est� hemos acabado y devolvemos -1
            return -1;
	return it->second;

}
