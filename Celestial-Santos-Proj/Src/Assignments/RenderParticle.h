#ifndef RENDERPARTICLE
#define RENDERPARTICLE
#include "MyParticle.h"
#include <SFML/Graphics.hpp>

class RenderParticle
{
public:
	MyParticle* PhysicParticle;
	sf::Shape* ParticleShape;

	//Constructor
	RenderParticle(MyParticle* p, sf::Shape* s) : PhysicParticle(p), ParticleShape(s) {}
	void Draw(sf::RenderWindow* window);
};


#endif //!RENDERPARTICLE

