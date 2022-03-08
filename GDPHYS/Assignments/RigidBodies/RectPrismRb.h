#pragma once
#include "../MyParticle.h"
class RectPrismRb : public MyParticle
{
public:
	void initializeRb(int particleType) override;
	float w;
	float h;
	vector<MyVector> points;
};

