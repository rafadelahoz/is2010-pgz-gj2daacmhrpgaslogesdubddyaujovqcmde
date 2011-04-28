#include "DamageableBlockade.h"


DamageableBlockade::DamageableBlockade(int x, int y, Game* game, GameState* gameState):Entity(x,y,game,gameState)
{
}

void DamageableBlockade::init(short typeWeakness, Graphic* graphic, int xColision, int yColision)
{
	iDamageable::init(1,1,0,typeWeakness);
	this->graphic = graphic;
	this->mask = new MaskBox(x, y, xColision, yColision, "DamageableBlockade");
	this->solid = true;
}


/*/////////////////////////////////////////////////////////////////////////////////////////
Hay que cambiar el graphic que le pasan por una ruta para que yo lo cargue, habr� un spriteSheet
y un archivo de configuraci�n con ancho de tile y alto de tile, xoffset y yoffset, columnas y filas
y luego 4 enteros que ser�n la animaci�n de parado y otros 4 de la animacion de muerte
/////////////////////////////////////////////////////////////////////////////////////////*/


void DamageableBlockade::onDeath()
{
	instance_destroy();
}

//esto est� temporal, es el arma el que deberia llamar a la onDamage
void DamageableBlockade::onCollision(CollisionPair other, Entity* e)
{
	if (other.b == "tool")
	{
		// Si algun arma nos colisiona nos hace da�o
		onDamage(1, PHYSICAL);
	}
}