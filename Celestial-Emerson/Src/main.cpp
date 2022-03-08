//Created by Emerson Paul P. Celestial

#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <iterator>
#include "Assignments/MyParticle.h"
#include "Assignments/MyVector.h"
#include "Assignments/Utils.h"
#include "Assignments/PhysicsWorld.h"
#include "Assignments/RenderParticle.h"
#include "Assignments/DragForceGenerator.h"
#include "Assignments/Springs/AnchoredSpring.h"
#include "Assignments/Springs/ParticleSpring.h"
#include "Assignments/Springs/BungeeSpring.h"
#include "Assignments/Springs/CableSpring.h"
#include "Assignments/Collision/ParticleContact.h"
#include "Assignments/Collision/ContactResolver.h"
#include "Assignments/Links/Rod.h"
#include "Assignments/Links/Cable.h"
#define PI 3.14159265

using namespace std::chrono_literals;
using namespace std::chrono;

//16ms = 1 frame in a 60fps
constexpr nanoseconds timestep(16ms);

//function declarations
void instantiateParticles(std::list<RenderParticle*>& RenderParticles, PhysicsWorld& pWorld,
    float mass, MyVector position, MyVector velocity, MyVector acceleration, float lifeSpan, DragForceGenerator& df,
    vector <MyParticle*>& particleList, vector <sf::CircleShape*>& shapeList);
float RandomFloat(float a, float b);


