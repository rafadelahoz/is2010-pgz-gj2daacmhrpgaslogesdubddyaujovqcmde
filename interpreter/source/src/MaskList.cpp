#include "MaskList.h"

// Constructora MaskList, llamamos a la constructora de Mask con los parametros adecuados
MaskList::MaskList(int x, int y, int width, int height, string type, vector<Mask*>* m):Mask(x, y, width, height, type){
	masks = m;
}

// Destructora MaskList - (en principio el programador se encarga de todo)
MaskList::~MaskList(){
	//for (int i=0; i < masks->size(); i++)
	//	delete &masks[i];
	//delete masks;
}

vector<CollisionPair>* MaskList::collide(Mask* other){
	vector<CollisionPair>* collPairs = Mask::collide(other); // Llamamos al padre para comprobar colisi�n simple de las Masks
	if (collPairs == NULL)			// Comprobamos si hubo colisi�n
		return NULL;				// Si no hubo devolvemos NULL
	else {	// Hubo collisi�n, comprobamos si los elementos internos verdaderamente colisionan
		vector<CollisionPair>* auxCollPairs;	// Preparamos un puntero para engancharlo a los resultados de las llamadas a collide

		//--- Colisi�n MaskList con MaskBox ---//
		if (MaskBox* maskB = dynamic_cast<MaskBox *> (other)){		// Probamos hacer un cast a MaskBox
			for(short i=0; i<masks->size(); i++){				// Iteramos nuestra lista interna de m�scaras
				auxCollPairs = maskB->collide(masks->at(i));	// Comprobamos collisiones con cada elemento
				concatVect(auxCollPairs, collPairs);			// A�adimos los resultados
				delete auxCollPairs;							// Liberamos memoria (creada al llamar al m�todo collide anterior) 
			}
			return collPairs;			// Devolvemos el vector con los resultados de las comprobaciones
		}

		//--- Colisi�n MaskList con MaskCircle ---//
		else if (MaskCircle* maskC = dynamic_cast<MaskCircle *> (other)){		// Probamos hacer un cast a MaskCircle
			for(short i=0; i<masks->size(); i++){				// Iteramos nuestra lista interna de m�scaras
				auxCollPairs = maskC->collide(masks->at(i));	// Comprobamos collisiones con cada elemento
				concatVect(auxCollPairs, collPairs);			// A�adimos los resultados
				delete auxCollPairs;							// Liberamos memoria (creada al llamar al m�todo collide anterior) 
			}
			return collPairs;			// Devolvemos el vector con los resultados de las comprobaciones
		}

		//--- Colisi�n MaskList con MaskList ---//
		else if (MaskList* maskL = dynamic_cast<MaskList *> (other)){		// Probamos hacer un cast a MaskList
			for(short i=0; i<masks->size(); i++){				// Iteramos nuestra lista interna de m�scaras
				auxCollPairs = maskL->collide(masks->at(i));	// Comprobamos collisiones con cada elemento
				concatVect(auxCollPairs, collPairs);			// A�adimos los resultados
				delete auxCollPairs;							// Liberamos memoria (creada al llamar al m�todo collide anterior) 
			}
			return collPairs;			// Devolvemos el vector con los resultados de las comprobaciones
		}

		//--- Colisi�n MaskList con SolidGrid ---//
		else if (SolidGrid* grid = dynamic_cast<SolidGrid *> (other)){		// Probamos hacer un cast a SolidGrid
			for(short i=0; i<masks->size(); i++){				// Iteramos nuestra lista interna de m�scaras
				auxCollPairs = grid->collide(masks->at(i));	// Comprobamos collisiones con cada elemento
				concatVect(auxCollPairs, collPairs);			// A�adimos los resultados
				delete auxCollPairs;							// Liberamos memoria (creada al llamar al m�todo collide anterior) 
			}
			return collPairs;			// Devolvemos el vector con los resultados de las comprobaciones
		}
	}
}

// M�todo privado para concatenar vectores
void MaskList::concatVect(vector<CollisionPair> *fromVect, vector<CollisionPair> *toVect){
	vector<CollisionPair>::iterator it;		// Creamos un iterador de CollisionPair
	it = toVect->end();					// Lo colocamos al final de toVect, donde insertaremos
	// A�adimos las colisiones que contenga fromVect
	toVect->insert(it, fromVect->begin(), fromVect->end());
}