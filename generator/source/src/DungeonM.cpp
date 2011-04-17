#include "DungeonM.h"

DungeonM::DungeonM(string zone, string theme, short gameDiff, short dungNumber, short ratio, short tool, DBManager* db) :
    Dungeon(zone, theme, gameDiff, dungNumber, ratio, tool, db) {
	// Por implementar db->getBoss()
    boss = 0;
	layout = NULL;
	areas = NULL;
}

DungeonM::~DungeonM() {
    // Libera la memoria de la matriz de pantallas, sin liberar las propias pantallas
    for (int i = 0; i < width; i++) {
        delete layout[i];
        layout[i] = NULL;
    }
    delete layout; layout = NULL;
    // Libera la memoria de las zonas de la mazmorra, liberando a su vez las pantallas
    for (int i = 0; i < n_areas; i++) {
        delete areas[i];    // La destructora de vector ya llama a la destructora de DunScreen
        areas[i] = NULL;
    }
    delete areas; areas = NULL;
	// Libera la memoria de la matriz de habitaciones por zona
	for (int i = 0; i < width; i++) {
		delete room_areas[i];
		room_areas[i] = NULL;
	}
	delete room_areas; room_areas = NULL;
}

void DungeonM::calculate_size() {
    // Tal cual est� pensado (y hecho) el algoritmo, requiere al menos 3 zonas (para separar las entidades principales)
    // Y como debe ser tan ancho y alto como �reas tenga, la mazmorra m�s peque�a ser� de 3x3 con 3 zonas (9 casillas)
	short hab = numDungeon + difficulty - 2;	// N�mero de habitaciones a a�adir a las b�sicas (anchoxalto)
	width = 3; height = 3;						// Valores iniciales
    for (int i = 0; i < hab; i++) {				// Repartimos las habitaciones extras
		if (rand() % 2 == 0) width++;
		else height ++;
	}
    n_areas = min(width, height);				// El n�mero de �reas ser� el m�ximo posible
}

void DungeonM::create_rooms() {
    // Utiliza una matriz de punteros a DunScreen para mayor comodidad
    layout = new DunScreen**[width];
    for (int i = 0; i < width; i++)
        layout[i] = new DunScreen*[height];	// ahora hay una fila m�s para meter las habitaciones extra
	
	// Introducimos las habitaciones extra
	for (int i = 0; i < width; i++) layout[i][bossY] = NULL;
	int j1, j2;
	if (bossY == 0) {
		j1 = 1;
		j2 = height;
	}
	else {
		j1 = 0;
		j2 = height -1;
	}
	layout[bossX][bossY] = boss_screen;
	layout[finalX][finalY] = final_screen;

	for (int i = 0; i < width; i++) {
        for (int j = j1; j < j2; j++) {
            // Creamos las pantallas vac�as, en la posici�n que se indica, con la zona que se indica
			DunScreen* s;
            if (rand() % 100 <= ratio) {	// Colocamos una pantalla con enemigos o con puzzle seg�n el ratio
				// Pantalla con enemigos
				short n_enemies = rand() % (difficulty + numDungeon) + 1;
				s = new DunScreen(i, j, -1, n_enemies, -1, -1, -1, zone, theme, db, numDungeon);
			}
			else {
				// Pantalla con puzzle
				// short puzzle = db->getPuzzle();
				s = new DunScreen(i, j, 0, -1, -1, -1, -1, zone, theme, db, numDungeon);
			}
            layout[i][j] = s;
        }
    }
}

bool DungeonM::check_room(short x1, short y1, short x2, short y2, short area, short room) {
	if (areas[area]->at(room)->getPosX() == x2 && areas[area]->at(room)->getPosY() == y2) {
		areas[area]->push_back(layout[x1][y1]);
		room_areas[x1][y1] = area;
		return true;
	}
	else return false;
}

