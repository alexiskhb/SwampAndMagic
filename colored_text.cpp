#include "colored_text.h"

using namespace std;

Colored::Colored() : fg_code(FG_DEFAULT), bg_code(BG_DEFAULT) {

}

Colored::Colored(const BGColorCode acode) : fg_code(FG_DEFAULT), bg_code(acode) {

}

Colored::Colored(const FGColorCode acode) : fg_code(acode), bg_code(BG_DEFAULT) {

}

Colored::Colored(const FGColorCode afg_code, const BGColorCode abg_code) : fg_code(afg_code), bg_code(abg_code) {

}

Colored::Colored(const BGColorCode abg_code, const FGColorCode afg_code) : fg_code(afg_code), bg_code(abg_code) {

}

std::string Colored::to_string() {
	return 
		"\033[" + std::to_string(fg_code) + ";" + 
		std::to_string(bg_code) + "m";
}

std::ostream& operator<<(std::ostream& out, Colored color) {
	return out << "\033[" << color.fg_code << ';' << color.bg_code << "m";
}