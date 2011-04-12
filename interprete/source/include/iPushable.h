#pragma once

#ifndef __I_PUSHABLE_H__
#define __I_PUSHABLE_H__

#include "HelperTypes.h"
#include <vector>

using namespace std;

class iPushable
{
	private:
		bool locked;
		bool useConstraints;
		vector<Direction> pushConstraints;
		
	public:
		// Distancia empujable en cada step
		int stepPushDist;
		
		// Por defecto ser� empujable
		iPushable();
		iPushable(int stepPushDist, bool useConstraints = false);
		~iPushable();

		void init(int stepPushDist, bool useConstraints = false);
		void lockPush();
		void unlockPush();
		bool isLockedPush();

		// Empuja y devuelve si ha sido empujado con �xito.
		// Param dir: direcci�n en la que nos empujan.
		// Param x, y: nuestra posici�n, actualizamos su valor
		bool onPush(Direction dir, int &x, int& y);

		// Indicamos en qu� direcciones puede empujarse (Activa los constraints)
		void setConstraints(vector<Direction> pushConstrains);
};

#endif
