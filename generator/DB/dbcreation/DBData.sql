-- Efectos: 0 = NONE, 1 = HP, 2 = MP, 3 = MONEY, 4 = KEY, 5 = HPMAX, 6 = MPMAX, 7 = BOSSKEY, 8 = AMMO
-- Tipos de da�o: 0 = SWORD
-- Path de los recursos: .\\BDD (aqu� s�lo se pone la ruta relativa, la parte com�n de las rutas en la BDJ y BDD)

-- Essential Elems
Insert into EssentialElem values (0, 0, '.\\Gfx\\cursor');			-- Cursor de men�
Insert into EssentialElem values (1, 1, '.\\Gfx\\game_menu');		-- Main menu
Insert into EssentialElem values (2, 2, '.\\Gfx\\shadow_s');		-- Sombra peque�a
Insert into EssentialElem values (3, 3, '.\\Gfx\\shadow_m');		-- Sombra mediana
Insert into EssentialElem values (4, 4, '.\\Gfx\\system');			-- Sistema de di�logos
Insert into EssentialElem values (5, 5, '.\\Gfx\\sprFont_strip94'); -- Font
Insert into EssentialElem values (6, 6, '.\\Gfx\\cursorM');			-- Cursor del men� de herramientas
Insert into EssentialElem values (7, 7, '.\\Gfx\\enemy-octorok'); 	-- TileSet del layout del minimapa (cambiar)
Insert into EssentialElem values (8, 8, '.\\Gfx\\StateMenuBackGround'); -- Bordes del men� de estado
Insert into EssentialElem values (9, 9, '.\\Gfx\\ToolMenuBackGround');
insert into EssentialElem values (10, 10, '.\\Gfx\\system-dummy');

-- Doors (id, type (0 normal, 1 boss), pathG)
Insert into Door values (0, 0, '.\\Gfx\\door');
Insert into Door values (1, 1, '.\\Gfx\\door');

-- DoorThemeTags
Insert into DoorThemeTags values (0, 'Zelda');
Insert into DoorThemeTags values (1, 'Zelda');

-- Gr�ficos de enemigos
Insert into Gfx values (0, '.\\Gfx\\enemy-octorok');
-- Gr�ficos de npcs
Insert into Gfx values (1, '.\\Gfx\\npc');
-- Gr�ficos de herramientas
Insert into Gfx values (2, '.\\Gfx\\weapon-slashsword');
Insert into Gfx values (3, '.\\Gfx\\weapon-sword');
Insert into Gfx values (4, '.\\Gfx\\weapon-bow');
Insert into Gfx values (5, '.\\Gfx\\weapon-staff');
Insert into Gfx values (6, '.\\Gfx\\weapon-boomerang');
-- Gr�ficos de items
Insert into Gfx values (7, '.\\Gfx\\key');
Insert into Gfx values (8, '.\\Gfx\\rupee');
Insert into Gfx values (9, '.\\Gfx\\heart');	
-- Gr�ficos de power ups
Insert into Gfx values (10, '.\\Gfx\\bigHeart');
-- Gr�ficos de blockades
Insert into Gfx values (11, '.\\Gfx\\grass');
-- Gr�ficos de ammo
Insert into Gfx values (12, '.\\Gfx\\arrow');
Insert into Gfx values (13, '.\\Gfx\\fireBall');
-- Gr�ficos de Player
insert into Gfx values (14, '.\\Gfx\\weird-sprsheet');

-- Players
insert into Player values (0, "RicksorMcDreiton", 14, 30, 10, 1245, 142);
insert into Player values (1, "Johan", 14, 0, 0, 0, 0);

-- Tilesets
Insert into TileSet values (0, '.\\Gfx\\forest-tset');
Insert into TileSet values (1, '.\\Gfx\\prairie-tset');
Insert into TileSet values (2, '.\\Gfx\\lake-tset');
Insert into TileSet values (3, '.\\Gfx\\desert-tset');
Insert into TileSet values (4, '.\\Gfx\\cave-dungeon');

-- Enemigos (id, name, gfxId, hp, str, df)
Insert into Enemy values (0, 'Octorok', 0, 10, 3, 2);

-- NPCs (id, name, gfxId, sfxId, movComp)
Insert into NPC values (0, 'Weird-bearded NPC', 1, -1, 0);

-- NPCText
Insert into NPCText values (0, 0);
Insert into NPCText values (0, 1);

-- Tools (id, gfxId, dmgType, type = {1, 2, 3}, gfxAmmo, maxAmmo, strength, name)
Insert into Tool values (0, 2, 0, 1, -1, -1, 3, 'Slash Sword'); -- Fuerza 3 because yes
Insert into Tool values (1, 3, 0, 1, -1, -1, 3, 'Thrust Sword');
Insert into Tool values (2, 4, 1, 2, 12, 20, 3, 'Bow');
Insert into Tool values (3, 5, 2, 2, 13, 10, 4, 'Magical Staff');
Insert into Tool values (4, 6, 3, 3, -1, -1, 2, 'Boomerang'); 

-- Blockades
Insert into Blockade values (0, 9, -1, 0);

-- Items
Insert into Item values (1, 'Heart', 1, 1, 6);
Insert into Item values (2, 'Rupee', 3, 1, 5); 

-- PowUps
Insert into PowUp values (0, 'Big Heart', 5, 1, 10);

-- EnemyThemeTags
Insert into EnemyThemeTags values (0, 'Zelda');

-- EnemyZoneTags
Insert into EnemyZoneTags values (0, 'Forest');
Insert into EnemyZoneTags values (0, 'Prairie');

-- NPCThemeTags
Insert into NPCThemeTags values (0, 'Zelda');

-- ToolThemeTags
Insert into ToolThemeTags values (0, 'Zelda');
Insert into ToolThemeTags values (1, 'Zelda');

-- BlockadeThemeTags
Insert into BlockadeThemeTags values (0, 'Zelda');

-- BlockadeZoneTags
Insert into BlockadeZoneTags values (0, 'Forest');
Insert into BlockadeZoneTags values (0, 'Prairie');

-- ItemThemeTags
Insert into ItemThemeTags values (0, 'Zelda');
Insert into ItemThemeTags values (1, 'Zelda');
Insert into ItemThemeTags values (2, 'Zelda');

-- PowUpThemeTags
Insert into PowUpThemeTags values (0, 'Zelda');

-- Zones
Insert into Zone values (0, 'Forest', 'Forest', 0);
Insert into Zone values (1, 'Prairie', 'Prairie', 1);
Insert into Zone values (2, 'Lake', 'Lake', 2);
Insert into Zone values (3, 'Desert', 'Desert', 3);

-- ZoneThemeTags
Insert into ZoneThemeTags values (0, 'Zelda');
Insert into ZoneThemeTags values (1, 'Zelda');
Insert into ZoneThemeTags values (2, 'Zelda');
Insert into ZoneThemeTags values (3, 'Zelda');

-- Dungeons
Insert into Dungeon values (0, 'DungeonJ', 4);
Insert into Dungeon values (1, 'DungeonM', 4);

-- DungeonThemeTags
Insert into DungeonThemeTags values (0, 'Zelda');
Insert into DungeonThemeTags values (1, 'Zelda');

-- DungeonZoneTags
Insert into DungeonZoneTags values (0, 'Forest');
Insert into DungeonZoneTags values (1, 'Prairie');