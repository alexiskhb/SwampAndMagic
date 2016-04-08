#include <map>
#include <ctime>
#include <string>
#include <cstdlib>
#include <iostream>
#include <curses.h>
#include <algorithm>
#include "bilateral_array.h"
#include "colored_text.h"
#include "base_objects.h"
#include "characters.h"
#include "control.h"
#include "objects.h"
#include "config.h"


typedef std::map<CharacterPtr, bool> CharacterBoolMap;
typedef std::pair<CharacterPtr, bool> CharacterBoolPair;


struct {
	std::list<CharacterPtr> characters;
	
	std::list<ObjectPtr> dyn_objects;
	
	std::list<BaseObjectPtr> relief;
	
	CharacterPtr knight;
	
	CharacterPtr princess;
	
	MapPtr map;
	
	std::string status;

	GMapSpecialList glob_map_special;

	Mode mode;

	bool is_over() {
		return characters.size() < 2 || knight->hitpoints() <= 0 || princess->hitpoints() <= 0 || is_win();
	}

	bool is_win() {
		return *knight % *princess;
	}

	void kill_died_characters_objects() {
		for(auto obj: dyn_objects) {
			if (map->is_far(knight, obj, 2)) {
				obj->destroy();
			}
			if (map->is_on_the_map(obj->get_coord())) {
				map->remove(obj);
			}
			if (obj->is_alive()) {
				*map << obj;
			}
		}
		for(auto ch: characters) {
			if (map->is_far(knight, ch, 2)) {
				ch->destroy();
			}
			if (ch->hitpoints() <= 0) {
				map->remove(ch);
			}
		}
		characters.remove_if([&](CharacterPtr ch) {
			return (ch->hitpoints() <= 0 && ch != knight) || !map->is_on_the_map(ch->get_coord());
		});
		dyn_objects.remove_if([&](ObjectPtr obj) {
			return !obj->is_alive() || !map->is_on_the_map(obj->get_coord());
		});
	}

	void next_turn() {
		++BaseObject::turn;
		map->move_the_frame(knight->get_coord() - knight->get_prev());
		CharacterBoolMap did_attack;
		for(auto ch_iter = std::next(characters.begin()); ch_iter != characters.end(); ch_iter++) {
			auto ch = *ch_iter;
			if (map->is_out_of_display(ch)) {
				continue;
			}
			did_attack.insert(CharacterBoolPair(ch, ch->attack(characters, dyn_objects, *map)));
		}
		// Monsters attack before the Knight does because they looking for the shortest way
		// in this procedure. It takes a long time which player spends thinking about next move
		did_attack.insert(CharacterBoolPair(knight, knight->attack(characters, dyn_objects, *map)));
		for(auto obj: dyn_objects) {
			if (map->is_out_of_display(obj)) {
				continue;
			}
			if (map->is_on_the_map(obj->get_coord())) {
				obj->impact(characters, dyn_objects, *map);
				map->remove(obj);
				*map << obj;
				obj->make_turn();
			}
			else {
				obj->move_to_prev();
				obj->destroy();
			}
		}
		for(auto ch: characters) {
			if (map->is_out_of_display(ch)) {
				continue;
			}
			if (!did_attack[ch]) {
				ch->move(*map, characters);
				if (!map->is_on_the_map(ch->get_coord())) {
					ch->move_to_prev();
					ch->suffer(ch->hitpoints());
				}
				else
				// changes will be accepted after calling kill_died_characters_objects()
				// so we can cancel try to move on wall here. prev_coords now match with actual.
				if (!map->is_penetrable(ch->get_coord())) {
					ch->move_to_prev();
				}
				else {
					// here we attach a character to cell
					// so other characters can not step on it
					map->remove(ch);
					*map << ch;
				}
			}
		}
		map->create_rooms(dyn_objects);
		kill_died_characters_objects();
	}

	void gprint(int row, int col, const char* text) {
		move(row, col);
		clrtobot();
		addstr(text);
	}

	inline void render() {
		int shift = 2;

		gprint(shift - 1, 0, " ");
		if (Config::instance().show_coordinates) {
			status = "(" + std::to_string(princess->getcol()) + ", " + std::to_string(-princess->getrow()) + ")";
			gprint(shift - 1, shift + MAP_WIDTH - status.size(), status.c_str());
		}

		map->display(shift);

		BaseObjectPtr obj = map->nearest_symb(
				knight->get_coord(), 
				std::string(ENEMIES), 
				std::min(MAP_HEIGHT, MAP_WIDTH)/2);

		status = "HP: " + std::to_string(characters.front()->hitpoints());
		gprint(MAP_HEIGHT+shift, MAP_WIDTH/2 - status.size()/2 - status.size()%2, status.c_str());

		if (Config::instance().show_coordinates) {
			status = "(" + std::to_string(knight->getcol()) + ", " + std::to_string(-knight->getrow()) + ")";
			gprint(MAP_HEIGHT+shift, shift + MAP_WIDTH - status.size(), status.c_str());
		}

		status = "NEAREST CHARACTER: " + std::to_string(obj->hitpoints());
		gprint(MAP_HEIGHT+shift+1, MAP_WIDTH/2 - status.size()/2 - status.size()%2, status.c_str());

		if (is_win()) {
	 		status = "And they lived happily ever after";
			gprint(MAP_HEIGHT+shift, MAP_WIDTH/2 - status.size()/2 - status.size()%2, status.c_str());
	 	}
	}

