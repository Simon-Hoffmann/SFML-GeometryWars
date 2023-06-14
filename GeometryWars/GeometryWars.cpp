#include <SFML/Graphics.hpp>

#include "Game.h"
#include <iostream>
#include <filesystem>
#include <string>
namespace fs = std::filesystem;

int main() {
	fs::path path = fs::current_path();
	std::string path_string = path.string();
	Game g(path_string);
	g.run();
}
