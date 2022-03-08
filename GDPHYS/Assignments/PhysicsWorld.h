#ifndef PHYSICSWORLD
#define PHYSICSWORLD
#include <list>
#include "MyParticle.h"
#include "ForceRegistry.h"
#include "GravityForceGenerator.h"
#include "Collision/ParticleContact.h"
#include "Collision/ContactResolver.h"
#include "Links/ParticleLink.h"
#include <vector>

#include "RigidBodies/CircleRb.h"
#include "RigidBodies/RectPrismRb.h"

class PhysicsWorld
{
public:
	std::list<ParticleLink*> Links;
protected:
	void GenerateContacts();
	void GetOverlaps();
protected:
	ContactResolver resolver = ContactResolver(20);
public:
	std::vector<ParticleContact*> Contacts;
	void AddContact(MyParticle* p1, MyParticle* p2, float restitution, MyVector contactNormal); //, float depth
public:
	std::list<MyParticle*> Particles;
	void addParticle(MyParticle* particle);
	void Update(float time);
protected:
	void UpdateParticeList();
public:
	MyVector gravity = MyVector(0, 0); //this is downward
public:
	PhysicsWorld();
public:
	ForceRegistry forceRegistry;
protected:
	GravityForceGenerator Gravity = GravityForceGenerator(MyVector(0, 0)); //9.8 - negative velocity(downward)
protected:
	void GenerateParticleContacts(MyParticle* a, MyParticle* b);
	void GenerateRigidbodyContacts(MyParticle* a, MyParticle* b);
	void GetContact(RectPrismRb* a, RectPrismRb* b);
	void GetContact(RectPrismRb* a, MyParticle* b);
	void GetContact(MyParticle* a, MyParticle* b);
};

#endif // !PHYSICSWORLD

