#include <cstdio>
#include "kesselapp.h"

KesselApp::KesselApp() {

}

KesselApp::~KesselApp() {

}

int KesselApp::OnExecute() {
	if (!OnInit())
		return -1;

	SDL_Event event;

	while (running) {
		Uint32 lstart = SDL_GetTicks();
		while (SDL_PollEvent(&event))
			OnEvent(&event);
		OnLoop();
		OnRender();
		Uint32 lend = SDL_GetTicks();
		if ((lend - lstart) < LOOP_TIME)
			SDL_Delay(LOOP_TIME - (lend - lstart));
	}

	OnCleanup();
	return 0;
}

bool KesselApp::OnInit() {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Could not initialise SDL. SDL error: %s\n", SDL_GetError());
		return false;
	}
	window = SDL_CreateWindow("Kessel Run", 
			SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED, 
			INIT_SCREEN_WIDTH, 
			INIT_SCREEN_HEIGHT, 
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == NULL) {
		fprintf(stderr, "Window could not be created. SDL error: %s\n", SDL_GetError());
		return false;
	}
	int imgflags = IMG_INIT_PNG;
	if (! (IMG_Init(imgflags)) & imgflags) {
		fprintf(stderr, "SDL_Image could not be loaded. %s\n", IMG_GetError());
		return false;
	}
	screensurface = SDL_GetWindowSurface(window);
	if ((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)) == NULL) {
		fprintf(stderr, "Could not create renderer. SDL error: %s\n", SDL_GetError());
		return false;
	}
	SDL_Surface* loadsurface = IMG_Load("Falcon.png");
	if (loadsurface == NULL) {
		fprintf(stderr, "Could not load Falcon.png. %s\n", IMG_GetError());
		return false;
	}
	falcontexture = SDL_CreateTextureFromSurface(renderer, loadsurface);
	if (falcontexture == NULL) {
		fprintf(stderr, "Could not convert texture. %s\n", SDL_GetError());
		return false;
	}
	BlackHole =  {BLACK_HOLE_MASS, 0, 0, 100, 0, 0, {0x000000}, true, false};
	Falcon = {10, 
		sqrt((GRAVITATIONAL_CONSTANT * BLACK_HOLE_MASS) / FALCON_STARTING_RADIUS), 
		-M_PI_2, 
		10, 
		FALCON_STARTING_RADIUS, 0, 
		{0x0}, 
		true, true};
	Falcon.r = 0x00;
	Falcon.g = 0xFF;
	Falcon.b = 0x00;
	Falcon.a = 0xFF;
	generator = std::mt19937(rd());
	posdist = std::normal_distribution<>(
			MAX_RADIAL_DISTANCE / 2, MAX_RADIAL_DISTANCE / 4);
	randheading = std::uniform_real_distribution<double>(0, M_PI * 2);
	colourDistribution = std::uniform_int_distribution<Uint8>(0x10, 0xE0);
	sizeDistribution = std::uniform_int_distribution<int>(1, 20);

	for (int i = 0; i < NUMBER_OF_ASTEROIDS; i++) {
		bool nocollisions = true;
		Asteroid newast;
		newast.radius = sizeDistribution(generator);
		newast.mass = newast.radius * newast.radius * M_PI;
		double radialdist;
		newast.heading = randheading(generator);
		double theta = newast.heading + M_PI / 2;
		do
			radialdist = posdist(generator);
		while (radialdist > MAX_RADIAL_DISTANCE || radialdist < MIN_RADIAL_DISTANCE);
		newast.x = radialdist * cos(theta);
		newast.y = radialdist * sin(theta);
		for (int q = 0; q < i && nocollisions; q++)
			if (collision(newast, asteroids[q]))
				nocollisions = false;
		// This is utterly stupid, but I can't get the obvious
		// do-while loop to work; seems to crash the random
		// number generator somehow. May also be to do with the
		// union in the Asteroid struct
		if (!nocollisions) {
			i--;
			continue;
		}
		newast.speed = sqrt((GRAVITATIONAL_CONSTANT * BLACK_HOLE_MASS) / radialdist);
		Uint8 cVal = colourDistribution(generator);
		newast.r = newast.g = newast.b = cVal;
		newast.a = 0xFF;
		newast.alive = true;
		newast.movable = true;
		asteroids.push_back(newast);
//		printf("%d: %f; %f\n", i, newast.x, newast.y);
	}
//	printf("%f\n", GRAVITATIONAL_CONSTANT * BLACK_HOLE_MASS);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	running = true;
	return true;
}

