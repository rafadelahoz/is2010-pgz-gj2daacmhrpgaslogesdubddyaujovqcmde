#include "EventController.h"
#include "Controller.h"


EventController::EventController(Game* g, GameState* gs, Controller* controller) : Entity(0, 0, g, gs)
{
	this->controller = controller;
	visible = true;
	collidable = false;

	currentTrans.effect = SCROLL;
	currentTrans.direction = NONE;
	currentTrans.speed = -1;

	depth = -5;

	fnt = new TileFont("data/graphics/sprFont_strip94.png", g->getGfxEngine());
	t = new TileTextLabel("Super Zelda",fnt, g->getGfxEngine());
}

EventController::~EventController()
{
	// si
	delete t;
	delete fnt;
};

void EventController::initTransition(TransitionProperties e, Image* oldRoom, Image* newRoom)
{
	// Se pone el controller en modo transition
	controller->setState(Controller::TRANSITION);

	// Se guardan los datos de transisici�n
	currentTrans = e;
	// y las im�genes
	currentRoom = oldRoom;
	nextRoom = newRoom;

	// actualizamos nuestros datos
	speed = e.speed;
	if (oldRoom != NULL)
		width = oldRoom->getWidth(),
		height = oldRoom->getHeigth();
	else width = 0, height = 0;

	switch (e.effect)
	{
	case SCROLL:
		mx = my = tx = ty = 0;
		xdir = 0; ydir = 0;
		switch (e.direction) 
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

		// Player visibles
		/*for (int i = 0; i < controller->getNumPlayers(); i++)
			controller->getPlayer(i)->setVisible(true);*/

		break;
	case FADE:

		for (int i = 0; i < controller->getNumPlayers(); i++)
			controller->getPlayer(i)->setVisible(false);

		fadeOut = true;
		falpha = 1;
		maxCounter = (int) ((float) speed)/2*game->getTargetFPS();
		setTimer(1, maxCounter);

		break;
	}
};

