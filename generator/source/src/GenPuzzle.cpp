#include "GenPuzzle.h"

const void* GenPuzzle::genPuzzle = NULL;

void* GenPuzzle::operator new (size_t size){
	if(genPuzzle == NULL){
		void* p = malloc(size);
		return p;
	}
	else
		return (void*)genPuzzle;
}

GenPuzzle::GenPuzzle(short item, DBManager* db, string zone, string theme, string tileSetPath) {
	if(genPuzzle == NULL){
		genPuzzle = this;
		this->item = item;
		this->db = db;
		decorator = new Decorator();
		decorator->init(zone, theme, tileSetPath);
	}
};

GenPuzzle::~GenPuzzle() {
	delete decorator;
	decorator = NULL;
};

void GenPuzzle::setItem(short item) {this->item = item;}

short GenPuzzle::generate(DunScreen* ds, short id, short type) {
	// EntidadItem que se obtiene tras resolver el puzzle.
	this->item = item;
	
	puzzle_t puzzle;
	
	// Asigno el identificador
	puzzle.id = id;
	
	switch (type) {
		case pARENA: // Arena de enemigos b�sica.
			enemyArena(ds,false,false,id);
			puzzle.type = ARENA;
			break;
		case pLINKEDARENA: // Arena de enemigos enlazada no se pero la precedencia temporal entre puzzles debe estar implicita en su posici�n en el vector de puzzles.
			enemyArena(ds,true,false,id);
			puzzle.type = ARENA;
			break;
		case pBUTTON:
			button(ds, false, true, id);
			puzzle.type = FLOORBUTTON;
			break;
		case pBOSSARENA:
			button(ds, false, false, id);
			puzzle.type = ARENA;
			break;
	}
	// A�ado el puzzle a la lista
	ds->addPuzzle(puzzle);

	return id+1;
}

void GenPuzzle::enemyArena(DunScreen* ds, bool linked, bool persistent, short& id) {
	short n;
	short order = ds->getEntities()->size();

	// Coloco los enemigos linkedto order = idarena
	placeEnemies(ds,order,1);

	int x = -1;
	int y = -1;
	/* type - x y - idCollectable - linkedto - espacio_variable */	
	if(linked && item != -1){	// hay enlace a otro conjunto de entidades de puzzle y recompensa
			if(persistent){ // recompensa en cada enlace
				ds->addEntity(new EntityPuzzleElement(ARENA,x,y,-1,id));
				ds->addEntity(new EntityPuzzleElement(INSTANCIATOR,x,y,-1,id));
				// order + 1 = linkedTo instanciator 
				ds->addEntity(placeItem(ds,order+1));
				n = rand() % NPUZZLES;
				id++;
				this->generate(ds,id,n);
			}
			else{ // hay enlace y no item luego nuevo puzzle.
				ds->addEntity(new EntityPuzzleElement(ARENA,x,y,-1,id));
				n = rand() % NPUZZLES;
				id++;
				this->generate(ds,id,n);
			}
	}
	else{
		if(item != -1){ // hay recompensa pero no enlace
			ds->addEntity(new EntityPuzzleElement(ARENA,x,y,-1,id));
			ds->addEntity(new EntityPuzzleElement(INSTANCIATOR,x,y,-1,id));
			// order + 1 = linkedTo instanciator 
			ds->addEntity(placeItem(ds,order+1));
		}
		else
			if(linked){ // no hay recompensa pero si enlace 
				ds->addEntity(new EntityPuzzleElement(ARENA,x,y,-1,id));
				n = rand() % NPUZZLES;
				id++;
				this->generate(ds,id+1,n);
			}
			else{ // Caso ni item ni enlace
				ds->addEntity(new EntityPuzzleElement(ARENA,x,y,-1,id));
			}
	}
}

