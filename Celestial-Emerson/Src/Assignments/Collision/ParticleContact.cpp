#include "ParticleContact.h"
#include <iostream>
void ParticleContact::Resolve(float time)
{
	//Call Resolve Velocity for now
	ResolveVelocity(time);
	ResolveInterpenetration(time);
}

float ParticleContact::GetSeparatingSpeed()
{
	MyVector velocityDiff = particles[0]->velocity;
	if (particles[1])
	{
		velocityDiff -= particles[1]->velocity;
	}
	return velocityDiff * collisionNormal;
}

void ParticleContact::ResolveVelocity(float time)
{
	// Ss
	float Ss = GetSeparatingSpeed();
	//If the object's already moving away from each other
	//Don't do anything
	if (Ss > 0)
	{
		return;
	}

	//separating speed after collision
	float Sf = -restitution * Ss;
	//Magnitude of Delta Velocity
	float deltaSpeed = Sf - Ss;

	//total Momentum
	float totalMass = (float)1 / particles[0]->mass;
	if (particles[1]) totalMass += (float)1 / particles[1]->mass;

	//Invalid collision if total mass is 0 or less
	if (totalMass <= 0) return;

	//	i	=	dV	*	m; //divide since the mass is 1/m
	float impulse_mag = deltaSpeed / totalMass;

	//Get the impulse vector using the contact normal
	MyVector Impulse = collisionNormal * impulse_mag;

	//	I / m
	MyVector V_0 = Impulse * ((float)1 / particles[0]->mass);
	//			Vf = Vi + I/m
	particles[0]->velocity = particles[0]->velocity + V_0;

	if (particles[1])
	{
		MyVector V_1 = Impulse * (-(float)1 / particles[1]->mass);
		particles[1]->velocity = particles[1]->velocity + V_1;
	}
}

void ParticleContact::ResolveInterpenetration(float time)
{
	//Check if the two overlapped
	if (depth <= 0) {
		return;
	}
	float totalMass = (float)1 / particles[0]->mass;
	if (particles[1]) totalMass += (float)1 / particles[1]->mass;

	if (totalMass <= 0) return;

	float depthMagnitude = depth / totalMass;
	MyVector depthVector = collisionNormal * depthMagnitude;

	MyVector a_Pos = depthVector * ((float)1 / particles[0]->mass);
	particles[0]->position += a_Pos;

	if (particles[1])
	{
		MyVector b_Pos = depthVector * (-(float)1 / particles[1]->mass);
		particles[1]->position += b_Pos;
	}

	depth = 0;
}