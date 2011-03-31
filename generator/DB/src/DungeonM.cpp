#include "DungeonM.h"

DungeonM::DungeonM(string zone, string theme, short gameDiff, short dungNumber, short ratio, short tool, DBManager* db) :
    Dungeon(zone, theme, gameDiff, dungNumber, ratio, tool, db) {
    // Usamos la interfaz con la base de datos
    // Dejo este m�todo sin implementar a la espera de hacer la interfaz con la base de datos
    boss = 0;
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
        delete areas[i];    // La destructora de vector ya llama a la destructora de Screen
        areas[i] = NULL;
    }
    delete areas; areas = NULL;
}

void DungeonM::calculate_size() {
    // Tal cual est� pensado (y hecho) el algoritmo, requiere al menos 3 zonas (para separar las entidades principales)
    // Y como debe ser tan ancho y alto como �reas tenga, la mazmorra m�s peque�a ser� de 3x3 con 3 zonas (9 casillas)
    // De primeras no s� c�mo calibrar la dificultad de las mazmorras, esto habr� que verlo en acci�n, as� que dejo este m�todo por implementar
    width = 4;
    height = 5;
    n_areas = 4;
}

void DungeonM::create_rooms() {
    // Utiliza una matriz de punteros a Screen para mayor comodidad
    layout = new Screen**[width];
    for (int i = 0; i < width; i++) {
        layout[i] = new Screen*[height];
        for (int j = 0; j < height; j++) {
            // Creamos las pantallas vac�as, en la posici�n que se indica, con la zona que se indica
            // El n�mero de enemigos queda pendiente de calibrar la dificultad
            Screen* s = new Screen(i, j, -1, -1, -1, -1, -1, zone, db);
            layout[i][j] = s;
        }

    }
}

void DungeonM::divide_into_areas() {
    // DEBUG
    /*
    int** aux = new int*[width];
    for (int i = 0; i < width; i++) {
        aux[i] = new int[height];
        for (int j = 0; j < height; j++)
            aux[i][j] = 0;
    }*/

    // Creamos las �reas
    areas = new vector<Screen*>*[n_areas];
    for (int i = 0; i < n_areas; i++)
        areas[i] = new vector<Screen*>();

    // Algoritmo tentativo
    int x = 0, y = 0;           // Punto en el que se comienza a generar la siguiente �rea
    int l = width / n_areas;    // Anchura (o altura) de la nueva zona de la mazmorra
    for (int i = 0; i < n_areas; i++) {
        if (i == n_areas - 1) {
            // Si es la �ltima zona, llega hasta el final
            for (int j = x; j < width; j++)
                for (int k = y; k < height; k++) {
                    areas[i]->push_back(layout[j][k]);  // Guarda la habitaci�n en cuesti�n en el �rea por la que va
                    //aux[j][k] = i;   // DEBUG
                }
        }
        else
            // De no ser la �ltima zona, crea una nueva zona con un tama�o proporcional al resto a partir del espacio disponible
            if (i % 2 == 0) {   // Alterna un sentido y otro de la divisi�n de zonas
                // Rect�ngulo en vertical
                for (int j = x; j < x + l; j++)
                    for (int k = y; k < height; k++) {
                        areas[i]->push_back(layout[j][k]);
                        //aux[j][k] = i;  // DEBUG
                    }
                x += l; // Actualiza el valor de x
                l = (height - y) / (n_areas - i - 1);   // Fija el alto de la nueva zona
            }
            else {
                // Rect�ngulo en horizontal
                for (int j = x; j < width; j++)
                    for (int k = y; k < y + l; k++) {
                        areas[i]->push_back(layout[j][k]);
                        //aux[j][k] = i; // DEBUG
                    }
                y += l; // Actualiza el valor de y
                l = (width - x) / (n_areas - i - 1);    // Fija el ancho de la nueva zona
            }
    }

    // DEBUG
    /*
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++)

            printf("%d ", aux[i][j]);
        printf("\n");
    }
    */
}

void DungeonM::allocate_keys(short boss_area) {
    // Colocamos las llaves normales y las del jefe
    // Una llave por �rea, para asegurarnos de que puede visitar todas las zonas
    for (int i = 0; i < n_areas; i++)
        areas[i]->at(rand() % areas[i]->size())->setKey();
    // La llave del jefe debe estar en una zona distinta a la del jefe
    int a;
    do { a = rand() % n_areas; }
    while (a == boss_area);
    areas[a]->at(rand() % areas[a]->size())->setBoss_key();
}