	void put_character(CharacterPtr ch) {
		characters.push_back(ch);
		*map << ch;
	}

	void put_dynobject(ObjectPtr obj) {
		dyn_objects.push_back(obj);
		*map << obj;	
	}

	void init_curses() {
		initscr();
		keypad(stdscr, true);
		noecho();
		start_color();
		curs_set(0);

		init_pair(ID_KNIGHT   , COLOR_WHITE , COLOR_GREEN  );
		init_pair(ID_PRINCESS , COLOR_WHITE , COLOR_CYAN   );
		init_pair(ID_DRAGON   , COLOR_WHITE , COLOR_RED    );
		init_pair(ID_ZOMBIE   , COLOR_WHITE , COLOR_YELLOW );
		init_pair(ID_WARLOCK  , COLOR_WHITE , COLOR_BLACK  );
		init_pair(ID_MAGIC    , COLOR_WHITE , COLOR_BLUE   );
		init_pair(ID_FLAME    , COLOR_WHITE , COLOR_RED    );
		init_pair(ID_SWAMP    , COLOR_GREEN , COLOR_WHITE  );
		init_pair(ID_CURSE    , COLOR_WHITE , COLOR_BLACK  );
		init_pair(ID_WALL     , COLOR_BLACK , COLOR_WHITE  );
		init_pair(ID_EMPTY    , COLOR_WHITE , COLOR_BLACK  );
		init_pair(ID_MEDKIT   , COLOR_WHITE , COLOR_RED    );
		init_pair(ID_DRGNEST  , COLOR_WHITE , COLOR_RED    );
		init_pair(ID_GRVYARD  , COLOR_WHITE , COLOR_GREEN  );
		init_pair(ID_ZIGGURAT , COLOR_WHITE , COLOR_BLACK  );
	}

	// Return true if initialization was succesful
	bool init() {
		try {
			Config::instance().load_configs();
		}
		catch (BadConfigException e) {
			std::cout << e.what() << std::endl;
		}
		catch (BadOptionException e) {
			std::cout << e.what() << std::endl;
			return false;
		}
		srand(time(0));
		map = std::make_shared<Map>(relief);
		map->create_room(0, 0, dyn_objects);

		knight = std::make_shared<Knight>(map->get_rand_free_cell(GCoord(0, 0)));
		characters.push_front(knight);
		*map << knight;

		int pr_row = rand()%300 - 150; pr_row += pr_row > 0 ? 300 : -300;
		int pr_col = rand()%300 - 150; pr_col += pr_col > 0 ? 300 : -300;
		GCoord pr_coord = GCoord(pr_row, pr_col);
		map->create_room(pr_coord.roomx(), pr_coord.roomy(), dyn_objects);
		princess = std::make_shared<Princess>(map->get_rand_free_cell(pr_coord));
		put_character(princess);
		
		glob_map_special.push_back(GMapSpecPair(knight, knight->symb()));
		glob_map_special.push_back(GMapSpecPair(princess, princess->symb()));

		BaseObject::turn = 0;
		mode = M_GAME;

		map->move_the_frame(knight->get_coord() - GCoord(MAP_HEIGHT/2, MAP_WIDTH/2));
		map->create_rooms(dyn_objects);
		return true;
	}

	void stop() {
		characters.clear();
		dyn_objects.clear();
		relief.clear();
		glob_map_special.clear();
	}

	void main_cycle() {
		render();
		while (!is_over()) {
			mode = Control::instance().put_command();
			switch (mode) {
				break;
				case M_GAME: {
					next_turn();
				}
				break;
				case M_MAP: {
					clear();
					map->show_global_map(glob_map_special, 4, 4);
				}
				break;
				case M_SETTINGS: {
					char sg = Control::instance().get_command();
					switch (sg) {
						break;
						case CMD_MAP: {
							Config::instance().show_minimap = !Config::instance().show_minimap;						
						}
						break;
						case CMD_COORD: {
							Config::instance().show_coordinates = !Config::instance().show_coordinates;
						}
					}
				}
			}
			if (mode != M_MAP) {
				render();
				if (Config::instance().show_minimap) {
	 				map->show_global_map(glob_map_special, 2, MAP_WIDTH + 4);
	 			}
			} 		
	 	}
	}

	void replay() {
		stop();
		if(!init()) {
			return;
		}
		main_cycle();
	}
} Game;

int main(int argc, char** argv) {
	if (!Game.init()) {
		return 0;
	}
	Game.init_curses();
 	Game.main_cycle();
 	while (true) {
 		Game.status = "PRESS '" + std::string(1, CMD_QUIT) + "' TO QUIT OR '" + std::string(1, CMD_REPLAY) + "' TO REPLAY";
 		Game.gprint(0, MAP_WIDTH/2 - Game.status.size()/2, Game.status.c_str());
 		Game.render();
 		char what_to_do = getch();
 		// Clear 0-th row
 		Game.gprint(0, 0, " ");
 		if (what_to_do == CMD_QUIT) {
 			Game.stop();
 			break;
 		}
 		if (what_to_do == CMD_REPLAY) {
 			Game.replay();
 		}
 	}
 	endwin();
 	return 0;
}
