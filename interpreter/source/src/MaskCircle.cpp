#include "MaskCircle.h"

// Constructora MaskCircle, llamamos a la constructora de Mask con los parametros adecuados
MaskCircle::MaskCircle(int x, int y, int width, int height, string type, int xc, int yc, float r):Mask(x, y, width, height, type){
	xcenter = xc;
	ycenter = yc;
	radius = r;
}

// Destructora por defecto
MaskCircle::~MaskCircle(){
}

vector<CollisionPair>* MaskCircle::collide(Mask* other){
	vector<CollisionPair>* collPairs = Mask::collide(other); // Llamamos al padre para comprobar colisi�n simple de las Masks
	if (collPairs == NULL)			// Comprobamos si hubo colisi�n
		return NULL;				// Si no hubo devolvemos NULL
	else {	// Hubo collisi�n, comprobamos si los elementos internos verdaderamente colisionan
		CollisionPair collPair;			// Preparamos un CollisionPair por si hay colisi�n 
		collPair.a = type;
		collPair.b = other->type;
				
		//--- Colisi�n MaskCircle con MaskCircle ---//
		if (MaskCircle* maskC = dynamic_cast<MaskCircle *> (other)){	// Probamos hacer un cast a MaskCircle
			float collDistance = radius + maskC->radius;		// Distancia en la que collisionan las dos circunferencias
			float distance = getDistance(xcenter, maskC->x, ycenter, maskC->y);	// Distancia actual entre los las m�scaras
			if (distance < collDistance){		// Hay colisi�n
				collPairs->push_back(collPair);	// Introducimos el par de collisi�n en el vector de colisiones
			}
		}

		//--- Colisi�n MaskCircle con MaskBox ---//
		else if (MaskBox* maskB = dynamic_cast<MaskBox *> (other)){
			float distance = -1.f;	// Guardaremos aqu� la distancia del c�rculo a la esquina del MaskBox
			
			// Comprobamos casos en los que la MaskCircle NO est� ubicada perpendicular respecto a los lados de la MaskBox
			if (x < maskB->x){		// Parte izquierda
				if (y < maskB->y)						// Esquina arriba-izquierda
					distance = getDistance(xcenter, maskB->x, ycenter, maskB->y);
				else if (y > maskB->y + maskB->height)	// Esquina abajo-izquierda
					distance = getDistance(xcenter, maskB->x, ycenter, maskB->y + maskB->height);
			}
			else if (x > maskB->x +	maskB->width){	// Parte derecha
				if (y < maskB->y)						// Esquina arriba-derecha
					distance = getDistance(xcenter, maskB->x + maskB->width, ycenter, maskB->y);
				else if (y > maskB->y + maskB->height)	// Esquina abajo-derecha
					distance = getDistance(xcenter, maskB->x + maskB->width, ycenter, maskB->y + maskB->height);
			}
			// Comprobamos si hubo cambio en distance, en cuyo caso tambien comprobamos la colisi�n
			if (distance >= 0 && distance < radius){ // Si la distancia a la esquina es menor que el radio colisionamos
				collPairs->push_back(collPair);	// Introducimos el par de collisi�n en el vector de colisiones
			}

			// La cirurferencia es perpendicular a los lados del MaskBox y por tanto se comporta como un cuadrado de lado radius
			else {
				// Averiguamos si colisionan
				if (checkBoxCollision(maskB->x, maskB->y, maskB->width, maskB->height))
					collPairs->push_back(collPair);	// Introducimos el par de collisi�n en el vector de colisiones
			}
		}

		//--- Colisi�n MaskCircle con MaskList ---//
		else if (MaskList* maskL = dynamic_cast<MaskList *> (other)){
			// Si es una MaskList delegamos el trabajo (faltaria hacer un flip)
			vector<CollisionPair> *auxCollPairs = maskL->collide(this);
			if (auxCollPairs != NULL){
				vector<CollisionPair>::iterator it;		// Creamos un iterador de CollisionPair
				it = collPairs->end();					// Lo colocamos al final de collPairs
				// A�adimos las colisiones que se hayan producido con MaskList
				collPairs->insert(it, auxCollPairs->begin(), auxCollPairs->end());
				delete auxCollPairs;		// Eliminamos el vector auxiliar
			}
		}
	}
	return collPairs;	// Job done! devolvemos los resultados
}

// Distancia de un punto a otro
float MaskCircle::getDistance (int x1, int x2, int y1, int y2){
	return sqrt((float)((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)));
}

bool MaskCircle::checkBoxCollision(int bx, int by, int bwidth, int bheigth){
	int left1 = x;							// L�mite izquierdo circunferencia
    int left2 = bx;							// L�mite izquierdo rect�ngulo
    int right1 = x + width;					// L�mite derecho circunferencia
    int right2 = bx + bwidth;				// L�mite derecho rect�ngulo
    int top1 = y;							// L�mite superior circunferencia
    int top2 = by;							// L�mite superior rect�ngulo
    int bottom1 = y + height;				// L�mite inferior circunferencia
    int bottom2 = by + bheigth;				// L�mite inferior rect�ngulo

	// Vamos comprobando colisiones, 
	// Si no hay colisi�n devolvemos NULL
    if (bottom1 < top2)	return false;		// Si la parte inferior de una NO collisiona con la superior de la otra
	if (top1 > bottom2) return false;		// Si la parte superior de una NO collisiona con la inferior de la otra
	if (right1 < left2) return false;		// Si la parte derecha de una NO collisiona con la izquierda de la otra
	if (left1 > right2) return false; 		// Si la parte izquierda de una NO collisiona con la derecha de la otra
	
	// Si las comprobaciones anteriores fallan entonces las m�scaras colisionan
	return true;	
}