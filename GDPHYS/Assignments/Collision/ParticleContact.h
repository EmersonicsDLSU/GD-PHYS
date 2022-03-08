#pragma once
#include "../MyParticle.h"

class ParticleContact
{
public:
	//Measure of how much they've overlapped
	float depth = 0;
public:
	float GetSeparatingSpeed();
	void ResolveVelocity(float time);
	void ResolveInterpenetration(float time);

public:
	MyParticle* particles[2];
	float restitution;
	MyVector collisionNormal;

public:
	void Resolve(float time);
};

