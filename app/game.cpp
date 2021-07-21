#include "game.h"

Game::Game(Life* lifePtr, const std::string& back_path, int delay, int size)
	: life_ptr_(lifePtr), width_(life_ptr_->GetWidth()), height_(life_ptr_->GetHeight()), delay_(delay),
	  cell_size_(size), backup_path_(back_path)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Initialize SDL: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	// + 1 so that the last grid lines fit in the screen.
	window_width = (width_ * cell_size_) + 1;
	window_height = (height_ * cell_size_) + 1;

	// Create an application window with the following settings:
	m_window_ = SDL_CreateWindow("Conway life",			  // window title
								 SDL_WINDOWPOS_UNDEFINED, // initial x position
								 SDL_WINDOWPOS_UNDEFINED, // initial y position
								 window_width,			  // width, in pixels
								 window_height,			  // height, in pixels
								 SDL_WINDOW_OPENGL		  // flags - see below
	);
	// Check that the window was successfully created
	if (m_window_ == NULL)
	{
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	m_renderer_ = SDL_CreateRenderer(m_window_, -1, SDL_RENDERER_PRESENTVSYNC);
	m_running = true;
}

Game::~Game()
{
	SDL_DestroyRenderer(m_renderer_);
	SDL_DestroyWindow(m_window_);
	m_renderer_ = nullptr;
	m_window_ = nullptr;
	SDL_Quit();
}
void Game::HandleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_r:
			{
				life_ptr_->FillRandomStates();
				break;
			}

		case SDLK_z:
			{
				life_ptr_->SaveState(backup_path_);
				break;
			}

		case SDLK_x:
			{
				life_ptr_->LoadState(backup_path_);
				break;
			}
		case SDLK_c:
		{
			life_ptr_->ClearStates();
			break;
		}

		case SDLK_ESCAPE:
			{
				pressed = !pressed;
				break;
			}

		case SDLK_w:
		case SDLK_UP:
			{
				grid_cursor.y -= cell_size_;
				break;
			}

		case SDLK_s:
		case SDLK_DOWN:
			{
				grid_cursor.y += cell_size_;
				break;
			}

		case SDLK_a:
		case SDLK_LEFT:
			{
				grid_cursor.x -= cell_size_;
				break;
			}

		case SDLK_d:
		case SDLK_RIGHT:
			{
				grid_cursor.x += cell_size_;
				break;
			}
		}
		break;

	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			grid_cursor.x = (event.motion.x / cell_size_) * cell_size_;
			grid_cursor.y = (event.motion.y / cell_size_) * cell_size_;
			life_ptr_->SetStateAlive(event.motion.x / cell_size_, event.motion.y / cell_size_);
		}
		else if (event.button.button == SDL_BUTTON_RIGHT)
		{
			grid_cursor.x = (event.motion.x / cell_size_) * cell_size_;
			grid_cursor.y = (event.motion.y / cell_size_) * cell_size_;
			life_ptr_->SetStateDead(event.motion.x / cell_size_, event.motion.y / cell_size_);
		}
		break;
	case SDL_MOUSEMOTION:
		{
			grid_cursor_ghost.x = (event.motion.x / cell_size_) * cell_size_;
			grid_cursor_ghost.y = (event.motion.y / cell_size_) * cell_size_;

			if (!mouse_active)
				mouse_active = SDL_TRUE;
			break;
		}

	case SDL_WINDOWEVENT:
		{
			if (event.window.event == SDL_WINDOWEVENT_ENTER && !mouse_hover)
				mouse_hover = SDL_TRUE;
			else if (event.window.event == SDL_WINDOWEVENT_LEAVE && mouse_hover)
				mouse_hover = SDL_FALSE;
			break;
		}

	case SDL_QUIT:
		{
			m_running = false;
			break;
		}
	}
}
void Game::RenderLoop()
{
	while (m_running)
	{
		HandleEvents();
		Render();
	}
}
void Game::Render()
{
	// Render background
	SDL_SetRenderDrawColor(m_renderer_, grid_background.r, grid_background.g, grid_background.b, grid_background.a);
	SDL_RenderClear(m_renderer_);
	// Render cells
	auto state = GetRectsOnCurrentState();
	for (auto& rect : state)
	{
		SDL_SetRenderDrawColor(m_renderer_, grid_cursor_ghost_color.r, grid_cursor_ghost_color.g,
							   grid_cursor_ghost_color.b, grid_cursor_ghost_color.a);
		SDL_RenderFillRect(m_renderer_, &rect);
	}
	// Draw grid ghost cursor.
	if (mouse_active && mouse_hover)
	{
		SDL_SetRenderDrawColor(m_renderer_, grid_cursor_ghost2_color.r, grid_cursor_ghost2_color.g,
							   grid_cursor_ghost2_color.b, grid_cursor_ghost2_color.a);
		SDL_RenderFillRect(m_renderer_, &grid_cursor_ghost);
	}
	// Draw grid lines.
	SDL_SetRenderDrawColor(m_renderer_, grid_line_color.r, grid_line_color.g, grid_line_color.b, grid_line_color.a);
	for (int x = 0; x < 1 + width_ * cell_size_; x += cell_size_)
	{
		SDL_RenderDrawLine(m_renderer_, x, 0, x, window_height);
	}
	for (int y = 0; y < 1 + height_ * cell_size_; y += cell_size_)
	{
		SDL_RenderDrawLine(m_renderer_, 0, y, window_width, y);
	}

	SDL_RenderPresent(m_renderer_);
	SDL_Delay(delay_);
	if (pressed)
	{
		life_ptr_->Mutate();
	}
}
std::vector<SDL_Rect> Game::GetRectsOnCurrentState()
{
	std::vector<SDL_Rect> result;
	for (int x = 0; x < width_; ++x)
	{
		for (int y = 0; y < height_; ++y)
		{
			if (life_ptr_->GetStateCell(x, y))
			{
				result.push_back({x * cell_size_, y * cell_size_, cell_size_, cell_size_});
			}
		}
	}
	return result;
}