void DungeonM::add_isolated_rooms() {
	// Recorremos la matriz de pantallas/zona
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			// Vemos si la pantalla actual no ha sido asignada a ninguna zona
			if (room_areas[i][j] == -1) {
				// Asignamos dicha pantalla a una zona adyacente
				int k = 0;
				bool found = false;
				while (k < n_areas && !found) {
					int l = 0;
					while (l < areas[k]->size() && !found) {
						int d = 0;
						while (d < 4 & !found) {
							switch (d) {
								case UP: 
									found = check_room(i, j, i, j-1, k, l); 
									break;
								case DOWN: 
									found = check_room(i, j, i, j+1, k, l); 
									break;
								case LEFT: 
									found = check_room(i, j, i-1, j, k, l); 
									break;
								case RIGHT: 
									found = check_room(i, j, i+1, j, k, l); 
									break;
							}
							d++;
						}
						l++;
					}
					k++;
				}
			}
		}
	}
}

bool DungeonM::add_room_to_area(short x, short y, short area) {
	if (x >= 0 && x < width && y >= 0 && y < height && room_areas[x][y] == -1) {
		room_areas[x][y] = area;
		areas[area]->push_back(layout[x][y]);
		return true;
	}
	else return false;
}



void DungeonM::divide_into_areas() {
	 // Creamos las �reas
    areas = new vector<DunScreen*>*[n_areas];
    for (int i = 0; i < n_areas; i++)
        areas[i] = new vector<DunScreen*>();

	// Inicializamos las habitaciones al �rea -1
	room_areas = new int*[width];
	for (int i = 0; i < width; i++) {
		room_areas[i] = new int[height];
		for (int j = 0; j < height; j++) {
			room_areas[i][j] = -1;
		}
	}

	// Marcamos la fila extra con -2
	for (int i = 0; i < width; i++)
		room_areas[i][bossY] = -2;

	int screens_per_area = width*height/n_areas;	// N�mero medio de habitaciones por zona
	// Hacer esto para cada una de las �reas
	for (int i = 0; i < n_areas; i++) {
		// Elegimos una pantalla no asignada a zona al azar
		int s = 0, x, y;
		while (s != -1) {
			x = rand() % width;
			y = rand() % height;
			s = room_areas[x][y];
		}
		// A�adimos la pantalla correspondiente a la zona actual
		room_areas[x][y] = i;
		areas[i]->push_back(layout[x][y]);
		// Cada zona deber� tener, como m�ximo w*h/n_areas pantallas (asignamos a esa zona tantas como se puedan sin pasarse)
		int n_screens = 1;		// N�mero de pantallas que tiene la zona actual
		bool no_mov = false;	// �Se puede ir desde una pantalla de la zona a otra que no est� asignada a ninguna?
		// Expandimos la zona actual de manera aleatoria si quedan habitaciones por a�adir y si no hemos agotado las posibilidades
		while (n_screens < screens_per_area && !no_mov) {
			// Recorremos las pantallas de la zona actual
			int visited_rooms = 0;
			for (int j = 0; j < areas[i]->size(); j++) {
				visited_rooms++;
				// Apuntamos las coordenadas de la habitaci�n actual
				x = areas[i]->at(j)->getPosX();
				y = areas[i]->at(j)->getPosY();
				// Probamos las cuatro direcciones
				bool checked_dirs = false;
				bool dirs[4];
				for (int k = 0; k < 4; k++) dirs[k] = false;
				while (!checked_dirs && n_screens < screens_per_area) {
					// Elegimos una direcci�n al azar si quedan habitaciones por a�adir a la zona
					int d = rand() % 4;
					switch (d) {
						case UP: 
							if (add_room_to_area(x, y-1, i)) n_screens++; 
							break;
						case DOWN: 
							if (add_room_to_area(x, y+1, i)) n_screens++; 
							break;
						case LEFT: 
							if (add_room_to_area(x-1, y, i)) n_screens++; 
							break;
						case RIGHT: 
							if (add_room_to_area(x+1, y, i)) n_screens++; 
							break;
					}
					dirs[d] = true;

					checked_dirs = true;
					for (int k = 0; k < 4; k++) checked_dirs = checked_dirs && dirs[k];
				}
				// Si hemos llegado al final del vector de habitaciones de la zona, es que no es posible a�adir m�s habitaciones
				no_mov = visited_rooms == areas[i]->size();
			}
		}
	}

	// A�adimos las habitaciones que se han quedado aisladas
	add_isolated_rooms();

	// Fijamos el �rea del jefe
	if (bossY == 0) boss_area = room_areas[bossX][1];
	else boss_area = room_areas[bossX][height-2];

	// Colocamos el bloqueo del jefe
	if (bossY == 0) layout[bossX][1]->setBoss_lock(UP);
	else layout[bossX][height-2]->setBoss_lock(DOWN);
}

