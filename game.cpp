#include "game_objects.h"
#include <iostream>
#include <map>
#include <ctime>


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
	std::list<ObjectPtr> objects;
	std::list<ObjectPtr> empties;
	CharacterPtr knight   = CharacterPtr(new Knight(4, MAP_WIDTH-3, HP_KNIGHT, DMG_KN_SWORD));
	CharacterPtr princess = CharacterPtr(new Princess(1, MAP_WIDTH-2, HP_PRINCESS, DMG_PRINCESS));
	Map map;
	unsigned int counter = 0;

	bool is_over() {
		return knight->hitpoints() <= 0 || princess->hitpoints() <= 0;
	}

	void refresh_characters_objects() {
		for(auto obj: objects) {
			map[obj->prevrow()][obj->prevcol()].remove(obj);
			map << obj;	
		}
		for(auto ch: characters) {
			if (ch->hitpoints() <= 0) {
				map[ch->getrow()][ch->getcol()].remove(ch);
			}
		}
		characters.remove_if([&](CharacterPtr ch) {
			return ch->hitpoints() <= 0 && ch != knight;
		});
	}

	void next_turn() {
		++counter;

		CharacterBoolMap did_attack;
		for(auto ch: characters) {
			did_attack.insert(CharacterBoolPair(ch, ch->attack(characters, objects, ch)));
		}
		for(auto ch: characters) {
			if (!did_attack[ch]) {
				ch->move();
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
		cout << "HP: " << characters.front()->hitpoints() << "\n"; 
		cout << "enemies cnt: " << characters.size()-2 << "\n\n";
	}

	void generate_level() {
		objects.push_back(ObjectPtr(new Wall(MAP_HEIGHT/2, MAP_WIDTH/2)));
		map << objects.back();
	}

	void init() {
		srand(time(0));
		for(int i = 0; i < MAP_HEIGHT; i++) {
			for(int j = 0; j < MAP_WIDTH; j++) {
				empties.push_back(ObjectPtr(new Object(i, j)));
				map << empties.back();
			}
		}
		for(int i = 0; i < MAP_HEIGHT; i++) {
			objects.push_back(ObjectPtr(new Wall(i, 0)));
			map << objects.back();
			objects.push_back(ObjectPtr(new Wall(i, MAP_WIDTH-1)));
			map << objects.back();
		}
		for(int j = 0; j < MAP_WIDTH; j++) {
			objects.push_back(ObjectPtr(new Wall(0, j)));
			map << objects.back();
			objects.push_back(ObjectPtr(new Wall(MAP_HEIGHT-1, j)));
			map << objects.back();
		}
		generate_level();

		characters.push_back(knight);
		map << characters.back();
		characters.push_back(princess);
		map << characters.back();
		characters.push_back(CharacterPtr(new Dragon(4, MAP_WIDTH-4, HP_DRAGON, DMG_DRAGON)));
		map << characters.back();
	}
} Game;


int main(int argc, char** argv) {
	Game.init();
	Game.render();
 	while (!Game.is_over()) {
 		Game.next_turn();
 		Game.render();
 	}
 	Game.render();
 	return 0;
}