void KesselApp::OnLoop() {
	for (unsigned int i = 0; i < asteroids.size(); i++) {
		Asteroid ast = asteroids[i];
		if (!ast.alive || !ast.movable)
			continue;
		double ga = gravitationalAcceleration(BlackHole, ast);
		double gaheading = headingTo(ast, BlackHole);
		if (ast.speed > MAX_SPEED) ast.speed = MAX_SPEED;
		ast.x += ast.speed * cos(ast.heading);
		ast.y += ast.speed * sin(ast.heading);
//		printf("%f, %f; %f, %f\n", ast.speed, ast.heading, ga, gaheading);
		ast = deltaV(ast, ga, gaheading);
		asteroids[i] = ast;
	}
	double fga = gravitationalAcceleration(BlackHole, Falcon);
	double fgaheading = headingTo(Falcon, BlackHole);
	if (Falcon.speed > MAX_SPEED) Falcon.speed = MAX_SPEED;
	Falcon.x += Falcon.speed * cos(Falcon.heading);
	Falcon.y += Falcon.speed * sin(Falcon.heading);
	printf("%f, %f\t", fga, fgaheading);
	Falcon = deltaV(Falcon, fga, fgaheading);

	
	for (unsigned int i1 = 0; i1 < asteroids.size(); i1++) 
		if (asteroids[i1].alive) {
			if (collision(asteroids[i1], BlackHole))
				asteroids[i1].alive = false;
			else {
				for (unsigned int i2 = i1 + 1; i2 < asteroids.size(); i2++)
					if (asteroids[i2].alive)
						if (collision(asteroids[i1], asteroids[i2]))
							handleCollision(&asteroids[i1], &asteroids[i2]);
				if (collision(asteroids[i1], Falcon))
					handleCollision(&asteroids[i1], &Falcon);
			}
		}

//	printf("\n");
}

void KesselApp::OnRender() {
	SDL_GetWindowSize(window, &winX, &winY);
	centreWindow();
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	circleColor(renderer, 
			(int)BlackHole.x + xOffset,
			(int)BlackHole.y + yOffset,
			(int)BlackHole.radius,
			0xFFFFFFFF);

	for (unsigned int i = 0; i < asteroids.size(); i++) {
		Asteroid ast = asteroids[i];
		if (!ast.alive)
			continue;
		filledCircleColor(renderer, 
				(int)ast.x + xOffset, 
				(int)ast.y + yOffset, 
				(int)ast.radius, 
				ast.RGBA);
	}
//	filledCircleColor(renderer,
//			(int)Falcon.x + xOffset,
//			(int)Falcon.y + yOffset,
//			(int)Falcon.radius,
//			Falcon.RGBA);
//
	SDL_Rect falcon_rect = {((int)Falcon.x + xOffset) - (int)Falcon.radius, ((int)Falcon.y + yOffset) - (int)Falcon.radius, (int)Falcon.radius * 2, (int)Falcon.radius * 2};
	SDL_RenderCopyEx(renderer, falcontexture, NULL, &falcon_rect, (Falcon.heading + M_PI_2) / M_PI * 180, NULL, SDL_FLIP_NONE);
	printf("%f; %f\t(%f, %f)\n", Falcon.speed, Falcon.heading, Falcon.x, Falcon.y);
	SDL_RenderPresent(renderer);
}

void KesselApp::OnEvent(SDL_Event* event) {
	switch (event->type) {
		case SDL_QUIT:
			running = false;
			break;
		case SDL_KEYDOWN:
			onKeyDown(&event->key);
			break;
		default:
			break;
	}
}

void KesselApp::OnCleanup() {
	SDL_DestroyTexture(falcontexture);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = NULL;
	renderer = NULL;
	IMG_Quit();
	SDL_Quit();
}

void KesselApp::centreWindow() {
//	xOffset = (winX / 2) - asteroids[0].x;
//	yOffset = (winY / 2) - asteroids[0].y;
//	xOffset = (winX / 2);
//	yOffset = (winY / 2);
	xOffset = (winX / 2) - Falcon.x;
	yOffset = (winY / 2) - Falcon.y;
}

void KesselApp::onKeyDown(SDL_KeyboardEvent* keyEvent) {
	switch (keyEvent->keysym.sym) {
		case SDLK_c:
			centreWindow();
			break;
		case SDLK_UP:
			{
				double oang = Falcon.heading;
				double ospd = Falcon.speed;
				Falcon.heading = addVectorsAnglePart(ospd, oang, FALCON_DELTA_V, oang);
				Falcon.speed = addVectorsAnglePart(ospd, oang, FALCON_DELTA_V, oang);
			}
			break;
		case SDLK_RIGHT:
			{
				double oang = Falcon.heading;
				double ospd = Falcon.speed;
				Falcon.heading = addVectorsAnglePart(ospd, oang, FALCON_DELTA_V, oang + M_PI_2);
				Falcon.speed = addVectorsAnglePart(ospd, oang, FALCON_DELTA_V, oang + M_PI_2);
			}
			break;
		case SDLK_LEFT:
			{
				double oang = Falcon.heading;
				double ospd = Falcon.speed;
				Falcon.heading = addVectorsAnglePart(ospd, oang, FALCON_DELTA_V, oang - M_PI_2);
				Falcon.speed = addVectorsAnglePart(ospd, oang, FALCON_DELTA_V, oang - M_PI_2);
			}
			break;
		case SDLK_DOWN:
			{
				double oang = Falcon.heading;
				double ospd = Falcon.speed;
				Falcon.heading = addVectorsAnglePart(ospd, oang, 1, oang - M_PI);
				Falcon.speed = addVectorsAnglePart(ospd, oang, 1, oang - M_PI);
			}
			break;
		default:
			break;
	}
}
