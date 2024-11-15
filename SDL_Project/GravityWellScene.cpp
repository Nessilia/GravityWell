#include "GravityWellScene.h"
#include <SDL.h>
#include <SDL_image.h>
#include <MMath.h>
#include <iostream>
#include "Entity.h"



gravityWellScene::gravityWellScene(SDL_Window* sdlWindow_){
	window = sdlWindow_;
	renderer = nullptr;
	planet = nullptr;
	xAxis = 30.0f;
	yAxis = 30.0f;
	IMG_Init(IMG_INIT_PNG);
}

gravityWellScene::~gravityWellScene(){
	IMG_Quit();
}


bool gravityWellScene::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);
	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, -1.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	backGround = new Entity();
	backGround->pos = Vec3(0.0f, 30.0f,0.0f);
	backGround->SetImage("cosmos.png", renderer);


	planet = new Entity();
	planet->pos = Vec3(10.0f, 20.0f, 0.0f);
	planet->SetImage("planet.png", renderer);
	planet->vel = Vec3(0.0f, 4.0f, 0.0f);
	planet->mass = 100.0f;

	star1 = new Entity();
	star1->pos = Vec3(5, 10, 0);
	star1->SetImage("star3.png", renderer);
	star1->mass = 100.0f;


	star2 = new Entity();
	star2->pos = Vec3(15, 25, 0);
	star2->SetImage("star3.png", renderer);
	star2->mass = 150.0f;


	return true;
}

void gravityWellScene::OnDestroy() {
	delete planet;
	planet = nullptr;

	delete star1;
	star1 = nullptr;

	delete star2;
	star2 = nullptr;
}

void gravityWellScene::HandleEvents(const SDL_Event& event)
{
	switch (event.type) {
	case SDL_KEYDOWN:
		// Change angle
		if (event.key.keysym.scancode == SDL_SCANCODE_O) {
			planet->angleDeg -= 10.0f;
		}
		if (event.key.keysym.scancode == SDL_SCANCODE_P) {
			planet->angleDeg += 10.0f;
		}
		break;

	case SDL_MOUSEBUTTONDOWN:
		// Get physics coordinate of mouse position
	{
		Vec3 mousePosPixelSpace(event.button.x, event.button.y, 0.0f);
		Matrix4 worldToPixelSpace = projectionMatrix;
		Matrix4 pixelToWorldSpace = MMath::inverse(projectionMatrix);

		Vec3 mousePosWorldSpace = pixelToWorldSpace * mousePosPixelSpace;
		mousePosPixelSpace.print("mouse pos pixel space");
		mousePosWorldSpace.print("mouse pos world space");
		planet->pos = mousePosWorldSpace;
	}
		break;

	default:
		break;
	}
}


