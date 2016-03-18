#include "base_objects.h"
#include "objects.h"
#include "characters.h"
#include <map>
#include <ctime>
#include "colored_text.h"
#include <string>
#include <curses.h>
#include <algorithm>
#include "bilateral_array.h"
#include <cstdlib>
#include "control.h"


typedef std::map<CharacterPtr, bool> CharacterBoolMap;
typedef std::pair<CharacterPtr, bool> CharacterBoolPair;


struct {
	// CHARACTERS
	// first(): player(knight)
	// next   : princess
	// third  : dragon
	// >3: zombies and other(if any)
	std::list<CharacterPtr> characters;
	std::list<ObjectPtr> dyn_objects;
	std::list<BaseObjectPtr> relief;
	CharacterPtr knight;
	CharacterPtr princess;
	MapPtr map;
	std::string status;
	unsigned int counter;

	bool is_over() {
		return characters.size() <= 2 || knight->hitpoints() <= 0 || princess->hitpoints() <= 0;
	}

	bool is_lose() {
		return knight->hitpoints() <= 0 || princess->hitpoints() <= 0;
	}

	void kill_died_characters_objects() {
		for(auto obj: dyn_objects) {
			if (map->is_on_the_map(obj->getrow(), obj->getcol())) {
				(*map)(obj->prevrow(), obj->prevcol()).remove(obj);
			}
			if (obj->is_alive()) {
				*map << obj;
			}
		}
		for(auto ch: characters) {
			if (ch->hitpoints() <= 0) {
				(*map)(ch->getrow(), ch->getcol()).remove(ch);
			}
		}
		characters.remove_if([&](CharacterPtr ch) {
			return ch->hitpoints() <= 0 && ch != knight;
		});
		dyn_objects.remove_if([&](ObjectPtr obj) {
			return !obj->is_alive();
		});
	}

	bool next_turn() {
		++counter;
		map->move_the_frame(knight->get_coord() - knight->get_prev());
		CharacterBoolMap did_attack;
		for(auto ch_iter = std::next(characters.begin()); ch_iter != characters.end(); ch_iter++) {
			auto ch = *ch_iter;
			did_attack.insert(CharacterBoolPair(ch, ch->attack(characters, dyn_objects, *map)));
		}
		did_attack.insert(CharacterBoolPair(knight, knight->attack(characters, dyn_objects, *map)));
		for(auto obj: dyn_objects) {
			if (map->is_on_the_map(obj->getrow(), obj->getcol())) {
				obj->impact(characters, dyn_objects);
				(*map)(obj->getrow(), obj->getcol()).remove(obj);
				*map << obj;
				obj->turn();
			}
			else {
				obj->move_to_prev();
				obj->destroy();
			}
		}
		for(auto ch: characters) {
			if (!did_attack[ch]) {
				ch->move(*map, characters);
				if (!map->is_on_the_map(ch->getrow(), ch->getcol())) {
					ch->move_to_prev();
					ch->suffer(ch->hitpoints());
				}
				else
				// changes will be accepted after calling kill_died_characters_objects()
				// so we can cancel try to move on wall here. prev_coords now match with actual.
				if (!map->is_penetrable(ch->getrow(), ch->getcol())) {
					ch->move_to_prev();
				}
				else {
					// here we attach a cell to character
					// so other characters can not step on it
					(*map)(ch->prevrow(), ch->prevcol()).remove(ch);
					*map << ch;
				}
			}
		}
		kill_died_characters_objects();
		map->create_rooms();
		return !knight->has_plans();
	}

	void gprint(int row, int col, const char* text) {
		move(row, col);
		clrtobot();
		addstr(text);
	}

	inline void render() {
		int shift = 2;
		map->display(shift);
		BaseObjectPtr obj = 
			map->nearest_symb(
				IntIntPair(knight->getrow(), knight->getcol()), 
				std::string(ENEMIES), 
				std::min(MAP_HEIGHT, MAP_WIDTH)/2);
		status = "HP: " + std::to_string(characters.front()->hitpoints());
		gprint(MAP_HEIGHT+shift, MAP_WIDTH/2 - status.size()/2 - status.size()%2, status.c_str());
		status = "NEAREST CHARACTER: " + std::to_string(obj->hitpoints());
		gprint(MAP_HEIGHT+shift+1, MAP_WIDTH/2 - status.size()/2 - status.size()%2, status.c_str());
		// cout << "(" << knight->getcol() << "; " << -knight->getrow() << ")\n"; 
	}

	void put_character(CharacterPtr ch) {
		characters.push_back(ch);
		*map << ch;
	}

	void put_dynobject(ObjectPtr obj) {
		dyn_objects.push_back(obj);
		*map << obj;	
	}

	void init() {
		map = std::make_shared<Map>(relief);
		knight = std::make_shared<Knight>(MAP_HEIGHT/2, MAP_WIDTH/2, HP_KNIGHT, DMG_KN_SWORD);
		princess = std::make_shared<Princess>(1, MAP_WIDTH-2, HP_PRINCESS, DMG_PRINCESS);
		counter = 0;
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
		
		put_character(knight);
		put_character(princess);
		put_character(std::make_shared<Dragon>(4, MAP_WIDTH-4));
		put_character(std::make_shared<Warlock>(10, MAP_WIDTH-10));
		put_dynobject(std::make_shared<Medkit>(MAP_HEIGHT/2, MAP_WIDTH/2));
		for(int i = 0; i < MAP_HEIGHT; i++) {
			for(int j = 0; j < MAP_WIDTH; j++) {
				if (map->is_penetrable(i, j) && chance(2)) {
					characters.push_back(std::make_shared<Zombie>(i, j, HP_ZOMBIE, DMG_ZOMBIE));
					*map << characters.back();
				}
			}
		}
	}

	void stop() {
		characters.clear();
		dyn_objects.clear();
		relief.clear();
	}

	void main_cycle() {
		render();
		while (!is_over()) {
	 		if (next_turn()) {
	 			render();
	 		}
	 	}
	 	render();
	}

	void replay() {
		stop();
		init();
		main_cycle();
	}
} Game;


int main(int argc, char** argv) {
	Game.init();
 	Game.main_cycle();
 	while (true) {
 		Game.status = std::string("PRESS 'Q' TO QUIT OR 'R' TO REPLAY");
 		Game.gprint(0, MAP_WIDTH/2 - Game.status.size()/2, Game.status.c_str());
 		Game.render();
 		char what_to_do = getch();
 		Game.gprint(0, 0, " ");
 		if (what_to_do == 'Q') {
 			Game.stop();
 			break;
 		}
 		if (what_to_do == 'r' || what_to_do == 'R') {
 			Game.replay();
 		}
 	}
 	endwin();
 	return 0;
}
