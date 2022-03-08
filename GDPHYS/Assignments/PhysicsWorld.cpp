#include "PhysicsWorld.h"

void PhysicsWorld::AddContact(MyParticle* p1, MyParticle* p2, float restitution, MyVector contactNormal) //, float depth)
{
	ParticleContact* toAdd = new ParticleContact();

	toAdd->particles[0] = p1;
	toAdd->particles[1] = p2;
	toAdd->restitution = restitution;
	toAdd->collisionNormal = contactNormal;
	//toAdd->depth = depth;

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
			//0 = Particle 1 = Rigid 2 = Circ 3 = Rect
			
			if((*a)->GetType() == 0 && (*b)->GetType() == 0)
			{
				GenerateParticleContacts(*a, *b);
			}
			else{
				GenerateRigidbodyContacts(*a, *b);
			}
			
		}
	}
}

void PhysicsWorld::GenerateParticleContacts(MyParticle* a, MyParticle* b)
{
	float distSq = (a->position - b->position).SquareMagnitude();
	float radSumSq = (a->radius + b->radius) * (a->radius + b->radius);

	if (distSq <= radSumSq)
	{
		//Collision Normal
		MyVector dir = (a->position - b->position);
		dir.Normalize();

		float r = radSumSq - distSq;
		float depth = sqrt(r);
		//1
		float restitution = a->restitution;
		//0.5
		if (b->restitution < restitution)
			restitution = b->restitution;

		AddContact(a, b, restitution, dir);
	}
}

void PhysicsWorld::GenerateRigidbodyContacts(MyParticle* a, MyParticle* b)
{
	if(
	(a->GetType() == 2 && b->GetType() == 2) || 
	(a->GetType() == 2 && b->GetType() == 0) || 
	(a->GetType() == 0 && b->GetType() == 2)
	)
	{
		GenerateParticleContacts(a,b);
	}

	else if(a->GetType() == 3 && b->GetType() == 3)
	{
		RectPrismRb* rect1 = dynamic_cast<RectPrismRb*>(a);
		RectPrismRb* rect2 = dynamic_cast<RectPrismRb*>(b);

		GetContact(rect1, rect2);
	}
	
	else
	{
		RectPrismRb* rect = dynamic_cast<RectPrismRb*>(a);
		if(rect == nullptr)
		{
			rect = dynamic_cast<RectPrismRb*>(b);
			GetContact(rect, a);
		}
		else
		{
			GetContact(rect, b);
		}
	}
			
}

void PhysicsWorld::GetContact(RectPrismRb* a, MyParticle* b)
{
	MyVector relV = b->position - a->position;
	float invRot = -a->rotation;

	MyVector rotV = Utils::RotatePoint(relV, invRot);

	float minX = rotV.x;
	if ((a->w / 2) < minX)
	{
		minX = a->w / 2;
	}

	float maxX = minX;
	if (maxX < -(a->w / 2)) //maxX
	{
		maxX = -(a->w / 2);
	}

	float minY = rotV.y;
	if ((a->h / 2) < minY)
	{
		minY = a->h / 2;
	}

	float maxY = minY;
	if (maxY < -(a->h / 2))
	{
		maxY = -(a->h / 2);
	}

	float D_X = rotV.x - maxX;
	float D_Y = rotV.y - maxY;

	bool col = (D_X * D_X + D_Y * D_Y) <= (b->radius * b->radius);

	if (col)
	{
		//Collision Normal
		MyVector dir = relV * -1;
		dir.Normalize();

		//1
		float restitution = a->restitution;
		//0.5
		if (b->restitution < a->restitution)
			restitution = b->restitution;

		AddContact(a, b, restitution, dir);
	}
}

void PhysicsWorld::GetContact(RectPrismRb* a, RectPrismRb* b)
{
	MyVector temp = MyVector(0, 0);

	std::vector<RectPrismRb*> rects;
	rects.push_back(a);
	rects.push_back(b);

	bool ret = true;
	for (int i = 0; i < rects.size(); i++)
	{
		for (int e1 = 0; e1 < rects[i]->points.size(); e1++)
		{
			int e2 = (e1 + 1);
			e2 %= rects[i]->points.size();

			MyVector p1 = rects[i]->points[e1];
			MyVector p2 = rects[i]->points[e2];

			MyVector perpendiculardge = MyVector(p2.y - p1.y, p1.x - p2.x);

			float minA = temp.getScalarProduct(perpendiculardge , rects[0]->points[0]);
			float maxA = temp.getScalarProduct(perpendiculardge , rects[0]->points[0]);

			for (int h = 1; h < rects[0]->points.size(); h++)
			{
				float proj = temp.getScalarProduct(perpendiculardge , rects[0]->points[h]);
				if (proj < minA)
				{
					minA = proj;
				}
				if (proj > maxA)
				{
					maxA = proj;
				}
			}

			float minB = temp.getScalarProduct(perpendiculardge , rects[1]->points[0]);
			float maxB = temp.getScalarProduct(perpendiculardge , rects[1]->points[0]);
			for (int h = 1; h < rects[1]->points.size(); h++)
			{
				float proj = temp.getScalarProduct(perpendiculardge , rects[1]->points[h]);
				if (proj < minB)
				{
					minB = proj;
				}
				if (proj > maxB)
				{
					maxB = proj;
				}
			}

			if (maxA < minB || maxB < minA)
			{
				ret = false;
				break;
			}
		}
	}

	if (ret)
	{
		//Collision Normal
		MyVector dir = (a->position - b->position);
		dir.Normalize();
		//1
		float restitution = a->restitution;
		//0.5
		if (b->restitution < a->restitution)
			restitution = b->restitution;

		cout << "COLLIDE" << endl;
		AddContact(a, b, restitution, dir);
	}
}
