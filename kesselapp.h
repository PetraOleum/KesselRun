#ifndef KESSELAPP_H
#define KESSELAPP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#define LOOP_TIME (1000/30)

#define INIT_SCREEN_WIDTH 500
#define INIT_SCREEN_HEIGHT 500

/// @brief Application class for Kessel Run
class KesselApp {
	private:
		/// @brief Is the app running
		bool running = false;

		/// @brief Is the app fullscreen
		bool fullscreen = false;

		/// @brief Holds the window
		SDL_Window* window = NULL;

		/// @brief Holds the surface
		SDL_Surface* screensurface = NULL;

		/// @brief Holds the renderer
		SDL_Renderer* renderer = NULL;

	public:
		/// @brief Constructor
		KesselApp();

		/// @brief Destructor
		~KesselApp();

		/// @brief Run the app
		///
		/// @return Execution status
		int OnExecute();

		/// @brief Initialise the app
		///
		/// @return Success/failure
		bool OnInit();

		/// @brief Handle events
		///
		/// @param event The event to handle
		void OnEvent(SDL_Event* event);

		/// @brief Run for every loop
		void OnLoop();

		/// @brief Rendering code
		void OnRender();

		/// @brief Cleanup code at finish
		void OnCleanup();
};

#endif
