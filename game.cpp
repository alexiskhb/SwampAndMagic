#include "base_objects.h"
#include "objects.h"
#include "characters.h"
#include <iostream>
#include <map>
#include <ctime>
#include "colored_text.h"


using std::cout;


typedef std::map<CharacterPtr, bool> CharacterBoolMap;
typedef std::pair<CharacterPtr, bool> CharacterBoolPair;

// struct Game
struct {
	// CHARACTERS
	// [0] : player(knight)
	// [1] : princess
	// [2] : dragon
	// [3+]: zombies and other(if any)
	std::list<CharacterPtr> characters;
	std::list<ObjectPtr> dyn_objects;
	std::list<BaseObjectPtr> relief;
	CharacterPtr knight   = CharacterPtr(new Knight(4, 3, HP_KNIGHT, DMG_KN_SWORD));
	CharacterPtr princess = CharacterPtr(new Princess(1, MAP_WIDTH-2, HP_PRINCESS, DMG_PRINCESS));
	Map map;
	unsigned int counter = 0;

	bool is_over() {
		return knight->hitpoints() <= 0 || princess->hitpoints() <= 0;
	}

	void refresh_characters_objects() {
		for(auto obj: dyn_objects) {
			if (map.is_on_the_map(obj->getrow(), obj->getcol())) {
				map[obj->prevrow()][obj->prevcol()].remove(obj);
			}
			if (obj->is_alive()) {
				map << obj;
			}
		}
		for(auto ch: characters) {
			if (ch->hitpoints() <= 0) {
				map[ch->getrow()][ch->getcol()].remove(ch);
			}
		}
		characters.remove_if([&](CharacterPtr ch) {
			return ch->hitpoints() <= 0 && ch != knight;
		});
		dyn_objects.remove_if([&](ObjectPtr obj) {
			return !obj->is_alive();
		});
	}

	void next_turn() {
		++counter;
		CharacterBoolMap did_attack;
		for(auto ch: characters) {
			did_attack.insert(CharacterBoolPair(ch, ch->attack(characters, dyn_objects, ch)));
		}
		for(auto obj: dyn_objects) {
			if (map.is_on_the_map(obj->getrow(), obj->getcol())) {
				obj->impact(characters, dyn_objects);
				map[obj->getrow()][obj->getcol()].remove(obj);
				map << obj;
				obj->turn();
			}
			else {
				obj->move_to_prev();
				obj->destroy();
			}
		}
		for(auto ch: characters) {
			if (!did_attack[ch]) {
				ch->move(map, characters);
				if (!map.is_on_the_map(ch->getrow(), ch->getcol())) {
					ch->move_to_prev();
					ch->suffer(ch->hitpoints());
					cout << "AHAHAHAAAHAA\n";
				}
				else
				// changes will be accepted after calling refresh_characters_objects()
				// so we can cancel try to move on wall here. prev_coords now match with actual.
				if (!map.is_penetrable(ch->getrow(), ch->getcol())) {
					ch->move_to_prev();
				}
				else {
					// here we attach a cell to character
					// so other characters can not step on it
					map[ch->prevrow()][ch->prevcol()].remove(ch);
					map << ch;
				}
			}
		}
		refresh_characters_objects();
	}

	inline void render() {
		cout << map;
		cout << "HP: " << characters.front()->hitpoints() << "	DRAGON: " << (*next(next(characters.begin())))->hitpoints() << "\n"; 
		cout << "enemies cnt: " << characters.size()-2 << "\n\n";
	}

	void generate_level() {
		map.generate(42, 7);
		for(int i = 0; i < MAP_HEIGHT; i++) {
			for(int j = 0; j < MAP_WIDTH; j++) {
				relief.push_back(ObjectPtr(new Object(i, j)));
				map << relief.back();
			}
		}
		for(int i = 0; i < MAP_HEIGHT; i++) {
			for(int j = 0; j < MAP_WIDTH; j++) {
				if (map.gen_is_wall(i, j)) {
					relief.push_back(BaseObjectPtr(new Wall(i, j)));
					map << relief.back(); 
				}
			}
		}
	}

	void init() {
		srand(time(0));
		generate_level();
		characters.push_back(knight);
		map << characters.back();
		characters.push_back(princess);
		map << characters.back();
		characters.push_back(CharacterPtr(new Dragon(4, MAP_WIDTH-4, HP_DRAGON, DMG_DRAGON)));
		map << characters.back();
		for(int i = 0; i < MAP_HEIGHT; i++) {
			for(int j = 0; j < MAP_WIDTH; j++) {
				if (map.is_penetrable(i, j) && chance(2, "")) {
					characters.push_back(CharacterPtr(new Zombie(i, j, HP_ZOMBIE, DMG_ZOMBIE)));
					map << characters.back();
				}
			}
		}
	}
} Game;


int main(int argc, char** argv) {
	cout << Colored(BG_BLACK, FG_B_YELLOW) << "aAADFDasFDSFas" << Colored() << std::endl;
	Game.init();
	Game.render();
 	while (!Game.is_over()) {
 		Game.next_turn();
 		Game.render();
 	}
 	Game.render();
 	return 0;
}