void DungeonM::allocate_goodies() {
    // Usamos un vector de booleanos que nos diga qu� zonas de la mazmorra ya tienen algo importante
    bool* used_area = new bool[n_areas];
    // Flags necesarios
    bool boss_set = false;  // El jefe aun no ha sido colocado
    bool tool_set = false;  // La herramienta a�n no ha sido colocada
    bool start_set = false; // La entrada a�n no ha sido colocada
    int boss_area = -1;

    // Nos aseguramos de que todas est�n libres al principio
    for (int i = 0; i < n_areas; i++)
        used_area[i] = false;

    while (!boss_set || !tool_set || !start_set) {
        // Elegimos una zona al azar para guardar lo que sea importante, comprobando que no est� ocupada
        int a = rand() % n_areas;
        while (used_area[a]) a = rand() % n_areas;
        int room = rand() % areas[a]->size();   // Elegimos una habitaci�n de esa zona

        if (!boss_set) {
            areas[a]->at(room)->setBoss(boss);  // Colocamos al boss
            // Colocar al boss implica m�s cosas, to be done
            boss_set = true;                    // Boss colocado
            used_area[a] = true;                // Zona ocupada
            boss_area = a;                      // Esta es la zona del boss
            a = rand() % n_areas;               // Elegimos otra �rea
            continue;                           // Volvemos al principio
        }
        if (!tool_set) {
            areas[a]->at(room)->setTool(tool);  // Colocamos la herramienta de la mazmorra
            tool_set = true;
            used_area[a] = true;                // Zona ocupada
            a = rand() % n_areas;               // Elegimos otra �rea
            continue;                           // Volvemos al principio
        }
        if (!start_set) {
            // Indicamos la habitaci�n inicial de la mazmorra
            iniX = areas[a]->at(room)->getPosX();
            iniY = areas[a]->at(room)->getPosY();
            used_area[a] = true;                // Zona ocupada
            start_set = true;
            continue;
        }
    }

    allocate_keys(boss_area);
}

Screen* DungeonM::make_door(Screen* s, int* connected, int* current_room, int dir_or, short area) {
    bool b;                                  // Es la habitaci�n por la que vamos la que buscamos?
    Screen* next_room = NULL;                // Siguiente habitaci�n a visitar
    int dir_dest;                            // Direcci�n de la puerta de la habitaci�n a visitar
    vector<Screen*>* aux = areas[area];      // Simplemente para acortar un poco el c�digo, puntero al �rea en cuesti�n
    int n_rooms = areas[area]->size();       // Cantidad de habitaciones que hay en esta zona

    for (int j = 0; j < n_rooms; j++) {      // Recorremos las habitaciones de la zona en busca de la que nos hace falta
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

        if (b) {    // La habitaci�n por la que vamos es la que buscamos
            s->setDoor(dir_or);                 // A�adimos una puerta a la habitaci�n de origen
            aux->at(j)->setDoor(dir_dest);      // A�adimos una puerta en la otra habitaci�n en direcci�n contraria
            connected[*current_room] = j;       // Conectamos la habitaci�n actual (el �ndice de esta) con la nueva habitaci�n
            next_room = aux->at(j);             // Fijamos como habitaci�n actual la �ltima habitaci�n visitada
            *current_room = j;                  // Hacemos lo mismo con el �ndice de la habitaci�n actual
            break;                              // Hemos encontrado la habitaci�n y hemos terminado el trabajo, salimos del bucle
        }
    }

    return next_room;

}

