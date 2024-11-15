#include "Entity.h"

Entity::Entity(): // this is an initializer list
	surface(nullptr), 
	texture(nullptr), 
	angleDeg(0.0f), 
	mass(1.0f),
	r(1.0f)
{
}

Entity::~Entity() {
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	surface = nullptr;
	texture = nullptr;
}


void Entity::SetImage(const char* filename, SDL_Renderer* renderer) {
	surface = IMG_Load(filename);
	if (surface == nullptr) {
		std::cerr << "Did you spell the file right?" << std::endl;
	}
	texture = SDL_CreateTextureFromSurface(renderer, surface);
}

void Entity::ApplyForce(Vec3 force)
{
	// Code this up like assignment 1
	acc = force * (1.0f / mass);
}

void Entity::Update(float deltaTime)
{

	pos = pos + (vel * deltaTime) + (acc * (0.5f * deltaTime * deltaTime));
	vel = vel + (acc * deltaTime);
	// Code this up like assignment 1
}
