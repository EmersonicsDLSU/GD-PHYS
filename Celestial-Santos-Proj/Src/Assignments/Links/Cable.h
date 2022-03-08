#pragma once
#include "ParticleLink.h"

class Cable : public ParticleLink
{
public:
	float length;
	float restitution = 0;

	ParticleContact* GetContact() override;
};

