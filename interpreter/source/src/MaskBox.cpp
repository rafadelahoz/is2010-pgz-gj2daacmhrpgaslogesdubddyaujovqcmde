#include "MaskBox.h"

// Constructora vac�a ya que la m�scara es la global (la del padre)
MaskBox(int x, int y, int width, int height, string type):
	Mask(x, y, width, height, type){}

// Destructora vac�a ya que lo hace el padre Mask
~MaskBox():~Mask(){}

vector<CollisionPair>* collide(Mask* other){
	// Vemos primero si colisionan las m�scaras globales (las del padre)
	vector<CollisionPair>* collPairs = Mask::collide(other);

	// Si no colisionan, entonces tampoco colisiona la caja, ya que son la misma
	if (collPairs == NULL) return NULL;
	else{ // Si colisionan las globales, las cajas tambi�n

		// Colisi�n con MaskBox
		if (MaskBox* maskB = dynamic_cast<MaskBox *> (other)){
			// Preparamos el par con los tipos de la colision
			CollisionPair collPair;
			collPair.a = type;
			collPair.b = other->type;
			// Metemos el par en el vector
			collPairs->push_back(collPair);
		}

		// Colisi�n con MaskCircle
		else if(MaskCircle* maskC = dynamic_cast<MaskCircle *> (other)){
			delete collPairs; // eliminamos el vector vac�o creado en mask, para no dejar basura
			collPairs = maskC->collide(this); // delegamos el trabajo en la m�scara c�rcular
			// Damos la vuelta a los pares de la lista, ya que la colisi�n realmente ha sido al rev�s
			// flipAll(collPairs);
		}

		// Colisi�n con MaskList
		else if (MaskList* maskL = dynamic_cast<MaskList *> (other)){
			delete collPairs; // eliminamos el vector vac�o creado en mask, para no dejar basura
			collPairs = maskL->collide(this); // delegamos el trabajo en la lista de m�scaras
			// Damos la vuelta a los pares de la lista, ya que la colisi�n realmente ha sido al rev�s
			// flipAll(collPairs);
		}

		// Colisi�n con SolidGrid
		else if (SolidGrid* grid = dynamic_cast<SolidGrid *> (other)){
			delete collPairs;
			collPairs = grid->collide(this);
			// Damos la vuelta a los pares de la lista, ya que la colisi�n realmente ha sido al rev�s
			// flipAll(collPairs);
		}
	
	} // Fin de else (han colisionado)
}