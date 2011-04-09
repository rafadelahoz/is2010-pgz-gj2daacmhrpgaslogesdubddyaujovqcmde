#pragma once

#ifndef __TILETEXTLABEL_H__
#define __TILETEXTLABEL_H__


#include "TextLabel.h"
#include "TileFont.h"
#include "TileMap.h"

using namespace std;


class TileTextLabel:public TextLabel
{
private:

	TileMap* tileMap;
	GfxEngine* gfxEngine;
	TileFont* tileFont;

public:

	TileTextLabel(string texto, TileFont* font, GfxEngine* gfxEngine,int w = 0, int h = 0);
	//TileTextLabel(TileFont* font, GfxEngine* gfxEngine,int w = 0, int h = 0);
	~TileTextLabel();

	void setScale(float scale);
	bool addCharacter(char c, Color color = Color::White);
	void setColumns(int columns);
	void setRows(int rows);
	int setText(string myText, TextMode m = REWRITE);
	void render(int x, int y);

};
#endif __TILETEXTLABEL_H__