void DungeonM::connect_rooms(short area) {
    int n_rooms = areas[area]->size();  // Cantidad de habitaciones que hay en esta zona
    int* connected = new int[n_rooms];  // Vector de conexiones entre habitaciones (muy Dijkstra)
    for (int i = 0; i < n_rooms; i++)
        connected[i] = -1;              // Inicializamos las conexiones a -1 (ninguna habitaci�n conectada)

    bool finished = false;              // �Ha terminado? No.
    Screen* s = areas[area]->at(0);     // Habitaci�n actual (inicialmente, la primera del vector)

    int current_room = 0;               // �ndice de la habitaci�n actual dentro del vector de zona

    while (!finished) {                 // Mientras no haya terminado...
        Screen* s1 = make_door(s, connected, &current_room, rand() % 4, area); // Elegimos una direcci�n al azar e intentamos hacer una puerta ah�
        if (s1 != NULL)
            s = s1; // Si hemos encontrado otra habitaci�n de la misma zona en esa habitaci�n, nos movemos a ella

        bool b = true;                      // Queremos comprobar si todas las habitaciones de la zona han sido visitadas
        for (int i = 0; i < n_rooms; i++)   // Recorremos el vector de habitaciones conectadas
            b = b && connected[i] != -1;    // Si alguna no ha sido conectada, no hemos terminado todav�a
        finished = b;                       // Si todas han sido visitadas, hemos terminado
    }
    delete connected; connected = NULL;     // Liberamos la memoria que hab�amos solicitado

}

Screen* DungeonM::make_lock(Screen* s1, short area) {
    // Dada una pantalla de una zona, tenemos que ver si alguna de las (m�ximo) 4 habitaciones adyacentes pertenece a la siguiente �rea
    // Vemos las coordenadas de la habitaci�n en la que nos encontramos
    int x = s1->getPosX();
    int y = s1->getPosY();
    // Miramos las habitaciones de la siguiente zona
    for (unsigned int i = 0; i < areas[area+1]->size(); i++) {
        Screen* s2 = areas[area+1]->at(i);
        // Comprobamos si es una de las 4 habitaciones adyacentes
        // Arriba
        if (y == s2->getPosY() + 1 && x == s2->getPosX()) {
            // Nos vale la habitaci�n!
            s1->setLock(UP);
            s2->setLock(DOWN);
            return s2;
        }
        // Abajo
        if (y == s2->getPosY() - 1 && x == s2->getPosX()) {
            // Nos vale la habitaci�n!
            s1->setLock(DOWN);
            s2->setLock(UP);
            return s2;
        }
        // Izquierda
        if (x == s2->getPosX() + 1 && y == s2->getPosY()) {
            // Nos vale la habitaci�n!
            s1->setLock(LEFT);
            s2->setLock(RIGHT);
            return s2;
        }
        // Derecha
        if (x == s2->getPosX() - 1 && y == s2->getPosY()) {
            // Nos vale la habitaci�n!
            s1->setLock(RIGHT);
            s2->setLock(LEFT);
            return s2;
        }
    }
    return NULL;    // No nos val�a ninguna de las 4 habitaciones adyacentes
}

void DungeonM::connect_areas() {
    // Tendremos que colocar tantos bloqueos como zonas menos una haya en la mazmorra
    for (int i = 0; i < n_areas - 1; i++) {
        bool set = false;               // Todav�a no hemos colocado el bloqueo
        while (!set) {                  // Si no hemos conseguido colocar el bloqueo, volvemos a intentarlo
            Screen* s1 = areas[i]->at(rand() % areas[i]->size());    // Elegimos una pantalla de la zona al azar
            Screen* s2 = make_lock(s1, i);
            if (s2 != NULL) set = true; // Si hemos llegado a otra habitaci�n, es que hemos conseguido colocar el cerrojo
        }
    }
}

void DungeonM::generate() {
    calculate_size();
    create_rooms();
    divide_into_areas();
    allocate_goodies();

    // Conecta las habitaciones de cada �rea entre s�
    for (int i = 0; i < n_areas; i++)
        connect_rooms(i);
    connect_areas();

    // Genera y almacena las habitaciones en el vector de pantallas
    for (int j = 0; j < height; j++)
        for (int i = 0; i < width; i++) {
            layout[i][j]->generate();
            screenList->push_back(layout[i][j]);
        }

    // MOAR DEBUG
    /*
    vector<Screen*>::iterator it;
    for (it = screenList->begin(); it < screenList->end(); it++)
        printf("%d %d | %d %d %d %d | %d %d %d %d\n", (*it)->getPosX(), (*it)->getPosY(),
               (*it)->getDoor(UP), (*it)->getDoor(DOWN), (*it)->getDoor(LEFT), (*it)->getDoor(RIGHT),
               (*it)->getLock(UP), (*it)->getLock(DOWN), (*it)->getLock(LEFT), (*it)->getLock(RIGHT));

    printf("Start: (%d,%d)\n", iniX, iniY);
    */

}

void DungeonM::print_dungeon() {
	
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
			layout[i][j]->print_screen();
		}
		printf("\n");
	}
}
