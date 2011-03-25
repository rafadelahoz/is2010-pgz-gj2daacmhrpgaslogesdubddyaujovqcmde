#include "GamePlayState.h"

GamePlayState::GamePlayState(int w, int h, Game* g) : GameState(g, w, h)
{
	// Se instancia el Controlador, que instancia todo
	// Quiz�s yo tengo que guardar punteros?

	controller = new Controller("", g, this);
	// Se a�ade a la lista de auto-manejo
	_add(controller);
};

GamePlayState::~GamePlayState()
{
	// eliminar datos que tenga, controller & such
	//delete controller;
	// limpiar deathRow y borrarlo
};

// Sobrecarga del m�todo del padre para utilizar la lista de eliminables
bool GamePlayState::add(Entity* e, bool condemned)
{
	// pensar
};

// Sobrecarga del m�todo del padre para utilizar la lista de eliminables
bool GamePlayState::removeEntity(Entity* e)
{
	// (Work in Progress)

	// En principio esto se llama ya sabiendo si la entidad pertenece 
	// o no al deathRow, activando o no el flag deleteAlso en consecuencia.
	// Pasos: (Suponiendo un GameState::remove(entity) que no la borre siempre y permita par�metro de borrado o no)
	// 1. return GameState::remove(e, deleteAlso)

	// [  Nota! Esto podr�a checkear si e pertenece al deathRow  ]
	
	// Por ahora se comprueba si ha de borrarse y se avisa al GameState para que lo haga
	// [  Descomentar cuando GameState pueda borrar sin eliminar  ] 
	return GameState::remove(e /*, isCondemned(e) */);
};

void GamePlayState::renderBG()
{
	// Se dibuja un rect�ngulo para pruebas de gr�ficos de forma temporal
	game->getGfxEngine()->renderRectangle(1, 1, roomw-2, roomh-2, Color::Black, true);
};