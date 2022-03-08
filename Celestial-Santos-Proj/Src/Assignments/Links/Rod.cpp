#include "Rod.h"

ParticleContact* Rod::GetContact()
{
	float curr_len = GetDistance();

	if (curr_len == length)
	{
		return nullptr;
	}

	ParticleContact* ret = new ParticleContact();
	ret->particles[0] = particles[0];
	ret->particles[1] = particles[1];

	MyVector dir = particles[1]->position - particles[0]->position;
	dir.Normalize();

	if (curr_len > length)
	{
		ret->collisionNormal = dir;
		ret->depth = curr_len - length;
	}
	else if (curr_len < length)
	{
		ret->collisionNormal = dir * -1;
		ret->depth = length - curr_len;
	}

	ret->restitution = restitution;

	return ret;
}
