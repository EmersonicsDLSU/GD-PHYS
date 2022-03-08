#ifndef MYPARTICLE
#define MYPARTICLE
#include "MyVector.h"
#include "Utils.h"
#include <math.h>
#include <iostream>
#include <SFML/Graphics.hpp>

class MyParticle
{
public: //consrtuctor
	MyParticle(float mass, MyVector position, MyVector velocity, MyVector acceleration, float lifeSpan, bool stationary = false);
	MyParticle();
public:
	float mass;
	float lifeSpan = 100.0f;
	MyVector position;
	MyVector velocity;
	MyVector acceleration;
protected:
	void updatePosition(float time);
	void updateVelocity(float time);
	bool isDestroyed = false;
public:
	void Update(float time);
	MyVector GetRenderPoint();
	bool IsDestroyed() {
		return isDestroyed;
	}
	void Destroy();
	void checkLifeSpan(float time);
public:
	float dampening = 0.9f;
	void AddForce(MyVector v);
	void ResetForce();
protected:
	MyVector accumulatedForce = MyVector(0,0,0);
public:
	float radius = 0;
	//elasticity/restitution 1-being so elastic and 0 - no elasticity(bounciness)
	float restitution = 1;
//Torque
public:
	float angularDampening = 0.9;
	float angularVelocity = 0;

	float rotation = 0; //Radians
public:
	float GetMomentOfInertia();
	void AddForceOnPoint(MyVector locPoint, MyVector force);
protected:
	float accumulatedTorque = 0;
public:
	bool stationary = false;
	MyVector startPos;
	void stationarySetting();
public:
	int particleType = 0; //0 = Particle 1 = Rigid 2 = Circ 3 = Rect
	int GetType();
	virtual void initializeRb(int particleType);
};

#endif //MYPARTICLE


