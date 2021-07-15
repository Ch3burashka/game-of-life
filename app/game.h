#pragma once

#include "Life.h"
#include <SDL2/SDL.h>

class Game
{
  private:
	SDL_Window* m_window_ = nullptr;
	SDL_Renderer* m_renderer_ = nullptr;
	Life* life_ptr_;

	int width_;
	int height_;
	int cell_size_ = 10;
	SDL_Color grid_background = {22, 22, 22, 255}; // Barely Black
	SDL_Color grid_line_color = {44, 44, 44, 255}; // Dark grey
	SDL_Color grid_cursor_ghost_color = {255, 255, 255, 255};
	SDL_Color grid_cursor_ghost2_color = {80, 44, 44, 255}; // Dark grey

	SDL_Color grid_cursor_color = {27, 255, 212, 255};

	int window_width = 0;
	int window_height = 0;
	bool m_running = false;
	int delay_ = 10;
	SDL_bool mouse_active = SDL_FALSE;
	SDL_bool mouse_hover = SDL_FALSE;
	std::vector<SDL_Rect> GetRectsOnCurrentState();

	SDL_Rect grid_cursor = {
		.x = (width_ - 1) / 2 * cell_size_,
		.y = (height_ - 1) / 2 * cell_size_,
		.w = cell_size_,
		.h = cell_size_,
	};

	SDL_Rect grid_cursor_ghost = {grid_cursor.x, grid_cursor.y, cell_size_, cell_size_};
	bool pressed = false;
	std::string backup_path_;

  public:
	Game(Life* lifePtr, const std::string& path, int delay = 10, int size = 10);
	void HandleEvents();
	void Render();
	void RenderLoop();
	~Game();
};
