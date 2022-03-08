#ifndef RENDERPARTICLE
#define RENDERPARTICLE
#include "MyParticle.h"
#include "RigidBodies/RectPrismRb.h"
#include "RigidBodies/CircleRb.h"
#include <SFML/Graphics.hpp>

class RenderParticle
{
public:
	MyParticle* PhysicParticle;
	RectPrismRb* RectParticle;
	CircleRb* CircParticle;
	int particleType;
	sf::Shape* ParticleShape;
	sf::RectangleShape* rectShape;
	sf::CircleShape* circleShape;

	//Constructor
	RenderParticle(MyParticle* p, sf::Shape* s);
	RenderParticle(RectPrismRb* p, sf::RectangleShape* s);
	RenderParticle(CircleRb* p, sf::CircleShape* s);
	void Draw(sf::RenderWindow* window);
};


#endif //!RENDERPARTICLE

