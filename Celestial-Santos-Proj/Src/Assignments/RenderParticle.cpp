#include "RenderParticle.h"

void RenderParticle::Draw(sf::RenderWindow* window)
{
	if (!PhysicParticle->IsDestroyed())
	{
		MyVector v = PhysicParticle->GetRenderPoint();
		ParticleShape->setPosition(v.x, v.y);

		//draw the shape
		window->draw(*ParticleShape);
	}
}
