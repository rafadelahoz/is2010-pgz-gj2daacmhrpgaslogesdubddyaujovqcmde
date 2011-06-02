/*----------------------------------------------------------------------------------*/
//This code is part of a larger project whose main purpose is to entretain either	//
//by working on it and helping it be better or by playing it in it's actual state	//																			
//																					//
//Copyright (C) 2011 Three Legged Studio											//
//																					//
//    This program is free software; you can redistribute it and/or modify			//
//    it under the terms of the GNU General Public License as published by			//
//    the Free Software Foundation; either version 2, or (at your option)			//
//    any later version.															//
//																					//
//    This program is distributed in the hope that it will be useful,				//
//    but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the					//
//    GNU General Public License for more details.									//
//																					//
//    You should have received a copy of the GNU General Public License				//
//    along with this program; if not, write to the Free Software Foundation,		//
//    Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.			//
//																					//
//	  You can contact us at projectpgz.blogspot.com									//
/*----------------------------------------------------------------------------------*/

#include "DamageableBlockade.h"


DamageableBlockade::DamageableBlockade(int x, int y, Game* game, GameState* gameState):Entity(x,y,game,gameState), iDamageable()
{
	dying = false;
}

void DamageableBlockade::init(short typeWeakness, string gfxPath, int xColision, int yColision)
{
	iDamageable::init(1,1,0,typeWeakness);
	if (loadAnimations(gfxPath))
	{
		((SpriteMap*) graphic)->playAnim("stand");
	}
	else
		delete graphic, graphic = NULL;
	this->mask = new MaskBox(x, y, xColision, yColision, "DamageableBlockade");
	this->solid = true;
}

/*/////////////////////////////////////////////////////////////////////////////////////////
El archivo de configuraci�n contiene columnas y filas, velocidad y numero de frames
y luego 4 enteros que ser�n la animaci�n de parado y otros 4 de la animacion de muerte
/////////////////////////////////////////////////////////////////////////////////////////*/

bool DamageableBlockade::loadAnimations(string fname)
{
	// se busca extensi�n
    size_t found = fname.find_last_of(".");
        
    //Si la encuentra
    if (found != string::npos){
            //Se descarta 
            fname = fname.substr(0,found);
    }
	
	// Se almacena el nombre de archivo sin extensi�n
    string fnameConf = fname;

    // carga configuracion y tileSet de los ficheros correspondientes
    //nombreArchivo.png
    fname = fname.append(".png");
    //nombreArchivo.txt
    fnameConf= fnameConf.append(".cfg");
	
	// Carga el archivo de config y lee
	FILE* f = fopen(fnameConf.c_str(), "rb");

	// Si el archivo es inv�lido, no se puede hacer nada
	if (f == NULL)
		return false;

	// 1. Ancho y alto de imagen 
	int ncol = 0, nrow = 0;
	if (fscanf(f, "%d %d", &ncol, &nrow) < 2)
		return false;

	//Una vez tengo ya el ancho y el alto puedo cargar el SpriteMap que ser� mi grafico
	graphic = new SpriteMap(fname, ncol, nrow, game->getGfxEngine());
	
	// Leo todas las animaciones
	// Stand
	loadAnimation(Stand, "stand", f);

	//Death
	loadAnimation(Death, "death", f);

	//Cierro el archivo y salgo
	fclose(f);

	return true;
};


bool DamageableBlockade::loadAnimation(BlockadeAnim anim, string name, FILE* file)
{
	// Cargar animaci�n indicada de from
	if (file == NULL || name == "")
		return false;

	int numFrames, speed, auxFrame;
	int* frameList;

	// Leemos datos de la animaci�n

	// Speed
	if (fscanf(file, "%d", &speed) < 1)
		return false;

	// N� frames
	if (fscanf(file, "%d", &numFrames) < 1)
		return false;

	// Se instancia el contenedor de frames
	frameList = new int[numFrames];

	// Se carga cada frame
	for (int i = 0; i < numFrames; i++)
	{
		if (fscanf(file, "%d", &auxFrame) < 1)
			return false;
		frameList[i] = auxFrame;
	}

	// Se a�ade la animaci�n al graphic con el nombre indicado
	SpriteMap* spMap = ((SpriteMap*) graphic);
	spMap->addAnim(name, frameList, numFrames, speed, true);

	delete frameList;

	return true;
}

void DamageableBlockade::onStep()
{
	if (dying)
		if (((SpriteMap*) graphic)->animFinished())
			instance_destroy();
}

void DamageableBlockade::onDeath()
{
	((SpriteMap*) graphic)->playAnim("death",(float) 1,false);
	dying = true;
}

//esto est� temporal, es el arma el que deberia llamar a la onDamage
/*void DamageableBlockade::onCollision(CollisionPair other, Entity* e)
{
	if (other.b == "tool")
	{
		// Si algun arma nos colisiona nos hace da�o
		onDamage(1, PHYSICAL);
	}
}*/