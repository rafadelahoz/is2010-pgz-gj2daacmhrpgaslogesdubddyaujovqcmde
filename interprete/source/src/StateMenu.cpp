#include "StateMenu.h"


StateMenu::StateMenu(int x, int y, Game* game, GameState* gstate) : GameMenuController(x, y, game, gstate)
{
	((GamePlayState*) world)->pauseGameEntities();

	//Creamos el grafico del fondo y el del cursor
	setGraphic(new Stamp("data/graphics/StateMenuBackground.png", game->getGfxEngine()));

	//Elijo el grafico del cursor
	setCursorImage(new Stamp("data/graphics/cursor.png", game->getGfxEngine()));

	//Defino el color que usaremos para tintar las piezas de corazon o objetos clave no conseguidos
	Color colorDisabled = Color(20,20,20);

	//-------------------------------------------------------------------------------------------------
	//Aqui se crea el item para el save y para el exit

	//Creamos la fuente para los textos del menu
	menuFont = new TileFont("data/graphics/sprFont_strip94.png",game->getGfxEngine());

	save = new GameMenuTextItemS("Save", menuFont, 3*game->getGfxEngine()->getGameScreenHeight()/4 + 20,
								3*game->getGfxEngine()->getGameScreenWidth()/4, game, gstate);
	save->setCursorLocation(LEFT);

	exit = new GameMenuTextItemS("Exit", menuFont, 3*game->getGfxEngine()->getGameScreenHeight()/4 + 20,
								3*game->getGfxEngine()->getGameScreenWidth()/4 + 8, game, gstate);
	exit->setCursorLocation(LEFT);
	//--------------------------------------------------------------------------------------------------
	//Aqui se crea el array de objetos clave
	//Creo el array de objetos clave y un iterador para el mismo, asi como un auxiliar
	keyItems = new vector<GameMenuItem*>;
	std::vector<GameMenuItem*>::iterator it = keyItems->begin();
	GameMenuItem* keyItem = NULL;
	
	//Pido el grafico del objeto clave, por ahora me lo invento
	//Pido el numero maximo de objetos clave, que por ahora, tambien me lo invento
	//Y pido el numero de objetos clave conseguidos que tambien me lo invento
	int nKeyObj = 12;
	int nKeyObjObt = 3;

	//Posicion del primer objeto clave
	int a = 10;
	int b = 3*game->getGfxEngine()->getGameScreenHeight()/4;

	//Ahora a�ado los objetos clave
	for (int i = 0; i < nKeyObj;i++)
	{
		keyItem = new GameMenuItem(a, b, game, gstate);
		//Metemos el grafico que me diga ToolController
		keyItem->graphic = (new Stamp("data/graphics/triforce.png", game->getGfxEngine()));
		//Si la no tenemos el objeto clave entonces saldr� griseado
		if (i >= nKeyObjObt)
			keyItem->graphic->setColor(colorDisabled);
		//La a�adimos
		keyItems->insert(it,keyItem);
		it = keyItems->end();

		//Calculamos la posicion del nuevo objeto clave
		a = a + ((3*game->getGfxEngine()->getGameScreenWidth()/4) - keyItem->graphic->getWidth())/nKeyObj;

	}
	//Ahora pongo a NULL el auxiliar de antes
	keyItem = NULL;
	//-------------------------------------------------------------------------------------------------------
	//Aqui se crean las piezas de corazon
	//Se mira a ver cuantas piezas de coraz�n tiene el player, por ahora me lo invento
	int numPieces = 3;

	GameMenuItem* heartPiece = new GameMenuItem((3*game->getGfxEngine()->getGameScreenHeight()/4) + 18 , 8, game, gstate);
	heartPiece->graphic = (new Stamp("data/graphics/bigHeartUL.png", game->getGfxEngine()));
	if(numPieces <= 0)
		heartPiece->graphic->setColor(colorDisabled);
	heartPieces[0] = heartPiece;

	heartPiece = new GameMenuItem(heartPieces[0]->x + heartPieces[0]->graphic->getWidth() , 8, game, gstate);
	heartPiece->graphic = (new Stamp("data/graphics/bigHeartUR.png", game->getGfxEngine()));
	if(numPieces <= 1)
		heartPiece->graphic->setColor(colorDisabled);
	heartPieces[1] = heartPiece;

	heartPiece = new GameMenuItem(heartPieces[0]->x + heartPieces[0]->graphic->getWidth() , heartPieces[0]->y + heartPieces[0]->graphic->getHeight(), game, gstate);
	heartPiece->graphic = (new Stamp("data/graphics/bigHeartDR.png", game->getGfxEngine()));
	if(numPieces <= 2)
		heartPiece->graphic->setColor(colorDisabled);
	heartPieces[2] = heartPiece;

	heartPiece = new GameMenuItem((3*game->getGfxEngine()->getGameScreenHeight()/4) + 18 , heartPieces[0]->y + heartPieces[0]->graphic->getHeight(), game, gstate);
	heartPiece->graphic = (new Stamp("data/graphics/bigHeartDL.png", game->getGfxEngine()));
	if(numPieces <= 3)
		heartPiece->graphic->setColor(colorDisabled);
	heartPieces[3] = heartPiece;

	//------------------------------------------------------------------------------------------------------
	// Aqui se a�aden si se tercia, la brujula el mapa y la llave del jefe
	bool mazmorra = true;

	if (mazmorra)
	{
		bossKey = new GameMenuItem((3*game->getGfxEngine()->getGameScreenHeight()/4) + 17 , heartPieces[3]->y + heartPieces[3]->graphic->getHeight() + 5, game, gstate);
		bossKey->graphic = (new Stamp("data/graphics/bossKeyM.png", game->getGfxEngine()));
		//Si no la tiene se oscurece
		
		map = new GameMenuItem((3*game->getGfxEngine()->getGameScreenHeight()/4) + 17 , bossKey->y + bossKey->graphic->getHeight() + 5, game, gstate);
		map->graphic = (new Stamp("data/graphics/bossKeyM.png", game->getGfxEngine()));

		compass = new GameMenuItem((3*game->getGfxEngine()->getGameScreenHeight()/4) + 17 , map->y + map->graphic->getHeight() + 5, game, gstate);
		compass->graphic = (new Stamp("data/graphics/bossKeyM.png", game->getGfxEngine()));
	}
}

StateMenu::~StateMenu()
{
	if (menuFont)
		delete menuFont,menuFont = NULL;

	if (keyItems)
		delete keyItems, keyItems = NULL;
}

void StateMenu::launch()
{
	//A�adimos todos los elementos al menu
	for (int i = 0; i < keyItems->size(); i++)
		addMenuItem(keyItems->at(i));
	
	//A�adimos el boton de guardar y el de salir
	addMenuItem(save);
	addMenuItem(exit);

	//A�adimos las piezas de coraz�n
	for (int i = 0; i < 4; i++)
		addMenuItem(heartPieces[i]);

	//A�adimos la llave del boss, la brujula y el mapa
	addMenuItem(bossKey);
	addMenuItem(map);
	addMenuItem(compass);

	//Lanzamos el menu
	GameMenuController::launch();
}

void StateMenu::onCancelled(iSelectable* selectable)
{
	quit();
	((GamePlayState*) world)->unpauseGameEntities();
}


void StateMenu::onRender()
{
	if (visible)
	{
		Entity::onRender();
		cursorImage->render(cursorPosX + ((GamePlayState*) world)->getOffset().first, cursorPosY + ((GamePlayState*) world)->getOffset().second);
	}
}