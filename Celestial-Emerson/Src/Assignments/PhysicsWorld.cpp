#include "PhysicsWorld.h"

void PhysicsWorld::AddContact(MyParticle* p1, MyParticle* p2, float restitution, MyVector contactNormal, float depth)
{
	ParticleContact* toAdd = new ParticleContact();

	toAdd->particles[0] = p1;
	toAdd->particles[1] = p2;
	toAdd->restitution = restitution;
	toAdd->collisionNormal = contactNormal;
	toAdd->depth = depth;

	Contacts.push_back(toAdd);
}

void PhysicsWorld::addParticle(MyParticle* particle)
{
	Particles.push_back(particle);
	forceRegistry.Add(particle, &Gravity);
}

void PhysicsWorld::Update(float time)
{
	UpdateParticeList();
	forceRegistry.UpdateForces(time);
	for (std::list<MyParticle*>::iterator i = Particles.begin();
		i != Particles.end(); i++)
	{
		//(*i)->acceleration = gravity;
		(*i)->Update(time);
	}

	GenerateContacts();

	if (Contacts.size() > 0)
	{
		resolver.Max_Iterations = Contacts.size() * 2;
		resolver.ResolveContacts(Contacts, time);
	}
}

void PhysicsWorld::UpdateParticeList()
{
	Particles.remove_if([](MyParticle* p) {return p->IsDestroyed(); });
}

PhysicsWorld::PhysicsWorld()
{

}

void PhysicsWorld::GenerateContacts()
{
	Contacts.clear();
	GetOverlaps();
	for (std::list<ParticleLink*>::iterator i = Links.begin();
		i != Links.end(); i++)
	{
		ParticleContact* contact = (*i)->GetContact();
		if (contact != nullptr)
		{
			Contacts.push_back(contact);
		}
	}
}

void PhysicsWorld::GetOverlaps()
{
	for (int i = 0; i < Particles.size() - 1; i++)
	{
		std::list<MyParticle*>::iterator a = std::next(Particles.begin(), i);
		for (int h = i + 1; h < Particles.size(); h++)
		{
			std::list<MyParticle*>::iterator b = std::next(Particles.begin(), h);

			float distSq = ((*a)->position - (*b)->position).SquareMagnitude();
			float radSumSq = ((*a)->radius + (*b)->radius) * ((*a)->radius + (*b)->radius);

			if (distSq <= radSumSq)
			{
				//Collision Normal
				MyVector dir = ((*a)->position - (*b)->position);
				dir.Normalize();

				float r = radSumSq - distSq;
				float depth = sqrt(r);
				//1
				float restitution = (*a)->restitution;
				//0.5
				if ((*b)->restitution < restitution)
					restitution = (*b)->restitution;

				AddContact(*a, *b, restitution, dir, depth);
			}
		}
	}
}