#include "Mask.h"

// Constructora, asignamos los parametros a los atributos correspondientes de Mask
Mask::Mask(int x, int y, int width, int height, string type){
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->type = type;
}

// Destructora por defecto
Mask::~Mask(){
}

vector<CollisionPair>* Mask::collide(Mask* other){
	//Consideramos Mask1 como nuestra m�scara y Mask2 como 
	//la otra m�scara con la que comprobamos colisi�n (other)

	int left1 = x;							// L�mite izquierdo Mask1
    int left2 = other->x;					// L�mite izquierdo Mask2
    int right1 = x + width;					// L�mite derecho Mask1
    int right2 = other->x + other->width;	// L�mite derecho Mask2
    int top1 = y;							// L�mite superior Mask1
    int top2 = other->y;					// L�mite superior Mask2
    int bottom1 = y + height;				// L�mite inferior Mask1
    int bottom2 = other->y + other->height;	// L�mite inferior Mask2

	// Vamos comprobando colisiones, 
	// Si no hay colisi�n devolvemos NULL
    if (bottom1 < top2)	return NULL;		// Si la parte inferior de una NO collisiona con la superior de la otra
	if (top1 > bottom2) return NULL;		// Si la parte superior de una NO collisiona con la inferior de la otra
	if (right1 < left2) return NULL;		// Si la parte derecha de una NO collisiona con la izquierda de la otra
	if (left1 > right2) return NULL; 		// Si la parte izquierda de una NO collisiona con la derecha de la otra
	
	// Si las comprobaciones anteriores fallan entonces las m�scaras colisionan
	vector<CollisionPair> *collPairs = new vector<CollisionPair>();	// Creamos un vector de CollisionPair
	return collPairs;					// Devolvemos el vector de pares de collisi�n vac�o
										// las clases que heredan se ocupan de rellenarlo
}