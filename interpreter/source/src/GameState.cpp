#include "GameState.h"

bool entity_compare(Entity* a, Entity* b);


void GameState::init()
{
    // Inicializaci�n de las listas
    entities = new set<Entity*>();
    enabled = new set<Entity*>();
    collidable = new set<Entity*>();
    // Usaremos la funci�n entity_compare para la comparaci�ne entre entidades
    renderable = new set<Entity*, bool(*)(Entity*, Entity*)>(entity_compare);

    // Inicializaci�n de los buffers
    addedEntitiesBuffer = new set<Entity*>();
    deletedEntitiesBuffer = new set<Entity*>();
    enabledBuffer = new set<Entity*>();
    collidableBuffer = new set<Entity*>();
    renderableBuffer = new set<Entity*>();
}

GameState::GameState(Game* g, int roomw, int roomh)
{
    // Inicializaci�n de estructuras
    init();

    // Almacenamos los par�metros necesarios
    map = NULL;
    this->roomw = roomw;
    this->roomh = roomh;
}

GameState::GameState(Game* g, Map* m, int roomw, int roomh)
{
    // Inicializaci�n de estructuras
    init();

    // Almacenamos los par�metros necesarios
    map = m;
    this->roomw = roomw;
    this->roomh = roomh;
}

GameState::~GameState()
{
    // Borrado de buffers de entidades.
    // Las entidades que contienen ya est�n dentro del GameState porque ser�n
    // borradas directamente de entities.
    delete renderableBuffer;
    delete collidableBuffer;
    delete enabledBuffer;
    delete deletedEntitiesBuffer;

    // El buffer de entidades a a�adir contiene entidades
    // que no estaban en el GameState, por lo que deben ser borradas expl�citamente.
    set<Entity*>::iterator it;
    for(it = deletedEntitiesBuffer->begin(); it != deletedEntitiesBuffer->end(); it++)
        if ((*it) != NULL)
        {
            delete (*it);
        }
    delete addedEntitiesBuffer;

    // Borrado de las listas de entidades
    delete renderable;
    delete collidable;
    delete enabled;
    // Borramos todas las entidades del sistema
    for(it = entities->begin(); it != entities->end(); it++)
        if ((*it) != NULL)
        {
            if (!(*it)->persistent)
            delete (*it);
        }
    delete entities;

    // Borramos el mapa
    if (map != NULL)
        delete map;
}

