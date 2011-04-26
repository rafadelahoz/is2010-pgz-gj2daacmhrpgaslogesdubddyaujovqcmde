#include "EventController.h"
#include "Controller.h"

#include "MessageDialog.h"
#include "CollectableGameItem.h"
#include "FloorButton.h"
#include "ComponentMelee.h"
#include "ComponentAnim.h"
#include "Enemy.h"
#include "Instantiator.h"


EventController::EventController(Game* g, GameState* gs, Controller* controller) : Entity(0, 0, g, gs)
{
	this->controller = controller;
	visible = true;
	collidable = false;

	currentTrans.effect = SCROLL;
	currentTrans.direction = NONE;
	currentTrans.speed = -1;

	depth = -5;
}

EventController::~EventController()
{
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
		{ 
			std::pair<int, int> off = ((GamePlayState*) world)->getOffset(); 
			mx = tx = off.first;
			my = ty = off.second;
		}
		xdir = 0; ydir = 0;
		switch (e.direction) 
		{			
			case UP: 
				ydir = 1;
				ty += height;
				break;
			case DOWN: 
				ydir = -1;
				ty -= height;
				break;
			case LEFT:  
				xdir = 1; 
				tx += width;
				break;
			case RIGHT:  
				xdir = -1; 
				tx -= width;
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
	CollectableGameItem* it;

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
				{
					controller->toolController->equip(1,controller->getPlayer(0), 0);
				}

				else if (game->getInput()->key(Input::kF))
				{
					controller->toolController->equip(2,controller->getPlayer(0), 1);
				}

				if (game->getInput()->keyPressed(Input::kU))
				{
					int id = 0;
					if (game->getInput()->key(Input::kLCTRL))
						id = 1;
					it = new CollectableGameItem(16*(2+rand()%10), 16*(2+rand()%8), game, world);
					if (id == 0)
						it->init(id, controller->getData()->getMapData(controller->getData()->getGameData()->getGameStatus()->getCurrentMapLocation().id)->getMapStatus(), "data/graphics/key.png", GameItem::ieKEY, 1);
					else
						it->init(id, controller->getData()->getMapData(controller->getData()->getGameData()->getGameStatus()->getCurrentMapLocation().id)->getMapStatus(), "data/graphics/bigHeart.png", GameItem::ieMAXHP, 4);
					world->add(it);
				}
								
				if (game->getInput()->keyPressed(Input::kP))
				{
					// Se crea un puzzle cuya recompensa son +15 llaves (1 sola vez) y una momia (todas las veces que se resuelva el puzzle)

					// Se crea el puzzle
					GamePuzzle* gp = new GamePuzzle(0, controller->getData()->getMapData(controller->getData()->getGameData()->getGameStatus()->getCurrentMapLocation().id)->getMapStatus());
					// Y se a�ade al gstate
					controller->gamePlayState->add(gp);
					// Se crea el bot�n
					int xx = (2+rand()%10)*16, yy = (2+rand()%8)*16;
					FloorButton* pt = new FloorButton(xx, yy, game, world);
					// Y se inicia con el puzzle
					pt->init(gp);
					// Se a�ade al gstate
					controller->gamePlayState->add(pt);
					
					// Se crea el instanciador
					Instantiator* it = new Instantiator(game, world);
					// Se a�ade al gstate
					controller->gamePlayState->add(it);

					// Se crea la recompensa (+15 llaves)
					CollectableGameItem* gi = new CollectableGameItem((2+rand()%10)*16, (2+rand()%8)*16, game, world);
					// Y se inicia
					gi->init(3, controller->getData()->getMapData(controller->getData()->getGameData()->getGameStatus()->getCurrentMapLocation().id)->getMapStatus(), "data/graphics/key.png", GameItem::ieKEY, 15);

					// Se crea la otra "recompensa"
					vector<Component*>* components = new vector<Component*>();
					components->push_back(new ComponentAnim(game,controller));
					components->push_back(new ComponentMelee(game,controller));					
					Enemy* e = new Enemy(112, 96, game, world, components);
					e->init("data/graphics/enemy-octorok.png", 15, 5, 8, 1);

					// Se linka la recompensa al instanciador
					it->addEntity(gi);
					it->addEntity(e);

					// Y se inicia el instanciador
					it->init(gp);
				}

				if (game->getInput()->keyPressed(Input::kE))
				{
					vector<Component*>* components = new vector<Component*>();
					components->push_back(new ComponentAnim(game,controller));
					components->push_back(new ComponentMelee(game,controller));
					Enemy* e = new Enemy(112, 96, game, world, components);
					e->init("data/graphics/enemy-octorok.png", 15, 5, 8, 1);
					world->add(e);
				};

				if (game->getInput()->keyPressed(Input::kM))
				{
					TileFont* font = new TileFont("data/graphics/sprFont_strip94.png", game->getGfxEngine());
					TileSet* fondo = new TileSet("data/graphics/system.png",8,8,game->getGfxEngine());
					MessageDialog* m = new MessageDialog(font,26,4,fondo,game->getGfxEngine(),8,152,game->getGameState(),game);
					m->setText("Ni de $2broma$0 va a funcionar la $2mierda $0de los textos del pfervo y te pienso poner mucho para que se $2rompa$0.\n\nIt is a $p1$2secret$0 to $2EVERYBODY$0.");
					world->add(m);
				};

				if (game->getInput()->keyPressed(Input::kR))
				{
					MapLocation m;
					m.id = 0; m.screenX = 3; m.screenY = 2;
					m.positionX = 4; m.positionY = 6;
					Teleporter* teleport = new Teleporter(m, 100, 100, game, world);
					world->add(teleport);
				};

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
							// Colocamos al player ajustando con el offset del GamePlayState
							controller->getPlayer(i)->x = mx-8-((GamePlayState*)world)->getOffset().first;
							if (currentTrans.direction == RIGHT) controller->getPlayer(i)->x += width;
							controller->getPlayer(i)->setVisible(true);
						}
					}
					else if (abs(my + speed*ydir) < abs(ty))
					{
						my += speed*ydir;
						for (int i = 0; i < controller->getNumPlayers(); i++)
						{
							// Colocamos al player ajustando con el offset del GamePlayState
							controller->getPlayer(i)->setVisible(true);
							controller->getPlayer(i)->y = my-16-((GamePlayState*)world)->getOffset().second;
							if (currentTrans.direction == DOWN) controller->getPlayer(i)->y += height+8;
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
}

void EventController::onRender()
{
	std::pair<int, int> offset = ((GamePlayState*)world)->getOffset();
	switch (controller->getState()) 
	{
	case Controller::TRANSITION:
		switch (currentTrans.effect)
		{
		case SCROLL:
			// Durante la transici�n, pintamos el mapa en desplazamiento en la correspondiente posici�n
			controller->game->getGfxEngine()->render(currentRoom, mx, my);
			controller->game->getGfxEngine()->render(nextRoom, mx - xdir*width, my - ydir*height);
			if (currentTrans.direction == UP)
				controller->getPlayer(0)->onRender();
			break;
		case FADE:
			game->getGfxEngine()->renderRectangle(offset.first, offset.second, width, height, Color::Black);

			if (fadeOut)
				game->getGfxEngine()->renderExt(currentRoom, offset.first, offset.second, Color::White, falpha, 1, 1, 0);
			else
				game->getGfxEngine()->renderExt(nextRoom, offset.first, offset.second, Color::White, falpha, 1, 1, 0);
			
			for (int i = 0; i < controller->getNumPlayers(); i++)
			{
				Player* p = controller->getPlayer(i);
				p->setVisible(true);
				p->graphic->setAlpha(falpha),
				p->x += offset.first;
				p->y += offset.second;
				p->onRender();
				p->x -= offset.first;
				p->y -= offset.second;
				p->graphic->setAlpha(1),
				p->setVisible(false);
			}

			break;
		}

	default:
		break;
	}
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