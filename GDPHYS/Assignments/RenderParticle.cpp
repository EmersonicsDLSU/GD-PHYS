#include "RenderParticle.h"
#include "MyVector.h"

//calls to draw the shapes depending on the particleType of the particle
void RenderParticle::Draw(sf::RenderWindow* window)
{
	//checks first if the particle is Destroyed
	if (!PhysicParticle->IsDestroyed())
	{
		//gets the renderPoint of the different kinds of particle
		MyVector v = PhysicParticle->GetRenderPoint();
		MyVector c = CircParticle->GetRenderPoint();
		MyVector r = RectParticle->GetRenderPoint();
		//checks the particleType then sets the position
		switch (particleType)
		{
		case 0:
			ParticleShape->setPosition(v.x, v.y);
			break;
		case 2:
			circleShape->setPosition(c.x, c.y);
			break;
		case 3:
			rectShape->setPosition(r.x, r.y);
			break;
		}

		/*MyVector v = PhysicParticle->GetRenderPoint();
		ParticleShape->setPosition(v.x, v.y);*/

		//draw the shape
		//0 = Particle 1 = Rigid 2 = Circ 3 = Rect
		//checks the particleType then draws the shape
		switch (particleType)
		{
		case 0:
			window->draw(*ParticleShape);
			break;
		case 2:
			window->draw(*circleShape);
			break;
		case 3:
			window->draw(*rectShape);
			break;
		}
	}
}

//renderParticle constructor for the normal Particle type
RenderParticle::RenderParticle(MyParticle* p, sf::Shape* s)
{
	this->PhysicParticle = p;
	this->RectParticle = new RectPrismRb();
	this->CircParticle = new CircleRb();

	this->ParticleShape = s;
	this->rectShape = NULL;
	this->circleShape = NULL;

	this->particleType = p->particleType;
}

//renderParticle constructor for the rectangle Particle type
RenderParticle::RenderParticle(RectPrismRb* p, sf::RectangleShape* s)
{
	this->PhysicParticle = new MyParticle();
	this->RectParticle = p;
	this->CircParticle = new CircleRb();

	this->ParticleShape = NULL;
	this->rectShape = s;
	this->circleShape = NULL;

	this->particleType = p->particleType;
}

//renderParticle constructor for the circle Particle type
RenderParticle::RenderParticle(CircleRb* p, sf::CircleShape* s)
{
	this->PhysicParticle = new MyParticle();
	this->RectParticle = new RectPrismRb();
	this->CircParticle = p;

	this->ParticleShape = NULL;
	this->rectShape = NULL;
	this->circleShape = s;

	this->particleType = p->particleType;
}
