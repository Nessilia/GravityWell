#include "Scene1.h"
#include <SDL.h>
#include <SDL_image.h>
#include <MMath.h>
#include <iostream>
#include "Entity.h"

Scene1::Scene1(SDL_Window* sdlWindow_){
	window = sdlWindow_;
	renderer = nullptr;
	flappy = nullptr;
	xAxis = 30.0f;
	yAxis = 15.0f;
	IMG_Init(IMG_INIT_PNG);
}

Scene1::~Scene1(){
	IMG_Quit();
}

bool Scene1::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);
	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, -1.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	

	flappy = new Entity();
	flappy->pos = Vec3(5.0f, 5.0f, 0.0f);
	flappy->SetImage("flappybird.png", renderer);
	return true;
}

void Scene1::OnDestroy() {
	delete flappy;
	flappy = nullptr;
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	switch (event.type) {
	case SDL_KEYDOWN:
		// Change angle
		if (event.key.keysym.scancode == SDL_SCANCODE_O) {
			flappy->angleDeg -= 10.0f;
		}
		if (event.key.keysym.scancode == SDL_SCANCODE_P) {
			flappy->angleDeg += 10.0f;
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
		flappy->pos = mousePosWorldSpace;
	}
		break;

	default:
		break;
	}
}


void Scene1::Update(const float deltaTime) {
	/// Physics goes here
	Vec3 gravAccel(0.0, -9.8, 0.0);
	Vec3 gravForce = flappy->mass * gravAccel;
	flappy->ApplyForce(gravForce);
	flappy->Update(deltaTime);

	// Make a circle around flappy
	for (int i = 0; i < flappy->points.size(); i++) {
		float angleDeg = i; // Scott will hate this, I need to cast into a float
		float angleRad = angleDeg * 2 * M_PI / 360.0f;
		// Following Umer's scribbles on the board
		// Patryck says, also add flappy's pos
		flappy->points[i].x = flappy->r * cos(angleRad) + flappy->pos.x;
		flappy->points[i].y = flappy->r * sin(angleRad) + flappy->pos.y;
	}

}

void Scene1::Render() const {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	Vec3 screenCoords = projectionMatrix * flappy->pos;
	SDL_Rect square;
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = flappy->GetSurface()->w;
	square.h = flappy->GetSurface()->h;
	SDL_RenderCopyEx(renderer, flappy->GetTexture(), nullptr, &square, flappy->angleDeg, nullptr, SDL_FLIP_NONE);
	
	// Draw circle of points
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
	for (int i = 0; i < flappy->points.size(); i++){
		Vec3 screenCoords = projectionMatrix * flappy->points[i];
		SDL_RenderDrawPoint(
			renderer, 
			static_cast<int>(screenCoords.x), 
			static_cast<int>(screenCoords.y));
	}
	
	SDL_RenderPresent(renderer);
}