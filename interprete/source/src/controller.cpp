#include "controller.h"

Controller::Controller(std::string path, Game* g, GameState* gs): Entity (0, 0, g, gs)
{
	// Almacena los par�metros necesarios
	game = g;
	world = gs;
	width = game->getGameConfig()->gameWidth;
	height = game->getGameConfig()->gameHeight;

	// Crea las im�genes que usar� para el cambio de posici�n del mapa
	currentRoom = new Image(width, height, game->getGfxEngine(), false, true);
	nextRoom = new Image(width, height, game->getGfxEngine(), false, true);
	
	
	init(path);
}
	
Controller::~Controller()
{
}
	
bool Controller::init(std::string path)
{

/* ---------------------------------------------------------------------
1.	Recupera los datos del archivo de config en path y, en el caso de 
	que este sea NULL, saca los valores necesarios de la DBI
--------------------------------------------------------------------- */

	// load_slot(path) or DBI

/* ---------------------------------------------------------------------
2.	Con los datos recuperados, crea Data y la inicia.
--------------------------------------------------------------------- */

	// gameData = new GameData();
	// gameData->init(stuff);

/* ---------------------------------------------------------------------
3.	Apartir de lo recuperado en el archivo de config, carga el layout del
	mapa actual y se lo da a Data (MapState)
--------------------------------------------------------------------- */

	// loadmap(path)
	// cargar en Data

/* ---------------------------------------------------------------------
4.	Carga la pantalla actual en su totalidad, y por cada entidad que
	cargue, la env�a a GameState al add sobrecargado. Al final
	se llama al init de ScreenMap con todos los datos cargados.
--------------------------------------------------------------------- */

	// load_screen()

/* ---------------------------------------------------------------------
5.	Crea los players en la posici�n configurada de la pantalla del
	mapa actual.
--------------------------------------------------------------------- */

	// init array de players a 4 (for si las flies se a�ade a mitad de partida uno)
	// create each player con info de dbi about hero

/* ---------------------------------------------------------------------
6.	Crea el hud adecuado a la cantidad de players.
--------------------------------------------------------------------- */

	// hud controller = new controller
	// for each player
	// hud controller->addhud(player[i])

/* ---------------------------------------------------------------------
7.	Playea todos los elementos que ha creado de la nueva pantalla.
--------------------------------------------------------------------- */

	return true;
}

void Controller::onStep()
{

	switch (state) 
	{
		case NORMAL: 
			{
				// check player in bounds map
				int i = 0;
				bool inside = true;
				// Calculamos si alg�n player se sale del mapa
				while ((i < numPlayers) && inside)
				{ 
					inside = screenMap->isInBounds(players[i]);
					i++;
				}

				// Si ocurre, realizamos la transici�n
				if (!inside)
				{
					// Calculamos por donde se ha salido
					Dir dir = screenMap->relative_position(players[i - 1]);
					// Cambiamos de pantalla
					move_to_screen(dir);
				}
				break;
			}
		case TRANSITION:
			{
				if (transitionEffect == SCROLL)
				{
					// Si no hemos acabado de desplazar la pantalla, continuamos
					if ((mx + speed*xdir) < tx)
						mx += speed*xdir;
					else if ((my + speed*ydir) < ty)
						my += speed*ydir;
					// Si hemos acabado, pasamos a estado normal
					else{
				
						// Activamos al player
						for (int i = 0; i < numPlayers; i++)
						{
							players[i]->setVisible(false);
							players[i]->disable();
						}
				
						//  Activamos el hud
						hud->disable();
				
						// Activamos el resto de entidades
						// TO BE DONE
				
						setState(NORMAL);
					}
				}
				break;
			}

		default:
			break;
	}
}


void Controller::onRender()
{
	switch (state) 
	{
		case TRANSITION:
			if (transitionEffect == SCROLL)
			{
				// Durante la transici�n, pintamos el mapa en desplazamiento en la correspondiente posici�n
				game->getGfxEngine()->render(currentRoom, mx, my);
				game->getGfxEngine()->render(nextRoom, mx + xdir*width, my + ydir*height);
			}
			break;

		default:
			break;
	}
}

