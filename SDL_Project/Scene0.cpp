#include "Scene0.h"
#include <SDL.h>
#include <SDL_image.h>
#include <MMath.h>
#include <iostream>
#include "Entity.h"

Scene0::Scene0(SDL_Window* sdlWindow_){
	window = sdlWindow_;
	renderer = nullptr;
	ball = nullptr;
	xAxis = 15.0f;
	yAxis = 30.0f;
	IMG_Init(IMG_INIT_PNG);
}

Scene0::~Scene0(){
	IMG_Quit();
}

bool Scene0::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);
	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, -1.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	

	ball = new Entity();
	ball->pos = Vec3(5.0f, 20.0f, 0.0f);
	ball->SetImage("ball.png", renderer);
	return true;
}

void Scene0::OnDestroy() {
	delete ball;
	ball = nullptr;
}

void Scene0::HandleEvents(const SDL_Event& event)
{
	switch (event.type) {
	case SDL_KEYDOWN:
		// Change angle
		if (event.key.keysym.scancode == SDL_SCANCODE_O) {
			ball->angleDeg -= 10.0f;
		}
		if (event.key.keysym.scancode == SDL_SCANCODE_P) {
			ball->angleDeg += 10.0f;
		}
		break;

	default:
		break;
	}
}


void Scene0::Update(const float deltaTime) {
	/// Physics goes here
	Vec3 gravAccel(0.0, -9.8, 0.0);
	Vec3 gravForce = ball->mass * gravAccel;
	ball->ApplyForce(gravForce);
	ball->Update(deltaTime);

}

void Scene0::Render() const {
	SDL_RenderClear(renderer);
	Vec3 screenCoords = projectionMatrix * ball->pos;
	SDL_Rect square;
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = ball->GetSurface()->w;
	square.h = ball->GetSurface()->h;
	SDL_RenderCopyEx(renderer, ball->GetTexture(), nullptr, &square, ball->angleDeg, nullptr, SDL_FLIP_NONE);
	SDL_RenderPresent(renderer);
}