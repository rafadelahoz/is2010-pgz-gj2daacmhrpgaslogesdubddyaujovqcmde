#include "Controller.h"

Controller::Controller(Game* g, GameState* gs, int players) : Entity (0, 0, g, gs)
{
	game = g;
	world = gs;
	width = game->getGameConfig()->gameWidth;
	height = game->getGameConfig()->gameHeigh;
	currentRoom = new Image(width, heigth, game->getGfxEngine(), false, true);
	nextRoom = new Image(width, heigth, game->getGfxEngine(), false, true);
	
	numPlayers = players;
	player = new Player[players];
}
	
Controller::~Controller()
{
}
	
void Controller::onStep()
{

	switch (state) 
	{
		NORMAL: 
			// check player in bounds map
			break;
		TRANSITION:
			// Si no hemos acabado de desplazar la pantalla, continuamos
			if ((mx + sp*xdir) < tx)
				mx += sp*xdir;
			else if ((my + sp*ydir) < ty)
				my += sp*ydir;
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
			
			break;
	}
}


void Controller::onRender()
{
	switch (state) 
		{
			TRANSITION:

				// Durante la transici�n, pintamos el mapa en desplazamiento en la correspondiente posici�n
				game->getGfxEngine()->render(currentRoom, mx, my);
				game->getGfxEngine()->render(nextRoom, mx + xdir*width, my + ydir*heigth);
				
				break;
		}
}

void Controller::change_screen(Dir dir){

	/* ---------------------------------------------------------------------
	1. Preguntar a Data por la pantalla destino. Recibe lista de propiedades
		1.1. Preguntar si la pantalla destino existe
	--------------------------------------------------------------------- */
	
			// Obtenemos la pantalla destino del cambio
			up = left = 0;
			switch (dir) 
			{
				UP: up = -1; break;
				DOWN: up = 1; break;
				LEFT:  left = -1; break;
				RIGTH:  left = 1; break;
			}
			mapId m = GameStatus->getMapId();
			m.mapX += left;
			m.mapY += up;            
			// Preguntamos si la pantalla existe

			if (Data->hasScreen(m))
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

			game->getGfxEngine()->setRenderTarget(currentRoom);
			world->OnRender();
			game->getGfxEngine()->resetRenderTarget();
		   
	/* ---------------------------------------------------------------------
	4. Mapa, c�rgame los datos del nuevo mapa y borra los anteriores ( o buffer ). 
	El nuevo mapa sustituir� al actual, contendr� a los players y el hud y adem�s
	las entidades cargadas deber�n estar disabled (de eso me ocupo yo, Controller).
	--------------------------------------------------------------------- */
	
			// naive code: rethink

			// suponemos que se ocupa de pasar el antiguo mapa al buffer
			if  (!map->loadScreen(m))
				return false;
				
	/* ---------------------------------------------------------------------
	5. Hazle foto al nuevo mapa.
	--------------------------------------------------------------------- */

			game->getGfxEngine()->setRenderTarget(nextRoom);
			world->onRender();
			game->getGfxEngine()->resetRenderTarget();

	/* ---------------------------------------------------------------------
	6. Junta fotos.
	--------------------------------------------------------------------- */
		
		// Estupidez. Need to generalizar a todas las pos (f�cil pero hay sue�o)
	
	/*
			Image* snap = new Image(2*w, h, game->getGfxEngine(), false, true);
	
			// Falta tener en cuenta el offset del mapa, si tiene
			game->getGfxEngine()->render(snap1, 0, 0, snap);
			game->getGfxEngine()->render(snap1, w, 0, snap);
	*/
	
	/* ---------------------------------------------------------------------
	7. Activa player y hud
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

				Data->setMapId(m);
				
	/* ---------------------------------------------------------------------
	9. Colocaci�n del player + preparaci�n para la transici�n
		9.1. Colocaci�n del player
	--------------------------------------------------------------------- */
	
				// Stupid code test
				int x, y;
				switch (dir) 
				{
					UP: x = w / 2; y = h; break;
					DOWN: x = w / 2; y = 0; break;
					LEFT: x = w; y = h / 2; break;
					RIGTH:  x = 0; y = h / 2; break;
				};

	/* ---------------------------------------------------------------------
		9.2. Aplicaci�n de efectos y preparaci�n de la transici�n.
	--------------------------------------------------------------------- */
	
			setStatus(TRANSITION);
			setTransitionEffect(SCROLL);
			
			// Direcci�n expl�cita de la transici�n
			int xdir, ydir
			xdir = ydir = 0;
			
			speed = 4;
			// Origen (mx,my) y destino (tx, ty) de la transici�n
			int mx, my, tx, ty;
			mx = my = tx = ty = 0;
			switch (dir) 
			{			
				UP: 
					ydir = -1;
					ty = height;
					break;
				DOWN: 
					ydir = 1;
					ty = -height;
					break;
				LEFT:  
					xdir = -1; 
					tx = width
					break;
				RIGTH:  
					xdir = 1; 
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


void Controller::setStatus(State st)
{
	state = st;
}

void Controller::setTransitionEffect(TransEffect te)
{
	transtitionEffect = te;
}
	
void Controller::change_map()
{
}
	
void Controller::attack(int idtool, Player* player){
	
	/* ---------------------------------------------------------------------
		1. Interactuar si es necesario:
			1.1. obtener la posici�n frente al player
	--------------------------------------------------------------------- */
			
				// Coordenadas del objeto frente al player
				int nx, ny;
				int up, left;
				up = left = 0;

				switch (player->dir) 
				{
					UP: up = -1; break;
					DOWN: up = 1; break;
					LEFT:  left = -1; break;
					RIGTH:  left = 1; break;
				}

				nx = player->x + D_INTERACT * left;
				ny = player->y + D_INTERACT * up;

	/* ---------------------------------------------------------------------
			1.2. Si el objeto es interactable, interactuar con �l
	--------------------------------------------------------------------- */
			
				// Objeto frente al player
				Entity* e = place_meeting(nx, ny, e);
				
				if (e != NULL)
				{
					// si es un objeto con el se puede interactuar, se har� lo propio
					if (dynamic_cast<iInteractable*>(e))
						e->onInteract(player);
				}
				
	/* ---------------------------------------------------------------------
		2. Si no tiene con qui�n interactuar, efect�a la acci�n 
		de la herramienta.
	---------------------------------------------------------------------*/
		
				// Por ahora, al crear una Tool se devuelve NULL si la herramienta
				// es activa pasiva y ya ha sido creada al equiparse
				else
				{
                   //DBI + Tool_Builder domains
				}
	}
	
}





#endif