bool Controller::move_to_screen(Dir dir){

/* ---------------------------------------------------------------------
1. Preguntar a Data por la pantalla destino. Recibe lista de propiedades
	1.1. Preguntar si la pantalla destino existe
--------------------------------------------------------------------- */
	
	// Obtenemos la pantalla destino del cambio
	int up; int left;
	up = left = 0;
	switch (dir) 
	{
		case UP:	up = -1; break;
		case DOWN:	up = 1; break;
		case LEFT:	left = -1; break;
		case RIGHT:	left = 1; break;
		default:	break;
	}

	/* Obtener el mapLocation de Data, rollo:

	MapId m;
	 
	m.mapX = m.mapX + left;
	m.mapY = m.mapY + up;       
	*/
 
	// Preguntamos si la pantalla existe
	if (true /*data->hasScreen(m)*/)
	{
			
/* ---------------------------------------------------------------------
	1.2. Obtenemos las propiedades de la pantalla destino.
--------------------------------------------------------------------- */
	
			// Ya est�n en m, lala(8)
			   
/* ---------------------------------------------------------------------
2. Desvisivilizar player y hud
	2.1. Desvisibilizar player
--------------------------------------------------------------------- */

		for (int i = 0; i < numPlayers; i++)
			players[i]->setVisible(false);
				
/* ---------------------------------------------------------------------
	2.2. Desvisibilizar hud
--------------------------------------------------------------------- */

		hud->setVisible(false);       

/* ---------------------------------------------------------------------
3. Hace foto y la guarda
--------------------------------------------------------------------- */

		// Limpiar currentRoom si es necesario
		game->getGfxEngine()->setRenderTarget(currentRoom);
		world->onRender();
		game->getGfxEngine()->resetRenderTarget();
		   
/* ---------------------------------------------------------------------
4. Mapa, c�rgame los datos del nuevo mapa y borra los anteriores ( o buffer ). 
El nuevo mapa sustituir� al actual, contendr� a los players y el hud y adem�s
las entidades cargadas deber�n estar disabled (de eso me ocupo yo, Controller).
--------------------------------------------------------------------- */

		// think
				
/* ---------------------------------------------------------------------
5. Hazle foto al nuevo mapa.
--------------------------------------------------------------------- */

		// Limpiar nextRoom si es necesario
		game->getGfxEngine()->setRenderTarget(nextRoom);
		world->onRender();
		game->getGfxEngine()->resetRenderTarget();

/* ---------------------------------------------------------------------
6. Junta fotos.
--------------------------------------------------------------------- */
		
		// No es necesario si pintamos las 2 directamente en la pantalla de juego
		// Nos ahorramos una pantalla auxiliar

/* ---------------------------------------------------------------------
7. Visibiliza player y hud
	7.1 Hace visible el player. Ser� necesario adem�s disablearlo para 
	que no pueda moverse durante la transici�n
--------------------------------------------------------------------- */
	
		for (int i = 0; i < numPlayers; i++)
		{
			players[i]->setVisible(false);
			players[i]->disable();
		}
	
/* ---------------------------------------------------------------------
	7.2. Hace visible el Hud y lo disablea por el mismo motivo.
--------------------------------------------------------------------- */
	
			hud->setVisible(true);
			hud->disable();

/* ---------------------------------------------------------------------
8. Actualiza los datos con la nueva pos del player en el mapa.
--------------------------------------------------------------------- */

			// Igual ha de hacerse antes de cargar el nuevo ScreenMap
				
/* ---------------------------------------------------------------------
9. Colocaci�n del player + preparaci�n para la transici�n
	9.1. Colocaci�n del player
--------------------------------------------------------------------- */
	
			// PONER OFFSET A LOS MAPAS

			// Stupid code test
			int x, y;
			switch (dir) 
			{
				case UP: 
					// Mantenemos la x y cambiamos la y
					y = height; 
					break;
				case DOWN:
					// Mantenemos la x y cambiamos la y
					y = 0; 
					break;
				case LEFT:
					// Mantenemos la y y cambiamos la x
					x = width;
					break;
				case RIGHT: 
					// Mantenemos la y y cambiamos la x
					x = 0;
					break;
			};

/* ---------------------------------------------------------------------
	9.2. Aplicaci�n de efectos y preparaci�n de la transici�n.
--------------------------------------------------------------------- */
	
		setState(TRANSITION);
		setTransitionEffect(SCROLL);
			
		// Direcci�n expl�cita de la transici�n
		int xdir, ydir;
		xdir = ydir = 0;
			
		speed = 4;
		// Origen (mx,my) y destino (tx, ty) de la transici�n
		int mx, my, tx, ty;
		mx = my = tx = ty = 0;
		switch (dir) 
		{			
			case UP: 
				ydir = 1;
				ty = height;
				break;
			case DOWN: 
				ydir = -1;
				ty = -height;
				break;
			case LEFT:  
				xdir = 1; 
				tx = width;
				break;
			case RIGHT:  
				xdir = -1; 
				tx = -width;
				break;
		}

/* ---------------------------------------------------------------------
10. Finalizaci�n
--------------------------------------------------------------------- */
		return true;
	}
	else
		return false;
}


