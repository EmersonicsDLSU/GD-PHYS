#include "MyParticle.h"


MyParticle::MyParticle(float mass, MyVector position, MyVector velocity, MyVector acceleration, float lifeSpan, bool stationary) :
	mass(mass), position(position), velocity(velocity), acceleration(acceleration), lifeSpan(lifeSpan), stationary(stationary), startPos(position)
{

}
MyParticle::MyParticle()
{

}

void MyParticle::Update(float time)
{
	if (mass == 0) return;
	
	updatePosition(time);
	updateVelocity(time);
	checkLifeSpan(time); 
	ResetForce();
	//exclusive for cable anchors
	if (stationary)
		stationarySetting();
}

void MyParticle::stationarySetting()
{
	this->position.x = startPos.x;
	this->position.y = startPos.y;
	this->velocity.x = 0;
	this->velocity.y = 0;
}

void MyParticle::checkLifeSpan(float time)
{
	lifeSpan -= time;
	if (lifeSpan <= 0.0f) {
		Destroy();
	}
}

void MyParticle::updateVelocity(float time)
{
	acceleration = acceleration + accumulatedForce * (1 / mass);
	
	velocity = velocity + acceleration * time;
	velocity = velocity * powf(dampening, time);

	float MoI = GetMomentOfInertia();
	angularVelocity += -accumulatedTorque * ((float)1 / MoI) * time;
	//angularVelocity += accumulatedTorque * ((float)1 / MoI) * time;
	angularVelocity = angularVelocity * powf(angularDampening, time);
}

void MyParticle::updatePosition(float time)
{
	//P2
	position = position + (velocity * time) + ((acceleration * powf(time, 2)) * (1 / 2));
	rotation = rotation + (angularVelocity * time);
}

MyVector MyParticle::GetRenderPoint()
{
	return Utils::P6ToSFMLPoint(position);
}

void MyParticle::Destroy()
{
	this->isDestroyed = true;
}


void MyParticle::AddForce(MyVector v)
{
	accumulatedForce = accumulatedForce + v;
}

void MyParticle::ResetForce()
{
	accumulatedForce = MyVector(0, 0);
	accumulatedTorque = 0;
	acceleration = MyVector(0, 0);
}

float MyParticle::GetMomentOfInertia()
{
	return((float)2 / 5 * mass * radius * radius);
}

//should only be use in RBs objects
void MyParticle::AddForceOnPoint(MyVector locPoint, MyVector f)
{
	accumulatedForce += f;
	cout << MyVector::getScalarProduct(locPoint, f) << endl;
	accumulatedTorque += MyVector::getScalarProduct(locPoint, f);
}

int MyParticle::GetType()
{
	//0 = Particle 1 = Rigid 2 = Circ 3 = Rect
	return this->particleType;
}

void MyParticle::initializeRb(int particleType)
{

}