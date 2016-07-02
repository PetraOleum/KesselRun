#ifndef KESSELAPP_H
#define KESSELAPP_H

#include <vector>
#include <random>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "space.h"

#define LOOP_TIME (1000/30)

#define INIT_SCREEN_WIDTH 500
#define INIT_SCREEN_HEIGHT 500

#define BLACK_HOLE_MASS 1000000000000000

#define MAX_RADIAL_DISTANCE 5000
#define MIN_RADIAL_DISTANCE 300

//#define FALCON_STARTING_RADIUS (MAX_RADIAL_DISTANCE / 2)
#define FALCON_STARTING_RADIUS (MIN_RADIAL_DISTANCE - 10)

#define NUMBER_OF_ASTEROIDS 1000

#define MAX_SPEED 50

#define FALCON_DELTA_V 0.001

/// @brief Application class for Kessel Run
class KesselApp {
	private:
		/// @brief Is the app running
		bool running = false;

		/// @brief Is the app fullscreen
		bool fullscreen = false;

		/// @brief Whether or not the moue is down
		bool mouseDown = false;

		/// @brief Centre window on black hole
		void centreWindow();

		/// @brief Handle key-downs
		///
		/// @param keyEvent The KeyboardEvent
		void onKeyDown(SDL_KeyboardEvent* keyEvent);

		/// @brief Updated with window width each render
		int winX = INIT_SCREEN_WIDTH;

		/// @brief Updated with window height each render
		int winY = INIT_SCREEN_HEIGHT;

		/// @brief Holds the window
		SDL_Window* window = NULL;

		/// @brief Holds the surface
		SDL_Surface* screensurface = NULL;

		/// @brief The texture for the Falcon
		SDL_Texture* falcontexture = NULL;

		/// @brief Holds the renderer
		SDL_Renderer* renderer = NULL;

		/// @brief Vector of asteroids, also Asteroids
		std::vector<Asteroid> asteroids;

		/// @brief Random number generator
		std::mt19937 generator;

		/// @brief Random device (part of random number generation)
		std::random_device rd;

		/// @brief Distribution of random numbers for distance
		std::normal_distribution<> posdist;

		/// @brief Distribution of coulour of Asteroids (greyscale)
		std::uniform_int_distribution<Uint8> colourDistribution;

		/// @brief Distribution of radiuses of Asteroids
		std::uniform_int_distribution<int> sizeDistribution;

		/// @brief Distribution of random numbers for heading
		std::uniform_real_distribution<double> randheading;

		/// @brief The Black Hole
		Asteroid BlackHole;

		/// @brief The Falcon
		Asteroid Falcon;

		/// @brief The x offset of the viewing window
		int xOffset = INIT_SCREEN_WIDTH / 2;

		/// @brief The y offset of the viewing window
		int yOffset = INIT_SCREEN_HEIGHT / 2;


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
