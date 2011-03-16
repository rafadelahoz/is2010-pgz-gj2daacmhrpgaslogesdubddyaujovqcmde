#pragma once
#ifndef __GFXLIST_H__
#define __GFXLIST_H__

#include "GfxEngine.h"
#include "Graphic.h"
#include <vector>

using namespace std;

//! Miembro de la familia de Graphic que facilita el trabajo con varios gr�ficos simultaneamente
/*!
	Agrupa varios gr�ficos no necesariamente del mismo tipo, simplificando el mantenimiento
	y el renderizado de todos en conjunto.

	Los gr�ficos se almacenan en posiciones determinadas, 
	asign�ndoles un nombre de forma que se puede trabajar
	individualmente o en conjunto.

	Los Graphic que se a�adan al GraphicList no deben borrarse mientras dure
	el ciclo de vida de este, al t�rmino del cual se eliminar�n autom�ticamente.
*/
class GraphicList : public Graphic
{
private:
	GfxEngine* gfxEngine;
	vector<pair<string, Graphic*> > gfxList;
public:

	//! Construye el GraphicList
	GraphicList(GfxEngine* gfxEngine);

	//! A�ade un gr�fico, asign�ndole un nombre, en la posici�n indicada
	bool addGraphic(string name, Graphic* gfx, int position = -1);

	//! Recupera un gr�fico para se modificado individualmente de forma r�pida
	Graphic* getGraphic(string name);

	//! Recupera un gr�fico para se modificado individualmente de forma r�pida
	Graphic* getGraphic(int position);

	//! Update
	void update();

	// Refresh
	void refresh();

	//! Render
	void render(int x, int y);

	/* M�todos de spritemap */
	
	//! A�adir animaci�n
	void addAnim(string name, vector<int>* frameList, int framesPerStep, bool loop, int position = -1);

	//! Reproducir animaci�n
	void playAnim(string name, int framesPerStep, bool loop, int position = -1);
};

#endif