void GameState::onInit()
{
    // Llama al evento correspondiente de las entidades
    set<Entity*>::iterator it;
    for(it = entities->begin(); it != entities->end(); it++)
        if ((*it) != NULL)
        {
            (*it)->onInitWorld();
        }
}
void GameState::_update()
{
    // Iteradores y estructuras que se usar�n
    set<Entity*>::iterator i;
    set<Entity*>::iterator j;

    vector<CollisionPair>::iterator k;
    vector<CollisionPair>* collision_list;

    // ---------------------------------
    // ---------- ON INIT --------------
    // ---------------------------------

    // Realizamos las acciones iniciales de cada entidad
    for(i = enabled->begin(); i != enabled->end(); i++)
        if ((*i) != NULL)
        {
             (*i)->onInitStep();
        }

    // ---------------------------------
    // --------- ON COLLISION ----------
    // ---------------------------------

    // Comprobaci�n de colisiones con el mapa, si existe
    if (map != NULL)
        for(i = collidable->begin(); i != collidable->end()--; i++)
            if ((*i) != NULL)
            {
                collision_list = map->getSolidGrid()->collide((*i)->mask);
                for(k = collision_list->begin(); k != collision_list->end(); k++)
                {
                    (*i)->onCollision((*k));
                }
            }

    // Comprobaci�n de colisiones entre entidades

    // Cada una de las entidades, desde la primera a la pen�ltima,
    for(i = collidable->begin(); i != collidable->end()--; i++)
        // con cada de las entidades restantes, desde la segunda a la �ltima
        for(j = i, j++; j != collidable->end(); j++)
        {
            // si ninguna de las entidades es inv�lida, comprobamos la colisi�n
            if (((*i) != NULL) && ((*j) != NULL))
            {
                // Obtenemos una lista de colisi�n y la iteramos
                collision_list = (*i)->mask->collide((*j)->mask);
                for(k = collision_list->begin(); k != collision_list->end(); k++)
                {
                    (*i)->onCollision((*k));
                    (*j)->onCollision((*k));
                }
                // debemos ocuparnos de eliminar la lista una vez usada
                delete collision_list;
            }
        }


    // ---------------------------------
    // ------------ ON STEP ------------
    // ---------------------------------

    // Actualizaci�n principal de las entidades
    for(i = enabled->begin(); i != enabled->end(); i++)
        if ((*i) != NULL)
        {
            (*i)->onStep();
        }

    // Actualizaci�n del gameState, si se desea
    onStep();


    // ---------------------------------
    // --------- ON END STEP -----------
    // ---------------------------------

    // Realizamos las acciones finales de cada entidad
    for(i = enabled->begin(); i != enabled->end(); i++)
    if ((*i) != NULL)
    {
        (*i)->onEndStep();
    }


    // ---------------------------------
    // -- Actualizaci�n de los buffers -
    // ---------------------------------

    // Buffer de entidades a a�adir
    for(i = addedEntitiesBuffer->begin(); i != addedEntitiesBuffer->end(); i++)
    if ((*i) != NULL)
    {
        _add((*i));
    }
    addedEntitiesBuffer->clear();

    // Buffer de entidades a eliminar
    for(i = deletedEntitiesBuffer->begin(); i != deletedEntitiesBuffer->end(); i++)
    if ((*i) != NULL)
    {
        _remove((*i));
    }
    deletedEntitiesBuffer->clear();

    // Buffer de entidades que han alterado su estado enabled.
    // Dependiendo de a qu� valor haya cambiado el estado, se a�adir�n o quitar�n de la correspondiente lista
    for(i = enabledBuffer->begin(); i != enabledBuffer->end(); i++)
    if ((*i) != NULL)
    {
        if ((*i)->enabled)
            enabled->insert((*i));
        else
            enabled->erase((*i));
    }
    enabledBuffer->clear();

    // Buffer de entidades que han alterado su estado collidable.
    // Dependiendo de a qu� valor haya cambiado el estado, se a�adir�n o quitar�n de la correspondiente lista
    for(i = collidableBuffer->begin(); i != collidableBuffer->end(); i++)
    if ((*i) != NULL)
    {
        if ((*i)->collidable)
            collidable->insert((*i));
        else
            collidable->erase((*i));
    }
    collidableBuffer->clear();

    // Buffer de entidades que han alterado su estado renderable.
    // Dependiendo de a qu� valor haya cambiado el estado, se a�adir�n o quitar�n de la correspondiente lista
    for(i = renderableBuffer->begin(); i != renderableBuffer->end(); i++)
    if ((*i) != NULL)
    {
        if ((*i)->visible)
            renderable->insert((*i));
        else
            renderable->erase((*i));
    }
    renderableBuffer->clear();
}

void GameState::_add(Entity* e)
{
    // Si la imagen no existe no hacemos nada
    if (e != NULL)
    {
        // a�adimos la entidad a la lista de entidades
        entities->insert(e);

        // dependiendo de sus atributos, la a�adimos a las dem�s listas o no.
        if (e->enabled)
            enabled->insert(e);
        if (e->collidable)
            collidable->insert(e);
        if (e->visible)
            renderable->insert(e);

        // se llama al evento de inicio de la entidad
        e->onInit();
    }
}

void GameState::onStep() {}

void GameState::renderBG() {}

void GameState::renderFG() {}