void EventController::onStep()
{
	
	switch (controller->getState()) 
	{
		case Controller::NORMAL: 
			{
				// check player in bounds map
				int i = 0;
				bool out = false;
				Direction dir = NONE;
				// Calculamos si alg�n player se sale del mapa
				while ((i < controller->getNumPlayers()) && !out)
				{ 
					//inside = controller->getScreenMap()->isInBounds(controller->getPlayer(i));
					dir = controller->getScreenMap()->relative_position(controller->getPlayer(i), out);
					i++;
				}

				// Si ocurre, realizamos la transici�n
				if (out)
				{
					// Cambiamos de pantalla
					if ((controller->getPlayer(i-1)->getState() != Player::Normal) || (!controller->moveScreen(dir)))
					{
						Player* p = controller->getPlayer(i-1);
						p->toLastPosition();
					}
				}

				if (game->getInput()->keyReleased(Input::kN1))
				{
					MapLocation m; m.id = 0; m.screenX = 3; m.screenY = 2;
					m.positionX = 4; m.positionY = 6;
					controller->teleportTo(m, controller->getPlayer(0), FADE, false);
				}
				else if (game->getInput()->keyReleased(Input::kN2))
				{
					MapLocation m; m.id = 1; m.screenX = 3; m.screenY = 2;
					m.positionX = 8; m.positionY = 8;
					controller->teleportTo(m, controller->getPlayer(0), FADE, false);
				}
				else if (game->getInput()->keyReleased(Input::kN3))
				{
					MapLocation m; m.id = 2; m.screenX = 3; m.screenY = 2;
					m.positionX = 4; m.positionY = 6;
					controller->teleportTo(m, controller->getPlayer(0), FADE, false);
				};

				if (game->getInput()->keyPressed(Input::kG))
					if (controller->getPlayer(0)->changeState(Player::Attack))
					{
						controller->toolController->attack(1, controller->getPlayer(0));
					}
				else if (game->getInput()->key(Input::kF))
					controller->getPlayer(0)->playAnim(Player::Thrust);

				if (controller->getPlayer(0)->getState() == Player::Attack &&
					((SpriteMap*)controller->getPlayer(0)->graphic)->animFinished())
					controller->getPlayer(0)->changeState(Player::Normal);

				// checkeamos si ha terminado de usarse la herramienta (sistema a mejorar en un futuro muy pr�ximo, solo v�lido para prueba)
				if (controller->toolController->createdTools.count(1) != 0)
				{
					ToolSword* sword = (ToolSword*)controller->toolController->createdTools.at(1);
					if (sword->animFinished())
						 controller->toolController->createdTools.erase(1), 
							controller->game->getGameState()->remove(sword);
				}
				break;
			}
		case Controller::TRANSITION:
			{
				switch (currentTrans.effect)
				{
				case SCROLL:
					// Si no hemos acabado de desplazar la pantalla, continuamos
					if (abs(mx + speed*xdir) < abs(tx))
					{
						mx += speed*xdir;
						for (int i = 0; i < controller->getNumPlayers(); i++)
						{
							controller->getPlayer(i)->x = mx-8;
							if (currentTrans.direction == RIGHT) controller->getPlayer(i)->x += width;
							controller->getPlayer(i)->setVisible(true);
						}
					}
					else if (abs(my + speed*ydir) < abs(ty))
					{
						my += speed*ydir;
						for (int i = 0; i < controller->getNumPlayers(); i++)
						{
							controller->getPlayer(i)->y = my-16;
							if (currentTrans.direction == DOWN) controller->getPlayer(i)->y += height+8;
							controller->getPlayer(i)->setVisible(true);
						}
					}
					// Si hemos acabado, pasamos a estado normal
					else
						controller->endTransition();

					break;

				case FADE:
					if (!fadeOut)
						falpha = 1.f - ((float) getTimer(1))/((float)maxCounter);
					else
						falpha = ((float)getTimer(1))/((float)maxCounter);

					if (getTimer(1) <= 0 && !fadeOut) controller->endTransition();
					break;
				}

				break;
			}

		default:
			break;
	}

	std::string str = "fps: ";
	char buf[256];
	str.append(itoa(game->getFPS(), buf, 10));
	t->setText(str);
}

void EventController::onRender()
{
	switch (controller->getState()) 
	{
	case Controller::TRANSITION:
		switch (currentTrans.effect)
		{
		case SCROLL:
			// Durante la transici�n, pintamos el mapa en desplazamiento en la correspondiente posici�n
			controller->game->getGfxEngine()->render(currentRoom, mx, my);
			controller->game->getGfxEngine()->render(nextRoom, mx - xdir*width, my - ydir*height);
			break;
		case FADE:
			game->getGfxEngine()->renderRectangle(0, 0, width, height, Color::Black);

			if (fadeOut)
				game->getGfxEngine()->renderExt(currentRoom, 0, 0, Color::White, falpha, 1, 1, 0);
			else
				game->getGfxEngine()->renderExt(nextRoom, 0, 0, Color::White, falpha, 1, 1, 0);
			
			for (int i = 0; i < controller->getNumPlayers(); i++)
				controller->getPlayer(i)->setVisible(true),
				controller->getPlayer(i)->graphic->setAlpha(falpha),
				controller->getPlayer(i)->onRender(),
				controller->getPlayer(i)->graphic->setAlpha(1),
				controller->getPlayer(i)->setVisible(false);

			break;
		}

	default:
		break;
	}

	t->render(0, 0);
}


void EventController::onTimer(int timer){

	switch (timer) {

		// multiplayer-warp timer
		case 0:
			/* resolver conflicto de cambio de mapas

			PortInfo winner;

			// get portal with most players in them

			// realiza el cambio de mapa de forma forzada sobre el jugador p
			change_map(winner.MapLocation, winner.p, winner.te, true);*/

			break;

		case 1:
			// fade timer
			if (fadeOut)
			{
				fadeOut = false;
				setTimer(1, maxCounter);
				for (int i = 0; i < controller->numPlayers; i++)
					controller->players[i]->x = xpos, controller->players[i]->y = ypos;
			}
			else
				controller->endTransition();
	}
}