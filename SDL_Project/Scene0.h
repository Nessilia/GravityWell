#ifndef SCENE0_H
#define SCENE0_H
#include <SDL.h>
#include <Matrix.h>
#include "Scene.h"
#include "Entity.h"

using namespace MATH;
class Scene0 : public Scene {
private:
	SDL_Window *window;
	Matrix4 projectionMatrix;
	Entity* ball;
	SDL_Renderer* renderer;
	float xAxis; 
	float yAxis;
public:
	Scene0(SDL_Window* sdlWindow);
	~Scene0();
	bool OnCreate() override;
	void OnDestroy() override;
	void HandleEvents(const SDL_Event& event) override;
	void Update(const float time) override;
	void Render() const override;
};

#endif

