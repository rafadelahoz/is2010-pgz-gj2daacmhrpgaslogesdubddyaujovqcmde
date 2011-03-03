#include "SpriteMap.h"


SpriteMap::SpriteMap(string fname, int nCol, int nRow, GfxEngine* gfxEngine)
{
	list = new map<string,Anim2D*>();
	currentAnim = NULL;
	img = new Image(fname,gfxEngine);
	this->gfxEngine = gfxEngine;
	logger = Logger::Instance();
	currentFramesPerStep = 0;
	currentLoop = false;
	frame = 0;
	lastFrame = 0;
	step = 1;
	this->nCol = nCol;
	this->nRow = nRow;
	finished = false;
}


void SpriteMap::deleteAnimMap()
{
	//Creamos un iterador para el map y lo inicializamos
	map< string, Anim2D* >::iterator it = list->begin();
	Anim2D* tmp;
	
	while (it != list->end())
	{
		//Referenciamos en el temporal la posible animaci�n a borrar
		tmp = it->second;
		it++;
		//Si hay animaci�n asociada en esa posici�n del map, la borramos
		if (tmp)
			delete tmp;
	}
	delete list;
}


SpriteMap::~SpriteMap() 
{
	//borro las animaciones y la imagen
	deleteAnimMap();
	delete img;
	Logger::DestroyInstance();
}


void SpriteMap::addAnim(string name, vector<int>* frameList, int framesPerStep, bool loop)
{
	//Creo la animaci�n y la a�ado a la lista de animaciones
	Anim2D* temp = new Anim2D(name, frameList, framesPerStep, loop);
	list->insert(make_pair(name, temp));
}



void SpriteMap::addAnim(string name, int* frameList, int numFrames, int framesPerStep, bool loop)
{
	//Creo un vector para guardar la lista de frames
	vector<int>* temp = new vector<int>(numFrames);
	
	//A�ado los frames a la lista
	for (int i = 0; i < numFrames; i++)
		(*temp)[i] = frameList[i];
		
	//Creo la animaci�n y la a�ado a la lista de animaciones
	Anim2D* aux = new Anim2D(name, temp, framesPerStep, loop);
	list->insert(make_pair(name, aux));
}


void SpriteMap::addAnim(Anim2D* a)
{
	list->insert(make_pair(a->getName(), a));
}


bool SpriteMap::nextFrame() 
{
	//Si se ha alcanzado el n�mero de pasos del juego que hacen falta para cambiar de frame, 
	//reinicio el numero de pasos de juego y aviso de que toca cambiar de frame
	if (step >= currentFramesPerStep) 
	{
		step = 1;
		return true;
	}
	//Si no, aumento el numero de pasos de juego desde el �ltimo cambio de frame, y aviso de que 
	//a�n no toca cambiar
	else 
	{
		step++;
		return false;
	}
}


void SpriteMap::playAnim(string name)
{
	// Creamos un iterador para map
	map<string, Anim2D*>::iterator it;
	//Pido que busque la animaci�n solicitada
	it = list->find(name);
	if (it == list->end())
		//Si no est� hemos acabado
		return;
	else
	{
		//Si no hay animaci�n actual o no es la que me piden, copio la nueva en currentAnim
		if (!(currentAnim && (currentAnim->getName() == name)))
		{
			currentAnim = list->find(name)->second;
			currentFramesPerStep = currentAnim->getFramesPerStep();
			currentLoop = currentAnim->getLoop();
		}
		//Tanto si he tenido que cambiar  de animaci�n como si no, reseteo los parametros de la 
		//animaci�n en curso porque la reproducci�n es nueva
		frame = 0; 
		finished = false;
	}
}
	


void SpriteMap::playAnim(string name, int framesPerStep, bool loop)
{
	//Llamo a playAnim(name) para que haga la parte de reproducir una nueva animaci�n
	playAnim(name);
	
	//Actualizo los parametros de la animaci�n actual para los framesPerStep y la repetici�n
	currentFramesPerStep = framesPerStep;
	currentLoop = loop;
}


void SpriteMap::playAnim(string name, float frameRate, bool loop)
{
	//Llamo a playAnim(name) para que haga la parte de reproducir una nueva animaci�n
	playAnim(name);

	//Actualizo los parametros de la animaci�n actual para los framesPerStep y la repetici�n
	currentFramesPerStep = (int) (currentAnim->getFramesPerStep()*100/frameRate);
	currentLoop = loop;
}


void SpriteMap::stopAnim()
{
	//Si existe animaci�n actual fija como �ltimo frame el acutal
	if (currentAnim)
	{
		lastFrame = currentAnim->getFrameList()->at(frame);
	}
}


void SpriteMap::update()
{
	//Si existe animaci�n, no est� finalizada y toca cambiar de frame
	if (currentAnim && !finished && nextFrame())
	{
		//Cambio de frame y compruebo si he llegado al final
		frame++;
		if (frame >= currentAnim->getFrameCount()) 
		{
			//Si he llegado al final compruebo si tiene repetici�n
			if (!currentLoop) 
			{
				//Si no la tiene marco la animaci�n actual como finalizada, reinicio los steps del 
				//juego desde el �ltimo cambio de frame, pongo el �ltimo frame usado como lastFrame
				//y indico que n�mero fue el ultimo frame reproducido
				finished = true;
				step = 1;
				lastFrame = currentAnim->getFrameList()->at((currentAnim->getFrameCount())-1);
				frame = currentAnim->getFrameCount()-1;
			}
			//Si tiene repetici�n reinicio el frame y dejo que siga
			else frame = 0;
		}
	}
}
	

void SpriteMap::render(int x, int y) 
{
	//Calculo el ancho y el alto de cada tile
	int w = img->getWidth() / nCol;
	int h = img->getHeigth() / nRow;
	
	//Si hay animaci�n actual
	if (currentAnim) 
	{
		//Calculo la posici�n en la imagen del frame actual
		int posnum = currentAnim->getFrameList()->at(frame);
		//La paso a coordenadas cartesianas
		pair<int,int> pos = numToXY(posnum);
		//Renderizo la parte de la imagen que va desde (pos.first*w, pos.second*h), es decir, 
		//la esquina superior izquierda del frame, hasta (pos.first*w + w, pos.second*h +h), 
		//es decir la esquina inferior derecha del frame a dibujar, en la posici�n (x,y) de la
		//imagen por defecto
		gfxEngine->renderPart(img, x, y, pos.first*w, pos.second*h, w, h);
	}
}


pair<int,int> SpriteMap::numToXY(int num)
{
	return make_pair(num % nCol, num / nCol);
}

int SpriteMap::XYToNum(int x, int y)
{
	return x*nCol+y;
}

void SpriteMap::setImg(Image* img)
{
	this->img = img;
}

Image* SpriteMap::getImg()
{
	return img;
}

string SpriteMap::getCurrentAnim()
{
	//Si existe la animaci�n devuelvo el nombre
	if (currentAnim) 
		return currentAnim->getName(); 
	//Si no, digo que no hay animaci�n en curso
	else 
		return "No hay ninguna animaci�n reproduciendose";
}

bool SpriteMap::animFinished()
{
	return finished;
}



