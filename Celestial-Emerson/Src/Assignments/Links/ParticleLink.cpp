#include "ParticleLink.h"

float ParticleLink::GetDistance()
{
	MyVector dist = particles[0]->position - particles[1]->position;
	return dist.getMagnitude();
}

float ParticleLink::GetDistance(MyVector pos1, MyVector pos2)
{
	MyVector dist = pos1 - pos2;
	return dist.getMagnitude();
}