void GenPuzzle::bossArena(DunScreen* ds, bool linked, bool persistent, short& id) {
	short n;
	short order = ds->getEntities()->size();

	// Coloco los enemigos linkedto order = idarena
	placeEnemies(ds,order,5);

	int x = -1;
	int y = -1;
	/* type - x y - idCollectable - linkedto - espacio_variable */	

	ds->addEntity(new EntityPuzzleElement(ARENA,x,y,-1,id));
	ds->addEntity(new EntityPuzzleElement(DOOR_OPEN_CLOSE,x,y,-1,id));
	ds->addEntity(new EntityPuzzleElement(INSTANCIATOR,x,y,-1,id));
	// order + 1 = linkedTo DOOR_OPEN_CLOSE
	addDoors(ds,order+1);
	// order +2 = linkedTo INSTANCIATOR
	ds->addEntity(placeItem(ds,order+2));
}

void GenPuzzle::button(DunScreen* ds, bool linked, bool persistent, short& id) {
	short first_entity = ds->getEntities()->size();
	if (linked) {
	}
	else {
		if (persistent) {
			// Colocamos un bot�n
			int x = 0, y = 0;
			get_valid_position(ds, &x, &y);
			// Colocamos un FloorButton enlazado al id del puzzle actual
			ds->addEntity(new EntityFloorButton(FLOORBUTTON, x, y,-1, id, decorator->gimmeFloorButton()));		// linkedTo: puzzleId
			// Colocamos un instantiator enlazado al id del puzzle actual
			ds->addEntity(new Entity(INSTANCIATOR, -1, -1, -1, id));	// linkedTo: puzzleId
			// Colocamos un bloque empujable (que no est� pegado a la pared), enlazado al FloorButton que colocamos antes
			do { get_valid_position(ds, &x, &y); }
			while (x == ds->getWall_size() || y == ds->getWall_size() ||
				   x == SCREEN_WIDTH - ds->getWall_size() - 1 ||
				   y == SCREEN_HEIGHT - ds->getWall_size() - 1);
			// Hay que pedir el gr�fico del TiledPushable
			ds->addEntity(new EntityTiledPushable(TILEDPUSHABLE, x, y, -1, -1, decorator->gimmeTile()));	// linkedTo: -1
			// Colocamos la recompensa, enlazada al instantiator
			ds->addEntity(placeItem(ds, first_entity+1));									// linkedTo: INSTANTIATOR
		}
		else {
		}
	}
}

EntityItem* GenPuzzle::placeItem(DunScreen* ds, short linkedTo){
	EntityItem* e = new EntityItem(ITEM, -1, -1, -1, -1, -1, -1, 1);

	int x = 0, y = 0;
	get_valid_position(ds, &x, &y);
	
	e->x = x; e->y = y;
	
	switch(item){
		case(KEY):{
				e->idCollectable = -1;
				e->effect = KEY;
				e->gfxId = db->getKeyGfxId();
				e->linkedTo = linkedTo;
				}
				break;
	};

	return e;
}

void GenPuzzle::placeEnemies(DunScreen* ds, short linkedTo,int nEnemies){
	short e = -1;
	short n = rand() % 5 + nEnemies;
	// Pide un enemigo v�lido a la interfaz con la base de datos
	e = db->getEnemy(ds->getZone());
    if(e != -1)
		for (int i = 0; i < n; i++) {
			// Escoge una localizaci�n v�lida en la habitaci�n
			int x = 0, y = 0;
			get_valid_position(ds, &x, &y);
			// Al salir del bucle, x e y representan una posici�n v�lida para el enemigo e
			enemy en;
			// LinkedTo arena
			en.id = e;
			en.linkedTo = linkedTo;
			en.posX = x;
			en.posY = y;

			ds->addEnemy(en);
		}
}

void GenPuzzle::addDoors(DunScreen* ds,int order) {

	int next = order;

	for(int i = 0; i < 4; i++){
		if(ds->getDoor(i))
			int i;
			//ds->addEntity(new EntityDOORSINLLAVE(NOMBRE, x, y, -1, next));	Por decidir nueva entidad o la Entity con alg�n flag?
	}
}

void GenPuzzle::get_valid_position(DunScreen* ds, int* x, int* y) {
	int s;
	do {
		*x = rand() % (SCREEN_WIDTH - ds->getWall_size()*2) + ds->getWall_size();
		*y = rand() % (SCREEN_HEIGHT - ds->getWall_size()*2) + ds->getWall_size();
		s = ds->getSolid(*x, *y);
	}
	while (s != 0 || ds->blocksDoor(*x, *y));
}