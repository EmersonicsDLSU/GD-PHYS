#pragma once
#include <vector>
#include "ParticleContact.h"


class ContactResolver
{
protected:
	unsigned Curr_Iteration = 0;

public:
	//Maximum number of times; Resolve function get called
	unsigned Max_Iterations;
	ContactResolver(unsigned _max_iter) : Max_Iterations(_max_iter){}
	void ResolveContacts(std::vector<ParticleContact*> contacts, float time);
};

