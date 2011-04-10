#include "TileTextLabel.h"


TileTextLabel::TileTextLabel(string texto, TileFont* font, GfxEngine* gfxEngine,int columns, int rows):TextLabel(texto)
{
	//Apuntamos al motor grafico
	this->gfxEngine = gfxEngine;
	//Me guardo la tileFont por si alguien la ha creado como una Font y no se ha preocupado de guardarla
	this->tileFont = font;
	//Creamos el tileMap con el tama�o de tile del tileset de la fuente
	tileMap = new TileMap(tileFont->getTileW(),tileFont->getTileH(),gfxEngine);
	//Indicamos al tileMap que su tileSet es el de la funte
	tileMap->setTileSet(font->getTileSet());
	//Si no me pasan las filas y columnas las pongo yo como vea
	if (columns == 0 && rows == 0)
	{
		rows = 1;
		columns = texto.size();
		setColumns(columns);
		setRows(rows);
		sizeSetted = false;
	}
	else
	{
		setColumns(columns);
		setRows(rows);
	}
	//Escribimos el texto en el tileMap
	setText(texto);
}

/*
TileTextLabel::TileTextLabel(TileFont* font, GfxEngine* gfxEngine,int w, int h):TextLabel("")
{
	//Apuntamos al motor grafico
	this->gfxEngine = gfxEngine;
	//Creamos el tileMap con el tama�o de tile del tileset de la fuente
	tileMap = new TileMap(font->getTileW(),font->getTileH(),gfxEngine);
	//Indicamos al tileMap que su tileSet es el de la funte
	tileMap->setTileSet(font->getTileSet());
	if (w == 0 && h == 0)
	{
		h = font->getTileH();
		w = font->getTileW();
	}

}
*/

TileTextLabel::~TileTextLabel()
{
	//Si existe tileMap
	if (tileMap)
	{
		//Lo borramos asegur�ndonos de no machacar la TileFont
		delete(tileMap);
		tileMap=NULL;
	}
	if (tileFont)
	{
		delete(tileFont);
		tileFont =NULL;
	}
}

void TileTextLabel::setColumns(int columns)
{
	tileMap->setCols(columns);
	sizeSetted = true;
}



void TileTextLabel::setRows(int rows)
{
	tileMap->setRows(rows);
	sizeSetted = true;
}


void TileTextLabel::setScale(float scale)
{
	TextLabel::setScale(scale);
	tileMap->setScale(scale,scale);
	Graphic::setScale(scale,scale);
}


bool TileTextLabel::addCharacter(char c, Color color)
{
    //Si la latra que quieres a�adir se sale del tama�o del tileMap devolvemos false
	if ((myText.size() + 1) >= tileMap->getCols() * tileMap->getRows())
		return false;
		
	//Le pregunto a la fuente donde tiene el caracter que yo quiero pintar
	int pos = tileFont->getGlyphId(c);
	//Concateno ese caracter al texto que llevo
	myText = myText.append(&c);

	//Calculo el tama�o del texto para saber que en que posicion me toca esribir
	int size = myText.size();
	
	//Calculo a partir del caracter que va, en que posicion (x,y) toca escribir
	int x = size % tileMap->getWidth();
	int y = size / tileMap->getWidth();

	/* 
    dibuja sobre mapImage el tile de tileSet de coordenadas especificadas:
    (pos % tileSet->getColumns())*w 
    (pos / tileSet->getColumns())*h
    */
    if (tileMap->getMapImage() != NULL)
			//Si el sitio donde quiero pintar est� dentro de la pantalla
            if ( tileFont->getTileSet()->getColumns() > 0 && tileFont->getTileSet()->getRows() > 0)
					//Pinto
                    gfxEngine->renderPartExt(tileFont->getTileSet()->getImg(), //Imagen
					x*tileFont->getTileW(), y*tileFont->getTileH(), //Posicion en la que se va a pintar
					(pos % tileFont->getTileSet()->getColumns())*tileFont->getTileSet()->getTileW(), //coordenada x de la posicion del tile en el tileSet
					(pos / tileFont->getTileSet()->getColumns())*tileFont->getTileSet()->getTileH(), //coordenada y de la posicion del tile en el tileSet
					tileFont->getTileSet()->getTileW(), tileFont->getTileSet()->getTileH(), //Ancho y alto de cada tile
					color, alpha, scaleH, scaleV, rotation, tileMap->getMapImage(), //Caracteristias de lo que se va a pintar y imagen sobre la que va a hacerlo
					tileFont->getTileSet()->getTileW()/2, tileFont->getTileSet()->getTileH()/2); //origen????
	return true;
};


int TileTextLabel::setText(string text, TextMode m)
{
	
	//Me guardo las filas y columnas del tileMap
	int fils = tileMap->getRows();
	int cols = tileMap->getCols();
		
		
	//-----------------------------------------------------------------------------------------
	//Me guardo si las filas y columnas se habian setteado
	bool setteado = this->sizeSetted;
	//Codigo temporal, solo un apa�o para que funcione mientras se cambian unas cosas del motor
	//borro el antiguo tileMap, cuya imagen ya no me vale y no se puede borrar
	delete(tileMap);
	//Creamos el tileMap con el tama�o de tile del tileset de la fuente
	tileMap = new TileMap(tileFont->getTileW(),tileFont->getTileH(),gfxEngine);
	//Indicamos al tileMap que su tileSet es el de la funte
	tileMap->setTileSet(tileFont->getTileSet());
	//Vuelvo a ponerle como estaban las filas, las columnas y si las habia puesto el usuario o eran autogestionadas.
	setColumns(cols);
	setRows(fils);
	this->sizeSetted = setteado;
	//-----------------------------------------------------------------------------------------
	

	//Si estoy en modo rewrite, paso de lo que hubiera escrito y escribo desde 0
	if (m == REWRITE)
		myText = text;
	//Si estoy en modo append concateno al final del texto lo que quiero escribir
	else if (m == APPEND)
		myText = myText.append(text);

	//Si no se han configurado nunca las filas y columnas las apa�o yo para que quepa
	if (!sizeSetted)
	{
		fils = 1;
		cols = myText.size();
	}

	//Me creo el mapa de tiles que le voy a pasar al tileMap
	int** mapa = (int **) malloc(cols *sizeof(int));
    for(int i = 0; i < cols; i++)
            mapa[i] = (int *) malloc(fils*sizeof(int));
	int i = 0;
	while((i < cols*fils) && (i < myText.size()))
	{
		mapa[i % cols][i / cols] = tileFont->getGlyphId(myText.at(i));
		i++;
	}
	//Actualizo mi tileMap con el map que le he creado
	tileMap->setMap(mapa, cols, fils);

	//Le aviso de que actualize su imagen
	this->tileMap->getMapImage();

	return i + 1;
}


void TileTextLabel::render(int x, int y)
{
	tileMap->render(x,y);
}