void GameState::_remove(Entity* e)
{
    // Si la imagen no existe no hacemos nada
    if (e != NULL)
    {
        // Eliminamos la entidad de las listas
        entities->erase(e);
        enabled->erase(e);
        collidable->erase(e);
        renderable->erase(e);

        // Borramos la entidad
        delete e;
    }
}

void GameState::onRender()
{
    // Si se desea, se pintar�n elementos por debajo de los dem�s
    renderBG();

    // Si hay mapa, se pinta
    if (map != NULL)
        map->render(0,0);

    set<Entity*>::iterator i;
    // Pintamos todas las entidades visibles
    for(i = renderable->begin(); i != renderable->end(); i++)
        if ((*i) != NULL)
        {
            (*i)->onRender();
        }

    // Si se desea, se pintar�n elementos por encima de los dem�s
    renderFG();
}

void GameState::addMap(Map* map)
{
    // si exist�a un mapa, lo borramos y asignamos el nuevo
    if (this->map != NULL)
        delete map;

    this->map = map;
}

void GameState::removeAll()
{
    // Borra todos los elementos de la lista
    set<Entity*>::iterator it;
    for(it = entities->begin(); it != entities->end(); it++)
        if ((*it) != NULL)
        {
            _remove(*it);
        }
}

bool GameState::add(Entity* e)
{
    // a�adimos la entidad al buffer para que se a�ada al final del tick
    if (e != NULL)
    {
        addedEntitiesBuffer->insert(e);
        return true;
    }
    else
        return false;
}

bool GameState::remove(Entity* e)
{
    // a�adimos la entidad al buffer para que se elimine al final del tick
    if (e != NULL)
    {
        addedEntitiesBuffer->erase(e);
        return true;
    }
    else
        return false;
}

void GameState::addList(vector<Entity*>* l)
{
    // a�adimos individualmente cada elemento de la lista
    vector<Entity*>::iterator it;
    for(it = l->begin(); it != l->end(); it++)
        if ((*it) != NULL)
        {
            add(*it);
        }
}

void GameState::removeList(vector<Entity*>* l)
{
    // eliminamos individualmente cada elemento de la lista
    vector<Entity*>::iterator it;
    for(it = l->begin(); it != l->end(); it++)
        if ((*it) != NULL)
        {
            remove(*it);
        }
}

void GameState::changedRenderable(Entity* e)
{
    // a�adimos la entidad al buffer para que se efect�e el cambio al final del tick
    if (e != NULL)
        renderableBuffer->insert(e);
}

void GameState::changedCollidable(Entity*e)
{
    // a�adimos la entidad al buffer para que se efect�e el cambio al final del tick
    if (e != NULL)
        collidableBuffer->insert(e);
}

void GameState::changedEnabled(Entity*e)
{
    // a�adimos la entidad al buffer para que se efect�e el cambio al final del tick
    if (e != NULL)
        enabledBuffer->insert(e);
}

vector<Entity*>* GameState::getType(std::string type)
{
    // Creamos la lista que contendr� el resultado
    vector<Entity*>* l = new vector<Entity*>();

    set<Entity*>::iterator i;
    // A�adiremos a la lista todas las entidades cuyo tipo coincida con el dado
    for(i = entities->begin(); i != entities->end(); i++)
        if ((*i) != NULL)
        {
            if ((*i)->type == type)
                l->push_back(*i);
        }
    return l;
}

bool GameState::place_free(int x, int y, Entity* e)
{
}

bool GameState::position_free(int x, int y){}
bool GameState::place_free_entities(int x, int y, Entity* e){}
bool GameState::position_free_entities(int x, int y){}
bool GameState::GameState::collides(Entity* a, Entity* b){}
Entity* GameState::place_meeting(int x, int y, Entity* e, std::string type){}
vector<Entity*>* GameState::enclosedEntities(Mask mask, std::string type){}
void GameState::moveToContact(int x, int y, Entity* e){}

bool entity_compare(Entity* a, Entity* b)
{
    return a->depth < b->depth;
};
