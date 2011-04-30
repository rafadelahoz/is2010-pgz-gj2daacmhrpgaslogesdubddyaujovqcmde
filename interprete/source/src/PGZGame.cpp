#include "PGZGame.h"


PGZGame::PGZGame() : Game(224*3, 224*3, 32, 224, 224, 3, 30)
{
	// Se inicializa la semilla del generador de n�meros aleatorios
	srand((unsigned int) time(NULL));

	// Se prepara la ventana de aplicaci�n
	// Se establece el color de fondo
	gfxEngine->setScreenBackgroundColor(Color(20, 20, 25));

	// Se instancia el GameState inicial y se activa

	controller = new Controller(this);
	controller->loadInputConfig(controller->mainInputConfig, "config-p1"); 
	gameState = new MainMenuState(224, 224, this);
}

PGZGame::~PGZGame()
{
	if (controller != NULL)
		delete controller, controller = NULL;

	/*if (gameState != NULL)
		delete gameState, gameState = NULL;*/
}

void PGZGame::onStep()
{
	// Eventos generales a todo el juego
	// Por ahora, ESCAPE para salir (luego habr� de cambiarse)
	if (getInput()->keyPressed(Input::kESC))
		setFinished(true);

	// Depuraci�n
	// Zoom del juego
	if (getInput()->keyPressed(Input::kO))
		gfxEngine->setGameScreenScale(min(gfxEngine->getGameScreenScaleH()+1, 3), min(gfxEngine->getGameScreenScaleV()+1, 3));
	else if (getInput()->keyPressed(Input::kL))
		gfxEngine->setGameScreenScale(max(gfxEngine->getGameScreenScaleH()-1, 1), max(gfxEngine->getGameScreenScaleV()-1, 1));
};


void PGZGame::startNewgame()
{
	controller->initData("");
			
	GamePlayState* gameState = new GamePlayState(224, 224, this);

	changeGameState(gameState);
	controller->initGamePlayState((GamePlayState*) gameState);
}