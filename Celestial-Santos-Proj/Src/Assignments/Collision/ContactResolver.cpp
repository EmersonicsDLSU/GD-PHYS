#include "ContactResolver.h"

void ContactResolver::ResolveContacts(std::vector<ParticleContact*> contacts, float time)
{
	//threshold for the number of resolve per frame
	for (unsigned i = Curr_Iteration; i < Max_Iterations; i++)
	{
		//ranks the contacts from lowest to highest SeparatingSpeed
		for (unsigned x = 1; x < contacts.size(); x++)
		{
			//sort
			if (contacts[0]->GetSeparatingSpeed() > contacts[x]->GetSeparatingSpeed() || contacts[x]->depth > 0)
			{
				ParticleContact* temp = contacts[0];
				contacts[0] = contacts[x];
				contacts[x] = temp;
			}
		}
		//resolve the current contact; resolves the contact with the least SS
		contacts[0]->Resolve(time);
		//checks if all contact's SS are >= 0
		bool isAllPosi = true;
		for (unsigned y = 0; y < contacts.size(); y++)
		{
			if (contacts[y]->GetSeparatingSpeed() < 0 || contacts[y]->depth > 0)
			{
				isAllPosi = false;
			}
		}
		//if (isAllPosi || contacts.size() == 1)
		if (isAllPosi)
		{
			return;
		}
	}
}