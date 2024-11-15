#ifndef ENTITY_H
#define ENTITY_H

#include <Vector.h>
#include <SDL_image.h>
#include <SDL.h>
#include <array>
using namespace MATH;

class Entity {
private:
	// Keep these private as we should only build them in the setImage method
	SDL_Surface* surface = nullptr; // Used to get the width and height of the image
	SDL_Texture* texture = nullptr; // Used to render the image

public:
	float angleDeg;
	Vec3 pos;
	Vec3 vel;
	Vec3 acc;
	float mass;
	// Let's pretend all entitys are circles
	float r;
	// Will render 360 points to make a circle
	// put it in a std::array so it knows it's size
	std::array<Vec3, 360> points;
	// Patryck says we could make an array like this
	// Vec3 points[360];

	Entity();
	~Entity();
	void SetImage(const char* filename, SDL_Renderer* renderer);

	// Just like assignment 1!
	void ApplyForce(Vec3 force);
	void Update(float deltaTime);


	// Need getters for private member variables
	SDL_Surface* GetSurface() const { return surface; }
	SDL_Texture* GetTexture() const { return texture; }


};

#endif
