#pragma once

#ifndef __FONT_H__
#define __FONT_H__

#include "Image.h"

using namespace std;


class Font
{
public:

	Font();								//No hace n�
	int getGlyphId(char c);             //La implementar� TileFont
	Image* getGlyphImage(char c);       //La implementar� la posible Ttf font

};
#endif __FONT_H__