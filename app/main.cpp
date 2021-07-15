#include "Life.h"
#include "game.h"
#include <SDL2/SDL.h>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

namespace fs = boost::filesystem;

int main(int argc, char** argv)
{
	// Add state near binary
	fs::path full_path(fs::initial_path<fs::path>());
	full_path = fs::system_complete(fs::path(argv[0]));
	fs::path bin_state_path = full_path.parent_path() / "state.bin";
	std::string bin_path_str = bin_state_path.string();




	Life life(80, 50);
	Game renderer(&life, bin_path_str, 10, 15);
	renderer.RenderLoop();
}