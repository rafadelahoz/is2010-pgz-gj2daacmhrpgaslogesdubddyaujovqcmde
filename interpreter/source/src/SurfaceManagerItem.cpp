#include "SurfaceManagerItem.h"

/***********************************************
 *			Funciones P�blicas				   *
 ***********************************************/

/* SurfaceManagerItem::SurfaceManagerItem(sf::Image* surf) */
/*
	Guardamos la direccion/puntero de una imagen y la cantidad de
	enlaces simb�licos que inicialmente es 1.
*/
SurfaceManagerItem::SurfaceManagerItem(sf::Image* surf)
{
	surface = surf;
	links = 1;
}

/* void SurfaceManagerItem::link() */
/*
	A�adimos un nuevo enlace simb�lico ya que alguien m�s
	est� usando la imagen.
*/
void SurfaceManagerItem::link()
{
	links++;
}
	
/* bool SurfaceManagerItem::unlink() */
/*
	Quitamos un enlace a la imagen.
	Si no hay enlaces con la imagen entonces devolvemos true.
*/
bool SurfaceManagerItem::unlink()
{
	links--;
	return (links <= 0);
}
	
/* sf::Image* SurfaceManagerItem::getSurface() */
/*
	Devuelve puntero a la propia imagen/superficie
*/
sf::Image* SurfaceManagerItem::getSurface()
{
	return surface;
}
	
/* int SurfaceManagerItem::getLinks() */
/*
	Devuelve el n�mero de enlaces que tiene la imagen
*/
int SurfaceManagerItem::getLinks()
{
	return links;
}