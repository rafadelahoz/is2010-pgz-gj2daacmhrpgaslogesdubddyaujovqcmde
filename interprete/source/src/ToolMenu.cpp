#include "ToolMenu.h"


ToolMenu::ToolMenu(int x, int y, Game* game, GameState* gstate, int centroX, int centroY, int radio) : GameMenuController(x, y, game, gstate)
{
	((GamePlayState*) world)->pauseGameEntities();
	
	//Incicializamos los atributos del circulo sobre el que se dibujar�n las cosas
	this->centroX = centroX;
	this->centroY = centroY;
	this->radio = radio;


	//Creamos el grafico del fondo y el del cursor
	setGraphic(new Stamp("data/graphics/menuBackground.png", game->getGfxEngine()));
	graphic->setAlpha(0.7);
	setCursorImage(new Stamp("data/graphics/cursorM.png", game->getGfxEngine()));

	//Defino el color que usaremos para tintar las herramientas en uso
	Color colorDisabled = Color(138,138,138);

	//Creamos un GameMenuItem auxiliar par ir a�adiendolos al vector, inicializo el vector y un iterador que necesitar�
	GameMenuItemS* iTool = NULL;
	iTools = new vector<GameMenuItemS*>;
	std::vector<GameMenuItemS*>::iterator it = iTools->begin();

	//Creamos la fuente para el texto que se pintar� debajo
	menuFont = new TileFont("data/graphics/sprFont_strip94.png",game->getGfxEngine());
	
	//Pido las herramientas que posee el player a toolController

	idTools = ((PGZGame*)game)->controller->getToolController()->getEquippableTools();	

	//Calculo de los angulos y con ello los puntos en los que se pintar�n las cosas
	//Centro = 112, 96
	//Radio = 92
	//Ecuaci�n en coordenadas parametricas, con funciones trigonometricas
	//x = centro.x + radio*cos(angulo)
	//y = centro.y + radio*sen(angulo)

	//Numero de herramientas que nos ha dado toolController
	int numElem = idTools.size();
	//360�
	float angulo = 2*Pi;	

	//El angulo de diferencia entre elemento y elemento son 360 entre el numero de elementos
	float fraccion = angulo/(numElem);

	//Creo las variables que ser�n el punto x y el punto y de colocaci�n de las herramientas 
	int a = 0;
	int b = 0;

	//Angulo inicial para que el primer elemento salga arriba en el medio
	angulo = 3*Pi/2;

	//Ahora toca a�adir las herramientas que hemos pedido antes a ToolController una por una
	for (int i = 0; i < numElem;i++)
	{
		//Calculamos la posicion de la nueva herramienta
		a = centroX + radio*cos(angulo);
		b = centroY + radio*sin(angulo);

		iTool = new GameMenuItemS(a, b, game, gstate);
		//Metemos el grafico que me diga ToolController
		iTool->graphic = ((PGZGame*)game)->controller->getToolController()->getToolGraphic(idTools.at(i));
		//Colocamos el cursor
		iTool->setCursorLocation(NONE);
		//Si la herramienta est� equipada por algun player la tintamos oscura para que se sepa que no se puede equipar
		if (((PGZGame*)game)->controller->getToolController()->findEquippedTool(idTools.at(i % numElem)) != -1)
			iTool->graphic->setColor(colorDisabled);
		//La a�adimos
		iTools->insert(it,iTool);
		it = iTools->end();

		angulo = angulo + fraccion;
	}
	//Y ahora a�ado el texto que ir� debajo

	iText = new GameMenuTextItem(((PGZGame*)game)->controller->getToolController()->getToolName(idTools.at(0)), menuFont, 
									0, 170, game, gstate);
	iText->x = game->getGfxEngine()->getGameScreenWidth()/2 - (((FriendlyTileMap*) (iText->graphic))->getWidth() / 2);


	iTool = NULL;
}

ToolMenu::~ToolMenu()
{
	if (menuFont)
		delete menuFont,menuFont = NULL;

	if (iTools)
		delete iTools, iTools = NULL;
}

void ToolMenu::launch()
{
	//A�adimos todos los elementos al menu
	for (int i = 0; i < iTools->size(); i++)
		addMenuItem(iTools->at(i));
	//A�adimos el texto del nombre del arma
	addMenuItem(iText);

	//Lanzamos el menu
	GameMenuController::launch();
}

void ToolMenu::onStep()
{
	GameMenuController::onStep();
}

void ToolMenu::onChosen(iSelectable* selectable)
{
	GameMenuItemS* elem = ((GameMenuItemS*)selectable);
	int i = 0;
	
	//Si el elemento existe lo busco, y si lo encuentro y no est� equipado lo equipo donde me digan
	if (selectable)
	{
		while ((i < iTools->size()) && (elem != iTools->at(i)))
			i++;
		if (((PGZGame*)game)->controller->getToolController()->findEquippedTool(idTools.at(i)) == -1)
			((PGZGame*)game)->controller->getToolController()->equip(idTools.at(i), ((PGZGame*)game)->controller->getPlayer(0), 0);
		quit();
		((GamePlayState*) world)->unpauseGameEntities();
	}
}

