#include "EngineManager.h"

/***********************************************
 *			Funciones Privadas				   *
 ***********************************************/

/* EngineManager::init() */
/*
	Esta funci�n se encarga de inicializar todos los atributos de la clase
	para ello llama a los init de cada uno de ellos y comprueba que devuelvan
	true, en caso contrario escribe en el log del sistema la localizaci�n del
	error y devuelve false.
	Si todo funciona correctamente devuelve true.
*/
bool EngineManager::init()
{
	try
	{
		if(!gfxEngine->init((sf::RenderWindow *)window))
		{
			log("EngineManager.init - Se ha producido un problema iniciando en Subsistema Gr�fico");
			return false;
		}

		if(!frameControl->init(window))
		{
			log("EngineManager.init - Se ha producido un problema iniciando en Subsistema de Temporizaci�n");
			return false;
		}

		if(!soundEngine->init())
		{
			log("EngineManager.init - Se ha producido un problema iniciando en Subsistema de Audio");
			return false;
		}

		if(!input->init(window))
		{
			log("EngineManager.init - Se ha producido un problema iniciando en Subsistema de Entrada");
			return false;
		}

		return true;
	}
	catch(exception& e)
	{
		logger->log(strcat("EngineManager::init - Excepci�n capturada: ", e.what()));
		return false;
	}
}

/***********************************************
 *			Funciones P�blicas				   *
 ***********************************************/

/* EngineManager::EngineManager(int screenW, int screenH, int screenBPP, int gameW = -1, int gameH = -1, int gameScale = -1) */
/*
	Preparamos todos los motores y luego los iniciamos mediante init()
*/
EngineManager::EngineManager(int screenW, int screenH, int screenBPP, int fps, int gameW, int gameH, int gameScale)
{
	gfxEngine = new GfxEngine(screenW, screenH, screenBPP, gameW, gameH, gameScale);
	frameControl = new FrameControl(fps);
	soundEngine = new SoundEngine();
	input = new Input();
	logger = Logger::Instance();
	try{
		window = new sf::RenderWindow();
	}catch(exception& e)
	{
		logger->log(strcat("EngineManager::EngineManager - Excepci�n capturada: ", e.what()));
	}

	logger->log("Iniciando Subsistemas...");
	
	// Inicializaci�n de los elementos
	if(!init())
	{
		log("Se produjeron errores al iniciar los Subsistemas.");
	}
}

/* EngineManager::~EngineManager() */
/*
	Eliminamos los distintos atributos y dejamos los punteros apuntando a NULL
*/
EngineManager::~EngineManager()
{
	// Notify
	logger->log("Finalizando Subsistemas...");

	delete gfxEngine;
	gfxEngine = NULL;

	delete frameControl;
	frameControl = NULL;

	delete soundEngine;
	soundEngine = NULL;

	delete input;
	input = NULL;

	logger->log("Subsistemas finalizados correctamente");

	logger->log(randomEndMessage());

	Logger::DestroyInstance();
	logger = NULL;
}
		
GfxEngine* EngineManager::getGfxEngine()
{
	return gfxEngine;
}

FrameControl* EngineManager::getFrameControl()
{
	return frameControl;
}
		
SoundEngine* EngineManager::getSoundEngine()
{
	return soundEngine;
}
		
Input* EngineManager::getInput()
{
	return input;
}

/* EngineManager::log(char* message) */
/*
	Escribimos un mensaje en el log utilizando las funciones de la clase Logger
*/
void EngineManager::log(char* message)
{
	logger->log(message);
}

const char* EngineManager::randomEndMessage()
{
	// Se devuelve un divertido a la par que informativo mensaje de hasta la vista para el log
	int numMsg = 10;

	switch (rand()%numMsg)
	{
		case 0: return "�Gracias por su visita!"; break;
		case 1: return "Por favor, vuelva otro d�a"; break;
		case 2: return "Hasta la vista, baby"; break;
		case 3: return "I'll be back"; break;
		case 4: return "THE END"; break;
		case 5: return "Que tenga un buen d�a"; break;
		case 6: return "Hasta que nos olamos"; break;
		case 7: return "Thanks for your patronage"; break;
		case 8: return "Fare thee well"; break;
		case 9: return "Godspeed, traveller"; break;
		default: return "ERROR404: MESSAGE_DEFAULT_NOT_FOUND"; break;
	};
};