
#include <SDL.h>
#include <Matrix.h>
#include "Scene.h"
#include "Entity.h"

using namespace MATH;
class gravityWellScene : public Scene {
private:
	SDL_Window *window;
	Matrix4 projectionMatrix;
	Entity* planet;
	Entity* star1;
	Entity* star2;
	Entity* backGround;
	SDL_Renderer* renderer;
	float xAxis; 
	float yAxis;
public:
	gravityWellScene(SDL_Window* sdlWindow);
	~gravityWellScene();
	bool OnCreate() override;
	void OnDestroy() override;
	void HandleEvents(const SDL_Event& event) override;
	void Update(const float time) override;
	void Render() const override;
	
};