int main() {
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(1080, 720), "P6 Test", sf::Style::Default);

    //PhysicsWorld, Renderparticle, and Utils instance
    PhysicsWorld pWorld = PhysicsWorld();
    std::list<RenderParticle*> RenderParticles;
    Utils::offset = MyVector(0, 500);
    DragForceGenerator df = DragForceGenerator(0, 0); //setted to zero(0); kinetic friction
    //storage of the particles and shapes object
    vector <MyParticle*> particleList;
    vector <sf::CircleShape*> shapeList;

    //rotation lesson
    //bearing
    MyParticle* originPoint = new MyParticle(50.0f, MyVector(500, 260), MyVector(0, 0), MyVector(0, 0), 100, true);
    originPoint->dampening = 1;
    originPoint->restitution = 0.6;
    originPoint->radius = 20;
    particleList.push_back(originPoint);
    pWorld.addParticle(originPoint);

    //originPoint->AddForce(MyVector(200000, 200000));

    //weights
    MyParticle* referencePoint1 = new MyParticle(0.10f, MyVector(500, 360), MyVector(0, 0), MyVector(0, 0), 100, false);

    //adds Force to the top-weight
    referencePoint1->AddForce(MyVector(20000, 0));

    referencePoint1->dampening = 0.8;
    referencePoint1->restitution = 0.6;
    referencePoint1->radius = 15;
    particleList.push_back(referencePoint1);
    pWorld.addParticle(referencePoint1);

    MyParticle* referencePoint2 = new MyParticle(0.10f, MyVector(500, 160), MyVector(0, 0), MyVector(0, 0), 100, false);
    referencePoint2->dampening = 0.8;
    referencePoint2->restitution = 0.6;
    referencePoint2->radius = 15;
    particleList.push_back(referencePoint2);
    pWorld.addParticle(referencePoint2);

    MyParticle* referencePoint3 = new MyParticle(0.10f, MyVector(600, 260), MyVector(0, 0), MyVector(0, 0), 100, false);
    referencePoint3->dampening = 0.8;
    referencePoint3->restitution = 0.6;
    referencePoint3->radius = 15;
    particleList.push_back(referencePoint3);
    pWorld.addParticle(referencePoint3);

    MyParticle* referencePoint4 = new MyParticle(0.10f, MyVector(400, 260), MyVector(0, 0), MyVector(0, 0), 100, false);
    referencePoint4->dampening = 0.8;
    referencePoint4->restitution = 0.6;
    referencePoint4->radius = 15;
    particleList.push_back(referencePoint4);
    pWorld.addParticle(referencePoint4);

    //links
    Rod* r5 = new Rod();
    r5->particles[0] = referencePoint1;
    r5->particles[1] = referencePoint3;
    r5->length = referencePoint1->position.getMagnitude(referencePoint3->position);
    pWorld.Links.push_back(r5);

    Rod* r6 = new Rod();
    r6->particles[0] = referencePoint3;
    r6->particles[1] = referencePoint2;
    r6->length = referencePoint3->position.getMagnitude(referencePoint2->position);
    pWorld.Links.push_back(r6);

    Rod* r7 = new Rod();
    r7->particles[0] = referencePoint1;
    r7->particles[1] = referencePoint4;
    r7->length = referencePoint1->position.getMagnitude(referencePoint4->position);
    pWorld.Links.push_back(r7);
     
    /*Rod* r8 = new Rod();
    r8->particles[0] = referencePoint4;
    r8->particles[1] = referencePoint1;
    r8->length = referencePoint4->position.getMagnitude(referencePoint1->position);
    pWorld.Links.push_back(r8);*/

    Rod* r1 = new Rod();
    r1->particles[0] = originPoint;
    r1->particles[1] = referencePoint1;
    r1->length = originPoint->position.getMagnitude(referencePoint1->position);
    pWorld.Links.push_back(r1);

    Rod* r3 = new Rod();
    r3->particles[0] = originPoint;
    r3->particles[1] = referencePoint3;
    r3->length = 100;
    pWorld.Links.push_back(r3);

    Rod* r2 = new Rod();
    r2->particles[0] = originPoint;
    r2->particles[1] = referencePoint2;
    r2->length = 100;
    pWorld.Links.push_back(r2);

    Rod* r4 = new Rod();
    r4->particles[0] = originPoint;
    r4->particles[1] = referencePoint4;
    r4->length = 100;
    pWorld.Links.push_back(r4);

    //texture
    sf::Texture t;
    if (!t.loadFromFile("shuriken.png"))  std::cout << "Fail snowman loading" << std::endl;

    sf::Sprite shuriken;
    shuriken.setTexture(t);
    shuriken.setOrigin(t.getSize().x / 2, t.getSize().y / 2);
    MyVector shurikenPos = Utils::P6ToSFMLPoint(originPoint->position);

    /*MyVector weightPos1 = Utils::P6ToSFMLPoint(referencePoint1->position);
    MyVector weightPos2 = Utils::P6ToSFMLPoint(referencePoint2->position);
    MyVector weightPos3 = Utils::P6ToSFMLPoint(referencePoint3->position);
    MyVector weightPos4 = Utils::P6ToSFMLPoint(referencePoint4->position);*/

    //base shape
    sf::CircleShape shape(originPoint->radius);
    shape.setFillColor(sf::Color::White);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shapeList.push_back(&shape);

    RenderParticle rP1 = RenderParticle(originPoint, &shape);
    RenderParticles.push_back(&rP1);

    //weightShapes
    sf::CircleShape wShape1(referencePoint1->radius);
    wShape1.setFillColor(sf::Color::Blue);
    wShape1.setOrigin(wShape1.getRadius(), wShape1.getRadius());
    shapeList.push_back(&wShape1);
    RenderParticle rP2 = RenderParticle(referencePoint1, &wShape1);
    RenderParticles.push_back(&rP2);

    sf::CircleShape wShape2(referencePoint2->radius);
    wShape2.setFillColor(sf::Color::Blue);
    wShape2.setOrigin(wShape2.getRadius(), wShape2.getRadius());
    shapeList.push_back(&wShape2);
    RenderParticle rP3 = RenderParticle(referencePoint2, &wShape2);
    RenderParticles.push_back(&rP3);

    sf::CircleShape wShape3(referencePoint3->radius);
    wShape3.setFillColor(sf::Color::Blue);
    wShape3.setOrigin(wShape3.getRadius(), wShape3.getRadius());
    shapeList.push_back(&wShape3);
    RenderParticle rP4 = RenderParticle(referencePoint3, &wShape3);
    RenderParticles.push_back(&rP4);

    sf::CircleShape wShape4(referencePoint4->radius);
    wShape4.setFillColor(sf::Color::Blue);
    wShape4.setOrigin(wShape4.getRadius(), wShape4.getRadius());
    shapeList.push_back(&wShape4);
    RenderParticle rP5 = RenderParticle(referencePoint4, &wShape4);
    RenderParticles.push_back(&rP5);

    MyVector renderPoint = originPoint->GetRenderPoint();
    shape.setPosition(renderPoint.x, renderPoint.y);

    renderPoint = referencePoint1->GetRenderPoint();
    wShape1.setPosition(renderPoint.x, renderPoint.y);
    //end of rotation lesson

    pWorld.forceRegistry.Add(particleList[0], &df);    //adds the friction
    pWorld.forceRegistry.Add(particleList[1], &df);    //adds the friction

    //custom variables
    int standing = 0;

    //clock/frame 
    using clock = high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    nanoseconds curr_ns(0ns);

    //particle effects properties
    int particleLimitSize = 25;

    sf::Event event;

    MyVector startingPos = referencePoint1->position;
    float timer = 0.0f;
    bool stopTimer = false;
    float revolution = 0.0f;
    bool once = true;
    float startingRad = 0.0f;
    int tempDeg;

    while (1) {
        curr_time = clock::now();
        auto dur = duration_cast<nanoseconds> (curr_time - prev_time);
        prev_time = curr_time;

        curr_ns += dur;
        if (curr_ns >= timestep) {

            auto ms = duration_cast<milliseconds>(curr_ns);

            //Call the update when it reaches a cetain timestep
            //ms is in milisecs while engine is using secs so we divide by 1000
            //(float)ms.count() / 1000

            //Rotation lesson
            //MyVector prevPos = referencePoint1->position - originPoint->position;
            pWorld.Update((float)ms.count() / 1000);
            //MyVector currPos = referencePoint1->position - originPoint->position;
            //end of Rotation lesson

            //
            if(!stopTimer)
                timer += (float)ms.count() / 1000;

            curr_ns -= timestep;

            while (window.pollEvent(event))
            {
                switch (event.type)
                {
                case sf::Event::Closed:
                    window.close();
                    break;
                }
            }

            window.clear();
            //Rotation lesson
            shurikenPos = Utils::P6ToSFMLPoint(originPoint->position);
            shuriken.setPosition(shurikenPos.x, shurikenPos.y);

            /*float p1 = (prevPos * currPos);
            float p2 = (originPoint->position - referencePoint1->position).SquareMagnitude();
            float rad_Angle = acosf(p1 / p2);
            float deg = rad_Angle * (180 / acosf(-1.0));
            cout << "p1: " << p1 << endl;
            cout << "p2: " << p2 << endl;
            cout << "rad_Angle: " << deg << endl;
            cout << "Degrees: " << deg << endl;
            if(deg >= 0)
            shuriken.rotate(-deg);*/

            float rad_Angle = atan2f(referencePoint1->position.y - originPoint->position.y, referencePoint1->position.x - originPoint->position.x);
            float deg = rad_Angle * (180 / acos(-1.0));
            tempDeg = (int)deg;
            if ((int)startingRad == tempDeg)
            {
                revolution++;
            }
            if (once)
            {
                startingRad = tempDeg;
                once = !once;
            }
            if (referencePoint1->velocity.x <= 10 && referencePoint2->velocity.x <= 10 && referencePoint3->velocity.x <= 10 && referencePoint4->velocity.x <= 10 && !stopTimer)
            {
                revolution += (((180.0f - deg) / 90.0f) * 0.25f) + 0.75f;
                cout << "Spinner has completed: " << revolution << " revolutions" << endl;
                cout << "Spinner took " << timer << " secs to reach minimum speed" << endl;
                stopTimer = true;
            }
            shuriken.setRotation(-deg);

            window.draw(shuriken);
            //end of Rotation lesson
            //iterates the particles then draw
            for (std::list<RenderParticle*>::iterator i = RenderParticles.begin();
                i != RenderParticles.end(); i++)
            {
                (*i)->Draw(&window);
            }
            window.display();
        }
    }
    std::system("pause");
    return 0;
}

//instantiate a new particle
void instantiateParticles(std::list<RenderParticle*>& RenderParticles, PhysicsWorld& pWorld,
    float mass, MyVector position, MyVector velocity, MyVector acceleration, float lifeSpan, DragForceGenerator& df,
    vector <MyParticle*>& particleList, vector <sf::CircleShape*>& shapeList)
{
    MyParticle* myP = new MyParticle(mass, position, velocity, acceleration, lifeSpan);
    //add particle to the PhysicWorld
    pWorld.addParticle(myP);
    sf::CircleShape* myS = new sf::CircleShape(myP->radius);

    static int particleIndex = 0;

    myS->setFillColor(sf::Color::White);
    myS->setOrigin(myS->getRadius(), myS->getRadius());

    RenderParticle* myRp = new RenderParticle(myP, myS);
    RenderParticles.push_back(myRp);
    particleList.push_back(myP);
    shapeList.push_back(myS);

    particleIndex++;
}

//returns a random float between the two float values
float RandomFloat(float a, float b)
{
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

