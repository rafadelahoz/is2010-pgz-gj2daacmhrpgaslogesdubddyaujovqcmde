#include "MaskList.h"

// Constructora MaskList, llamamos a la constructora de Mask con los parametros adecuados
MaskList::MaskList(int x, int y, int width, int height, string type, vector<Mask*>* m, int xoffset, int yoffset):Mask(x, y, width, height, type, xoffset, yoffset){
	masks = m;
}

// Destructora MaskList - (en principio el programador se encarga de todo)
MaskList::~MaskList()
{
	if (masks != NULL)
	{
		Mask* tmp;
		vector<Mask*>::iterator it = masks->begin();
		while (it != masks->end())
		{
			if ((*it) != NULL)
			{
				tmp = (*it);
				masks->erase(it);
				delete tmp;				
			}
			it = masks->begin();
		}
		delete masks;
	};
}

vector<CollisionPair>* MaskList::collide(Mask* other){
	vector<CollisionPair>* collPairs = Mask::collide(other); // Llamamos al padre para comprobar colisi�n simple de las Masks
	if (collPairs == NULL)			// Comprobamos si hubo colisi�n
		return NULL;				// Si no hubo devolvemos NULL
	else {	// Hubo collisi�n, comprobamos si los elementos internos verdaderamente colisionan
		vector<CollisionPair>* auxCollPairs;	// Preparamos un puntero para engancharlo a los resultados de las llamadas a collide

		//--- Colisi�n MaskList con MaskBox ---//
		if (MaskBox* maskB = dynamic_cast<MaskBox *> (other)){		// Probamos hacer un cast a MaskBox
			for(unsigned short i=0; i<masks->size(); i++){				// Iteramos nuestra lista interna de m�scaras
				auxCollPairs = maskB->collide(masks->at(i));	// Comprobamos collisiones con cada elemento
				if (auxCollPairs != NULL)
				{
					Mask::flipAll(auxCollPairs);						// Flip del vector de pares
					concatVect(auxCollPairs, collPairs);			// A�adimos los resultados
					delete auxCollPairs;							// Liberamos memoria (creada al llamar al m�todo collide anterior)
				}
			}
			return collPairs;			// Devolvemos el vector con los resultados de las comprobaciones
		}

		//--- Colisi�n MaskList con MaskCircle ---//
		else if (MaskCircle* maskC = dynamic_cast<MaskCircle *> (other)){		// Probamos hacer un cast a MaskCircle
			for(unsigned short i=0; i<masks->size(); i++){				// Iteramos nuestra lista interna de m�scaras
				auxCollPairs = maskC->collide(masks->at(i));	// Comprobamos collisiones con cada elemento
				if (auxCollPairs != NULL)
				{
					Mask::flipAll(auxCollPairs);						// Flip del vector de pares
					concatVect(auxCollPairs, collPairs);			// A�adimos los resultados
					delete auxCollPairs;							// Liberamos memoria (creada al llamar al m�todo collide anterior)
				}
			}
			return collPairs;			// Devolvemos el vector con los resultados de las comprobaciones
		}

		//--- Colisi�n MaskList con MaskList ---//
		else if (MaskList* maskL = dynamic_cast<MaskList *> (other)){		// Probamos hacer un cast a MaskList
			for(unsigned short i=0; i<masks->size(); i++){				// Iteramos nuestra lista interna de m�scaras
				auxCollPairs = maskL->collide(masks->at(i));	// Comprobamos collisiones con cada elemento
				if (auxCollPairs != NULL)
				{
					Mask::flipAll(auxCollPairs);						// Flip del vector de pares
					concatVect(auxCollPairs, collPairs);			// A�adimos los resultados
					delete auxCollPairs;							// Liberamos memoria (creada al llamar al m�todo collide anterior)
				}
			}
			return collPairs;			// Devolvemos el vector con los resultados de las comprobaciones
		}

		//--- Colisi�n MaskList con SolidGrid ---//
		else if (SolidGrid* grid = dynamic_cast<SolidGrid *> (other)){		// Probamos hacer un cast a SolidGrid
			for(unsigned short i=0; i<masks->size(); i++){				// Iteramos nuestra lista interna de m�scaras
				auxCollPairs = grid->collide(masks->at(i));	// Comprobamos collisiones con cada elemento
				if (auxCollPairs != NULL)
				{
					Mask::flipAll(auxCollPairs);						// Flip del vector de pares
					concatVect(auxCollPairs, collPairs);			// A�adimos los resultados
					delete auxCollPairs;							// Liberamos memoria (creada al llamar al m�todo collide anterior)
				}
			}
			return collPairs;			// Devolvemos el vector con los resultados de las comprobaciones
		}
		else
			return NULL;
	}
}

void MaskList::setXY(int x, int y){

    Mask::setXY(x, y);
    vector<Mask*>::iterator it;
    for(it = masks->begin(); it != masks->end(); it++)
        if ((*it) != NULL)
        {
            (*it)->setXY(x,y);
        }
}


// M�todo privado para concatenar vectores
void MaskList::concatVect(vector<CollisionPair> *fromVect, vector<CollisionPair> *toVect){
	vector<CollisionPair>::iterator it;		// Creamos un iterador de CollisionPair
	it = toVect->end();					// Lo colocamos al final de toVect, donde insertaremos
	// A�adimos las colisiones que contenga fromVect
	toVect->insert(it, fromVect->begin(), fromVect->end());
}