void ToolMenu::onCancelled(iSelectable* selectable)
{
	GameMenuItemS* elem = ((GameMenuItemS*)selectable);
	int i = 0;

	//Si el elemento existe lo busco, y si lo encuentro y no est� equipado lo equipo donde me digan
	if (selectable)
	{
		while ((i < iTools->size()) && (elem != iTools->at(i)))
			i++;
		if (((PGZGame*)game)->controller->getToolController()->findEquippedTool(idTools.at(i)) == -1)
			((PGZGame*)game)->controller->getToolController()->equip(idTools.at(i), ((PGZGame*)game)->controller->getPlayer(0), 1);
		quit();
		((GamePlayState*) world)->unpauseGameEntities();
	}
}

void ToolMenu::onRender()
{
	if (visible)
	{
		Entity::onRender();
		cursorImage->render(cursorPosX + ((GamePlayState*) world)->getOffset().first, cursorPosY + ((GamePlayState*) world)->getOffset().second);
	}
}

iSelectable* ToolMenu::getMandatorySelectable(iSelectable* slc, Direction dir)
{
	GameMenuItemS* elem = ((GameMenuItemS*)slc);
	int selectedToolPos;

	//Busco que elemento es el que me han dicho que es desde el que se est� moviento el cursor
	for (int i = 0; i < iTools->size(); i++)
	{
		if(elem == iTools->at(i))
			selectedToolPos = i + iTools->size();
	}
	//Si est� a la derecha del centro del circulo
	if (elem->x >= centroX - 1)
	{
		//Si est� encima del centro del circulo
		if (elem->y <= centroY - 1)
		{
			if (dir == RIGHT || dir == DOWN || dir == DOWNRIGHT)
				elem = iTools->at((selectedToolPos + 1) % (idTools.size()));
			else if (dir == LEFT ||dir == UP||dir == UPLEFT)
				elem = iTools->at((selectedToolPos - 1) % (idTools.size()));
			else if (dir==DOWNLEFT || dir == UPRIGHT)
				elem = iTools->at((selectedToolPos) % (idTools.size()));
		}
		//Si est� debajo del centro del circulo
		else
		{
			if (dir == LEFT || dir == DOWN || dir==DOWNLEFT)
				elem = iTools->at((selectedToolPos + 1) % (idTools.size()));
			else if (dir == RIGHT || dir == UP || dir == UPRIGHT)
				elem = iTools->at((selectedToolPos - 1) % (idTools.size()));
			else if (dir == DOWNRIGHT ||dir == UPLEFT)
				elem = iTools->at((selectedToolPos) % (idTools.size()));
		}
	}
	//Si est� a la izquierda del centro del circulo
	else 
	{
		//Si est� encima del centro del circulo
		if (elem->y <= centroY - 1)
		{
			if (dir == RIGHT || dir == UP || dir == UPRIGHT)
				elem = iTools->at((selectedToolPos + 1) % (idTools.size()));
			else if (dir == LEFT || dir==DOWN || dir==DOWNLEFT)
				elem = iTools->at((selectedToolPos - 1) % (idTools.size()));
			else if (dir == UPLEFT || dir==DOWNRIGHT)
				elem = iTools->at((selectedToolPos) % (idTools.size()));
		}
		//Si est� debajo del centro del circulo
		else
		{
			if (dir == LEFT || dir==UP || dir==UPLEFT)
				elem = iTools->at((selectedToolPos + 1) % (idTools.size()));
			else if (dir == RIGHT || dir==DOWN || dir==DOWNRIGHT)
				elem = iTools->at((selectedToolPos - 1) % (idTools.size()));
			else if (dir == UPRIGHT || dir==DOWNLEFT)
				elem = iTools->at((selectedToolPos) % (idTools.size()));
		}
	}
	//Miro a que herramienta me voy a mover
	int j = 0;
	while ((j < iTools->size()) && (elem != iTools->at(j)))
		j++;

	//Borro el anterior 
	removeMenuItem(iText);

	//Escribo el texto de la misma
	iText = new GameMenuTextItem(((PGZGame*)game)->controller->getToolController()->getToolName(idTools.at(j)),menuFont, 
								0, 170, game, world);
	iText->x = (game->getGfxEngine()->getGameScreenWidth()/2) - (((FriendlyTileMap*) (iText->graphic))->getWidth() / 2);

	//Lo a�ado de nuevo
	addMenuItem(iText);

	//Devuelvo el elemento al que me tengo que mover
	return elem;
}

iSelectable* ToolMenu::getAlternativeSelectable(iSelectable* slc, Direction dir)
{
	return NULL;
}