void DungeonM::allocate_keys() {
    // Colocamos las llaves normales y las del jefe
    // Una llave por �rea, para asegurarnos de que puede visitar todas las zonas
	DunScreen* s;
    for (int i = 0; i < n_areas; i++) {
		// Nos aseguramos de que no caiga una llave en la habitaci�n del jefe
		do { s = areas[i]->at(rand() % areas[i]->size()); }
		while (s->getBoss() >= 0);
        s->setKey();
	}
    // La llave del jefe debe estar en una zona distinta a la del jefe
    int a;
    do { a = rand() % n_areas; }
    while (a == boss_area);
    areas[a]->at(rand() % areas[a]->size())->setBoss_key();
}

void DungeonM::allocate_boss() {
	// Elijo una de las cuatro esquinas de la mazmorra
	boss_corner = rand() % 4;
	switch (boss_corner) {
		case 0: 
			bossX = 0; bossY = 0;
			finalX = 1; finalY = 0;
			break;
		case 1: 
			bossX = width - 1; bossY = 0; 
			finalX = width -2; finalY = 0;
			break;
		case 2: 
			bossX = 0; bossY = height; 
			finalX = 1; finalY = height;
			break;
		case 3: 
			bossX = width - 1; bossY = height; 
			finalX = width - 2; finalY = height;
			break;
	}
	height++;	// A�ado una fila al layout para meter al jefe y la pantalla posterior
	// Coloco el boss en dicha esquina
	boss_screen = new DunScreen(bossX, bossY, -1, -1, boss, -1, -1, zone, theme, db, numDungeon);
	final_screen = new DunScreen(finalX, finalY, -1, -1, -1, -1, -1, zone, theme, db, numDungeon);
	// Coloco los bloqueos del jefe apropiados
	if (bossY == 0) boss_screen->setBoss_lock(DOWN);
	else boss_screen->setBoss_lock(UP);
	// Coloco una puerta de la habitaci�n del jefe a la habitaci�n final
	if (bossX == 0) {
		boss_screen->setDoor(RIGHT);
		final_screen->setDoor(LEFT);
	}
	else {
		boss_screen->setDoor(LEFT);
		final_screen->setDoor(RIGHT);
	}
}

void DungeonM::allocate_goodies() {
    // Usamos un vector de booleanos que nos diga qu� zonas de la mazmorra ya tienen algo importante
    bool* used_area = new bool[n_areas];
    // Flags necesarios
    bool boss_set = false;  // El jefe aun no ha sido colocado
    bool tool_set = false;  // La herramienta a�n no ha sido colocada
    bool start_set = false; // La entrada a�n no ha sido colocada

    // Nos aseguramos de que todas est�n libres al principio
    for (int i = 0; i < n_areas; i++)
        used_area[i] = false;
	// Fijamos la zona del jefe como usada
	used_area[boss_area] = true;

    while (!tool_set || !start_set) {
        // Elegimos una zona al azar para guardar lo que sea importante, comprobando que no est� ocupada
        int a = rand() % n_areas;
        while (used_area[a]) a = rand() % n_areas;
        int room = rand() % areas[a]->size();   // Elegimos una habitaci�n de esa zona

		if (!tool_set) {
            areas[a]->at(room)->setTool(tool);  // Colocamos la herramienta de la mazmorra
            tool_set = true;
            used_area[a] = true;                // Zona ocupada
        } else if (!start_set) {
            // Indicamos la habitaci�n inicial de la mazmorra
            iniX = areas[a]->at(room)->getPosX();
            iniY = areas[a]->at(room)->getPosY();
            used_area[a] = true;                // Zona ocupada
            start_set = true;
        }
    }
}

