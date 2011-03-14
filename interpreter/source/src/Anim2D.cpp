#include "Anim2D.h"



//Constructora parametrizada
Anim2D::Anim2D(string name, vector<int>* frames, int framesPerStep, bool loop):Anim(name)
{
	//Asigno todos los atributos con su parametro de entrada correspondiente
	this->name = name;
	this->framesPerStep = framesPerStep;
	this->loop = loop;
	this->frameList = frames;
	logger = Logger::Instance();
	//Si la lista que me han pasado no es NULL asigno su tama�o a la variable frameCount
	if (frameList != NULL)
		frameCount = frameList->size();
	else 
		logger->log("Se ha creado una animaci�n con una lista de frames vacia, cuidadin al acceder");
}

//Destructora
Anim2D::~Anim2D()
{
	//Borro el frameList
	delete frameList;
	frameList = NULL;
}

//Devuelve el numero de steps que se ejecuta un frame
int Anim2D::getFramesPerStep()
{
	return framesPerStep;
}

//Modifica el numero de steps que se ejecuta un frame
void Anim2D::setFramesPerStep(int framesPerStep)
{
	this->framesPerStep = framesPerStep;
}

//Devuelve si la animaci�n se repetir� al acabar o no
bool Anim2D::getLoop()
{
	return loop;
}

//Modifica si la animaci�n se repetir� al acabar o no
void Anim2D::setLoop(bool loop)
{
	this->loop = loop;
}

//Devuelve el nombre de la animaci�n
string Anim2D::getName()
{
	return name;
}

//Devuelve el n�mero de frames que tiene la animaci�n
int Anim2D::getFrameCount()
{
	return frameCount;
}

//Devuelve la lista de frames
vector<int>* Anim2D::getFrameList()
{
	return frameList;
}








