#include "Player.h"

#define IMG_WIDTH 10    // ancho de la imagen del player
#define IMG_HEIGHT 16    // alto de la imagen del player

// Suponemos que mask y graphic no est�n inicialmente creados, sino dejar�a basura
Player::Player(int x, int y, Game* game, GameState* world, std::string gfxpath, int hp, int mp) : GameEntity(x, y, game, world) {

   // Asignamos el gr�fico a la entidad player, de momento una imagen est�tica
   graphic = new Stamp("player.png", game->getGfxEngine());

   // Creamos la m�scara
   mask = new MaskBox(x, y, IMG_WIDTH, IMG_HEIGHT, "player"); // offsets est�n a cero por defecto

   // Cambiamos la configuraci�n por defecto de los flags que nos interesan
   solid = true;
};

void Player::onStep()
{

   // MOVIMIENTO
   int move_pixels = 2; // n�mero de p�xeles que se mueve el player
   int xtemp = x;
   int ytemp = y;

   // Comprobamos entrada para mover al player
       // movimiento horizontal
   if (game->getInput()->key(Input::kRIGHT)) xtemp += move_pixels;
   else if (game->getInput()->key(Input::kLEFT)) xtemp -= move_pixels;
       // movimento vertical
   if (game->getInput()->key(Input::kUP)) ytemp -= move_pixels;
   else if (game->getInput()->key(Input::kDOWN)) ytemp += move_pixels;

   // Si el sitio est� libre movemos el player, sino lo dejamos donde est�
   if (place_free(xtemp, ytemp))
       x = xtemp; y = ytemp; // movimiento de la entidad
};

Dir Player::getDir(){

	return UP;

}