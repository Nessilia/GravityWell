#ifndef SCENE1_H
#define SCENE1_H
#include <SDL.h>
#include <Matrix.h>
#include "Scene.h"
#include "Entity.h"

using namespace MATH;
class Scene1 : public Scene {
private:
	SDL_Window *window;
	Matrix4 projectionMatrix;
	Entity* flappy;
	Entity* cliff;
	SDL_Renderer* renderer;
	float xAxis; 
	float yAxis;
public:
	Scene1(SDL_Window* sdlWindow);
	~Scene1();
	bool OnCreate() override;
	void OnDestroy() override;
	void HandleEvents(const SDL_Event& event) override;
	void Update(const float time) override;
	void Render() const override;
};

#endif

