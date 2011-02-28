#include "FrameControl.h"

// Constructora y destructora

// Simplemente guarda el valor de fpsLimit
// Si fpsLimit < 0 => fpsLimit = 0 (sin l�mite)
FrameControl::FrameControl(int fpsL) {
	if (fpsL < 0) 
		fpsLimit = 0;
	else 
		fpsLimit = fpsL;

	// Inicialmente, fps = 0
	fps = 0;

	window = NULL;
}

// La destructora no necesita liberar nada (window lo libera EngineManager)
FrameControl::~FrameControl() {}

// M�todos de la clase

// Inicializa el sistema de control de frames
bool FrameControl::init(sf::Window* window) {
	// Comprueba que window ha sido instanciado previamente
	if (window != NULL) {
		// Almacena la direcci�n del objeto window
		this->window = window;
		// fpsLimit es el valor con el que se invoc� a la constructora
		window->SetFramerateLimit(fpsLimit);
	}
	return (window == NULL);
}

// Cambia el l�mite de frames por segundo
void FrameControl::setFPSLimit(int fpsL) {
	// Si el control de frames no ha sido inicializado no hace nada
	if (window != NULL) {
		// Almacena el nuevo valor de fpsLimit
		if (fpsL < 0) 
			fpsLimit = 0;
		else 
			fpsLimit = fpsL;

		// Inicia de nuevo el control de frames
		init(window);
	}
}

// Devuelve el valor de fpsLimit
int FrameControl::getFPSLimit() {
	return fpsLimit;
}

// Obtiene el valor medio de los fps a los que "corre" (yeah!) la aplicaci�n
int FrameControl::getFramesPerSecond() {

}

// Devuelve la duraci�n del �ltimo frame
int FrameControl::getDelay() {
	// Comprueba que el control de frames ha sido inicializado correctamente
	if (window == NULL) 
		return 0;
	else 
		return ((int) window->GetFrameTime());
}

// Obtiene el valor de la duraci�n del frame en cada paso del juego
// y actualiza el valor de la media de fps en consecuencia
// Debe llamarse en cada paso del juego
void FrameControl::update() {
	// Si no ha sido inicializado FrameControl no hace nada
	if (window != NULL) {
		// Comprueba cu�nto dur� el �ltimo paso de juego
		float new_delay = (float) getDelay();

		// Si fps = 0 => No hab�a valor medio de fps previo => fps = 1/getDelay
		if (fps == 0)
			fps = (int) (1.0/new_delay);
		// Si no, devuelve la media entre el valor medio acumulado y el nuevo valor
		else
			fps = (int) ((((float) fps) + (1.0/new_delay))/2.0);
	}
}