bool Controller::change_map(MapId m, Player* p, TransitionEffect te, bool brute){

/* -----------------------
	STAAART!

	Si brute est� activado, ir a 1 directamente.

	A. Ir al map de portales activos y aumentar en 1 el contador de �ste (el id del portal puede ser interno), si no existe se pone a 1.
	B. Si el contador aumentado es mayor que la mitad de los players
		B1. Hacer la correspondiente animaci�n del player con el portal.
		B2. Desactivar al player.
		B3.ir a 1.
	C. Si el contador es menor, y es el primer portal que se toca (easy, si el mapa estaba vac�o al entrar al m�todo), poner un timer(0, n?)
		C1. Hacer la correspondiente animaci�n del player con el portal.
		C2. Desactivar al player.
		C3. return (false?)

	1. Preguntar a la DBI si existe el mapa destino. (b�sicamente pedirle el archivo correspondiente al id y si le da caca pues caca no vale)
		1y. Si existe, obtener v�a DBI el archivo del mapa (done en 1), cargarlo (loadmap) y meter la info Data (no inclu�do en loadmap, loadmap devuelve la info).
		1n. Si no existe, el portal no tiene efecto [return false]

	// A partir de aqu�, �dem al cambio de pantalla (�reusar c�digo?)

	2. Preguntar a Data si existe la pantalla dada (rezando que Data tenga esa funci�n).
		2y. Si no, elegir la pantalla de inicio del mapa (cargada en 1a).
		2n. Si existe, ir a 3.
	3. Hacer invisibles a los player y al hud
	4. Hacer foto
	5. Cargar la pantalla destino.
	6. Dar la info cargada a ScreenMap y GameState. Las entidades que se carguen deber�n disablearse.
	7. Foto
	8. Visibilizar players y hud; disablearlos para la transici�n.
	9. Preparar los efectos de la transici�n
	10. Fin! [return true]

	FINIISH!
----------------------- */

	return true;
}

void Controller::onTimer(int timer){

	switch (timer) {

		// multiplayer-warp timer
		case 0:
			/* resolver conflicto de cambio de mapas

			PortInfo winner;

			// get portal with most players in them

			// realiza el cambio de mapa de forma forzada sobre el jugador p
			change_map(winner.mapId, winner.p, winner.te, true);*/

			break;
	}
}


void Controller::setState(State st)
{
	state = st;
}

void Controller::setTransitionEffect(TransitionEffect te)
{
	transitionEffect = te;
}