DunScreen* DungeonM::make_door(DunScreen* s, int* connected, int* current_room, int dir_or, short area) {
    bool b;                                  // Es la habitaci�n por la que vamos la que buscamos?
    DunScreen* next_room = NULL;                // Siguiente habitaci�n a visitar
    int dir_dest;                            // Direcci�n de la puerta de la habitaci�n a visitar
    vector<DunScreen*>* aux = areas[area];      // Simplemente para acortar un poco el c�digo, puntero al �rea en cuesti�n
    int n_rooms = areas[area]->size();       // Cantidad de habitaciones que hay en esta zona

	int j = 0;
	bool found = false;
	while (j < n_rooms && !found) {
        b = false;
        switch (dir_or) {                    // Seg�n la direcci�n de la puerta de la habitaci�n original...
            case UP:
				b = s->getPosX() == aux->at(j)->getPosX() && s->getPosY() == aux->at(j)->getPosY() + 1;
                dir_dest = DOWN;
                break;
            case DOWN:
                b = s->getPosX() == aux->at(j)->getPosX() && s->getPosY() == aux->at(j)->getPosY() - 1;
                dir_dest = UP;
                break;
            case LEFT:
                b = s->getPosY() == aux->at(j)->getPosY() && s->getPosX() == aux->at(j)->getPosX() + 1;
                dir_dest = RIGHT;
                break;
            case RIGHT:
                b = s->getPosY() == aux->at(j)->getPosY() && s->getPosX() == aux->at(j)->getPosX() - 1;
                dir_dest = LEFT;
                break;
        }

        if (b) { // La habitaci�n por la que vamos es la que buscamos
            s->setDoor(dir_or);                 // A�adimos una puerta a la habitaci�n de origen
            aux->at(j)->setDoor(dir_dest);      // A�adimos una puerta en la otra habitaci�n en direcci�n contraria
            connected[*current_room] = j;       // Conectamos la habitaci�n actual (el �ndice de esta) con la nueva habitaci�n
            next_room = aux->at(j);             // Fijamos como habitaci�n actual la �ltima habitaci�n visitada
            *current_room = j;                  // Hacemos lo mismo con el �ndice de la habitaci�n actual
            found = true;                       // Hemos encontrado la habitaci�n y hemos terminado el trabajo, salimos del bucle
        }
		j++;
    }

    return next_room;

}

void DungeonM::connect_rooms(short area) {
    int n_rooms = areas[area]->size();  // Cantidad de habitaciones que hay en esta zona
    int* connected = new int[n_rooms];  // Vector de conexiones entre habitaciones (muy Dijkstra)
    for (int i = 0; i < n_rooms; i++)
        connected[i] = -1;              // Inicializamos las conexiones a -1 (ninguna habitaci�n conectada)

    bool finished = false;              // �Ha terminado? No.
    DunScreen* s = areas[area]->at(0);     // Habitaci�n actual (inicialmente, la primera del vector)

    int current_room = 0;               // �ndice de la habitaci�n actual dentro del vector de zona

	if (n_rooms == 1) finished = true;	// Si la zona est� formada por una �nica habitaci�n, evidentemente hemos terminado

    while (!finished) {                 // Mientras no haya terminado...
        DunScreen* s1 = make_door(s, connected, &current_room, rand() % 4, area); // Elegimos una direcci�n al azar e intentamos hacer una puerta ah�
        if (s1 != NULL)
            s = s1; // Si hemos encontrado otra habitaci�n de la misma zona en esa habitaci�n, nos movemos a ella

        bool b = true;                      // Queremos comprobar si todas las habitaciones de la zona han sido visitadas
        for (int i = 0; i < n_rooms; i++)   // Recorremos el vector de habitaciones conectadas
			// Si alguna no ha sido conectada, no hemos terminado todav�a
			b = b && connected[i] != -1;   
        finished = b;                       // Si todas han sido visitadas, hemos terminado
    }
    delete connected; connected = NULL;     // Liberamos la memoria que hab�amos solicitado

}

