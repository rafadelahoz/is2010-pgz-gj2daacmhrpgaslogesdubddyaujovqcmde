#pragma once

#ifndef _DEFTYPES_H_
#define _DEFTYPES_H_

#include <string>

using namespace std;

struct player_t {
	short id;
	string name;
	short gfxId;
	short hp;		// Puntos de vida
	short mp;		// Puntos de magia
	short atk;		// Ataque
	short def;		// Defensa

	bool operator<(const player_t &p) const { return id < p.id; }
};

struct essential_elem_t {
	short id;
	short type;
	string path;
};

struct enemy_t {
	short id;
	short gfxId;
	short hp;
	short atk;
	short df;
	string name;

	bool operator<(const enemy_t &e) const { return id < e.id; }
};

struct npc_t {
	short id;
	short gfxId;
	short sfxId;
	short movComp;
	vector<short>* texts;

	string name;
	bool operator<(const npc_t &npc) const { return id < npc.id; }
};

struct tool_t {
	short id;
	short gfxId;
	short dmgType;	// Tipo de da�o (bloqueos que abre)
	short gfxAmmo;	// Gr�fico de la munici�n del arma (id, -1 uno si no usa munici�n)
	short maxAmmo;	// Munici�n m�xima
	short strength;	// Fuerza del arma/herramienta
	string name;
	short type;		// Tipo de herramienta (melee o lo otro)

	bool operator<(const tool_t &t) const { return id < t.id; }
};

// �tems y PowUps tienen los mismos campos (de momento)
struct item_t {
	short id;
	short power;
	short effect;
	short gfxId;
	string name;
	bool operator<(const item_t &pu) const { return id < pu.id;}
};

// KeyItems y pigeons
struct obj_t {
	short id;
	short gfxId;
	string name;
	bool operator<(const obj_t &i) const { return id < i.id; }
};

struct boss_t {
	short id;
	string name;

	bool operator<(const boss_t &b) const { return id < b.id; }
};

struct block_t {
	short id;
	short type;
	short gfxId;
	short dmgType;

	bool operator<(const block_t &b) const { return id < b.id;}
};

struct gfx_t {
	short id;
	string path;

	bool operator<(const gfx_t &g) const { return id < g.id;}
};

struct sfx_t {
	short id;
	string path;

	bool operator<(const sfx_t &s) const { return id < s.id;}
};

struct zone_t {
	short id;
	short idTileSet;
	string name;
	string gen;

	bool operator<(const zone_t &z) const { return id < z.id;}
};

struct dungeon_t {
	short id;
	short idTileSet;

	bool operator<(const dungeon_t &d) const { return id < d.id;}
};

#endif