void gravityWellScene::Update(const float deltaTime) {
	/// Physics goes here
	
	// Following my instructions in the assignment doc
	// STEP 1
	// Find vector from planet to star 1
	// Vec3 PS1 = ....
	const float G = 1.0f;

	// Step 1: Find the vector from planet to star 1
	Vec3 PS1 = star1->pos - planet->pos;
	// STEP 2
	// Find distance from planet to star 1
	// Use pythag theory
	// float r1 = ...
	float r1_squared = PS1.x * PS1.x + PS1.y * PS1.y + PS1.z * PS1.z;
	// STEP 
	// Find the magnitude of the force of gravity
	// Code up Newtons Law of Gravitation
	// float fg1_magnitude = ...
	// Step 3: Calculate the gravitational force magnitude from star 1 using r1_squared directly
	float fg1_magnitude = (G * star1->mass * planet->mass) / r1_squared;
	// STEP 4
	// Find the direction of gravity from planet to star 1
	// Normalize the vector from planet to star 1
	// Vec3 fg1_direction = ....
	Vec3 fg1_direction;
	if (r1_squared > 0) {  // Avoid division by zero
		float r1 = std::sqrt(r1_squared);
		fg1_direction = Vec3(PS1.x / r1, PS1.y / r1, PS1.z / r1);
	}
	else {
		fg1_direction = Vec3(0.0f, 0.0f, 0.0f);  // If on top of each other, set no direction
	}
	// STEP 5
	// Combine magnitude with direction
	// Vec3 fg1 = fg1_magnitude * fg1_direction;
	Vec3 fg1 = fg1_direction * fg1_magnitude;
	// STEP 6
	// Do this all over again (steps 1 - 5) for star 2
	Vec3 PS2 = star2->pos - planet->pos;
	float r2_squared = PS2.x * PS2.x + PS2.y * PS2.y + PS2.z * PS2.z;
	float fg2_magnitude = (G * star2->mass * planet->mass) / r2_squared;

	Vec3 fg2_direction;
	if (r2_squared > 0) {
		float r2 = std::sqrt(r2_squared);
		fg2_direction = Vec3(PS2.x / r2, PS2.y / r2, PS2.z / r2);
	}
	else {
		fg2_direction = Vec3(0.0f, 0.0f, 0.0f);
	}

	Vec3 fg2 = fg2_direction * fg2_magnitude;
	// STEP 7
	// Add up the forces
	//Vec3 totalForce;
	// totalForce = fg1 + fg2;
	Vec3 totalForce = fg1 + fg2;
	   	  
	planet->ApplyForce(totalForce);
	planet->Update(deltaTime);



	if (planet->pos.x < 0) planet->pos.x = 0;
	if (planet->pos.y < 0) planet->pos.y = 0;
	if (planet->pos.x > xAxis) planet->pos.x = xAxis;
	if (planet->pos.y > yAxis) planet->pos.y = yAxis;
	// Make a circle around planet
	for (int i = 0; i < planet->points.size(); i++) {
		float angleDeg = i; // Scott will hate this, I need to cast into a float
		float angleRad = angleDeg * 2 * M_PI / 360.0f;
		// Following Umer's scribbles on the board
		// Patryck says, also add flappy's pos
		planet->points[i].x = planet->r * cos(angleRad) + planet->pos.x;
		planet->points[i].y = planet->r * sin(angleRad) + planet->pos.y;
	}

}

void gravityWellScene::Render() const {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	{
		Vec3 screenCoords3 = projectionMatrix * backGround->pos;
		SDL_Rect square3;
		square3.x = static_cast<int>(screenCoords3.x);
		square3.y = static_cast<int>(screenCoords3.y);
		float scale = 3.0f;
		square3.w = backGround->GetSurface()->w * scale *1.1;
		square3.h = backGround->GetSurface()->h * scale;
		SDL_RenderCopyEx(renderer, backGround->GetTexture(), nullptr, &square3, backGround->angleDeg, nullptr, SDL_FLIP_NONE);
	}


	{
		Vec3 screenCoords = projectionMatrix * planet->pos;
		SDL_Rect square;
		square.x = static_cast<int>(screenCoords.x);
		square.y = static_cast<int>(screenCoords.y);
		float scale = 0.4f;
		square.w = planet->GetSurface()->w * scale;
		square.h = planet->GetSurface()->h * scale;
		SDL_RenderCopyEx(renderer, planet->GetTexture(), nullptr, &square, planet->angleDeg, nullptr, SDL_FLIP_NONE);
	}

	{
		Vec3 screenCoords1 = projectionMatrix * star1->pos;
		SDL_Rect square1;
		square1.x = static_cast<int>(screenCoords1.x);
		square1.y = static_cast<int>(screenCoords1.y);
		float scale = 0.3f;
		square1.w = star1->GetSurface()->w * scale;
		square1.h = star1->GetSurface()->h * scale;
		SDL_RenderCopyEx(renderer, star1->GetTexture(), nullptr, &square1, star1->angleDeg, nullptr, SDL_FLIP_NONE);
	}
	{
		Vec3 screenCoords2 = projectionMatrix * star2->pos;
		SDL_Rect square2;
		square2.x = static_cast<int>(screenCoords2.x);
		square2.y = static_cast<int>(screenCoords2.y);
		float scale = 0.3f;
		square2.w = star2->GetSurface()->w * scale;
		square2.h = star2->GetSurface()->h * scale;
		SDL_RenderCopyEx(renderer, star2->GetTexture(), nullptr, &square2, star2->angleDeg, nullptr, SDL_FLIP_NONE);
	}
	



	// Draw circle of points
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
	for (int i = 0; i < planet->points.size(); i++){
		Vec3 screenCoords = projectionMatrix * planet->points[i];
		SDL_RenderDrawPoint(
			renderer, 
			static_cast<int>(screenCoords.x), 
			static_cast<int>(screenCoords.y));
	}
	
	SDL_RenderPresent(renderer);
}