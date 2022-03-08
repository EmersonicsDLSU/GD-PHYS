#pragma once
#include "../MyParticle.h"
#include "../MyVector.h"
class CircleRb : public MyParticle
{
public:
	void initializeRb(int particleType) override;
};