void DungeonM::connect_areas() {
	int* prev = new int[n_areas];
	for (int i = 0; i < n_areas; i++) prev[i] = -1;

	for (int i = 0; i < n_areas; i++) {
		if (prev[i] == -1) { // Todav�a no ha sido conectado
			bool connected = false;
			while (!connected) {
				// Cogemos una habitaci�n de la zona al azar, pero que no tenga boss
				DunScreen* s1; 
				do { s1 = areas[i]->at(rand() % areas[i]->size()); }
				while (s1->getBoss() >= 0);
				DunScreen* s2 = NULL;
				int x = s1->getPosX();
				int y = s1->getPosY();

				// Miramos las habitaciones adyacentes a ver si podemos conectarlas
				int d = 0;
				while (d < 4 && !connected) {
					switch (d) {
						case UP:
							if (y-1 >= 0 && room_areas[x][y-1] != i && room_areas[x][y-1] != prev[i] && room_areas[x][y-1] != -2) {
								prev[i] = room_areas[x][y-1];
								connected = true;
								s2 = layout[x][y-1];
								s1->setLock(UP);
								s2->setLock(DOWN);
							}
							break;
						case DOWN:
							if (y+1 < height && room_areas[x][y+1] != i && room_areas[x][y+1] != prev[i] && room_areas[x][y+1] != -2) {
								prev[i] = room_areas[x][y+1];
								connected = true;
								s2 = layout[x][y+1];
								s1->setLock(DOWN);
								s2->setLock(UP);
							}
							break;
						case LEFT:
							if (x-1 >= 0 && room_areas[x-1][y] != i && room_areas[x-1][y] != prev[i] && room_areas[x-1][y] != -2) {
								prev[i] = room_areas[x-1][y];
								connected = true;
								s2 = layout[x-1][y];
								s1->setLock(LEFT);
								s2->setLock(RIGHT);
							}
							break;
						case RIGHT:
							if (x+1 < width && room_areas[x+1][y] != i && room_areas[x+1][y] != prev[i] && room_areas[x+1][y] != -2) {
								prev[i] = room_areas[x+1][y];
								connected = true;
								s2 = layout[x+1][y];
								s1->setLock(RIGHT);
								s2->setLock(LEFT);
							}
							break;
					}
					d++;
				}
			}
		}
	}

	delete prev; prev = NULL;
}

void DungeonM::generate() {
    calculate_size();
	allocate_boss();
    create_rooms();
    divide_into_areas();
    allocate_goodies();
	allocate_keys();

    // Conecta las habitaciones de cada �rea entre s�
    for (int i = 0; i < n_areas; i++)
        connect_rooms(i);
    connect_areas();

    // Genera y almacena las habitaciones en el vector de pantallas
	for (int i = 0; i < width; i++)
	    for (int j = 0; j < height; j++) {
			if (layout[i][j] != NULL) {
				layout[i][j]->generate();
				screenList->push_back(layout[i][j]);
			}
        }
}

void DungeonM::print_dungeon() {
	
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
			layout[i][j]->print_screen();
		}
		printf("\n");
	}
}
