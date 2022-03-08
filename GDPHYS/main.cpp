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
#include "Assignments/RigidBodies/CircleRb.h"
#include "Assignments/RigidBodies/RectPrismRb.h"
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
void instantiateRB(PhysicsWorld& pWorld, vector <MyParticle*>& particleList, vector <sf::CircleShape*>& shapeList, CircleRb* circle, sf::CircleShape* circleShape, std::list<RenderParticle*>& RenderParticles,
    MyVector position, MyVector circleUtilPos, MyVector locPoint, MyVector force, float radius);

//0 = Particle 1 = Rigid 2 = Circ 3 = Rect
enum particleType { Particle = 0, Rigid = 1, Circ = 2, Rect = 3 }; //enum type for particleType

int main() {
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(1080, 720), "P6 Test", sf::Style::Default);

    //PhysicsWorld, Renderparticle, and Utils instance
    PhysicsWorld pWorld = PhysicsWorld();
    std::list<RenderParticle*> RenderParticles;
    Utils::offset = MyVector(0, 250);
    DragForceGenerator df = DragForceGenerator(0, 0); //setted to zero(0); kinetic friction

    //storage of the particles and shapes object
    vector <MyParticle*> particleList;
    vector <sf::CircleShape*> shapeList;
    vector <sf::RectangleShape*> rectList;

    //week 15 lesson
    //instantiation of the circleObjects with rigidBodies
    //circle1
    CircleRb* circleParticle1 = new CircleRb();
    circleParticle1->initializeRb((int)particleType::Circ);
    circleParticle1->mass = 5.0f;
    circleParticle1->startPos = MyVector(400, 100);
    circleParticle1->position = MyVector(400, 100);
    circleParticle1->velocity = MyVector(0, 0);
    circleParticle1->acceleration = MyVector(0, 0);
    circleParticle1->lifeSpan = 1000.0f;
    circleParticle1->stationary = false;
    circleParticle1->dampening = 0.9;
    circleParticle1->restitution = 0.6;
    circleParticle1->radius = 20;
    particleList.push_back(circleParticle1);
    pWorld.addParticle(circleParticle1);

    sf::CircleShape circleShape1(circleParticle1->radius);
    circleShape1.setFillColor(sf::Color::Red);
    circleShape1.setOrigin(circleShape1.getRadius(), circleShape1.getRadius());
    shapeList.push_back(&circleShape1);
    MyVector circlePos1 = Utils::P6ToSFMLPoint(MyVector(400, 100));
    circleShape1.setPosition(circlePos1.x, circlePos1.y);

    RenderParticle c_rPC1 = RenderParticle(circleParticle1, &circleShape1);
    RenderParticles.push_back(&c_rPC1);
    //adds a force to the object directing to a specific local point location
    circleParticle1->AddForceOnPoint(MyVector(0, 10), MyVector(10000, 0));

    //circle2
    CircleRb* circleParticle2 = new CircleRb();
    circleParticle2->initializeRb((int)particleType::Circ);
    circleParticle2->mass = 5.0f;
    circleParticle2->startPos = MyVector(700, 100);
    circleParticle2->position = MyVector(700, 100);
    circleParticle2->velocity = MyVector(0, 0);
    circleParticle2->acceleration = MyVector(0, 0);
    circleParticle2->lifeSpan = 1000.0f;
    circleParticle2->stationary = false;
    circleParticle2->dampening = 0.9;
    circleParticle2->restitution = 0.6;
    circleParticle2->radius = 20;
    particleList.push_back(circleParticle2);
    pWorld.addParticle(circleParticle2);

    sf::CircleShape circleShape2(circleParticle2->radius);
    circleShape2.setFillColor(sf::Color::Red);
    circleShape2.setOrigin(circleShape2.getRadius(), circleShape2.getRadius());
    shapeList.push_back(&circleShape2);
    MyVector circlePos2 = Utils::P6ToSFMLPoint(MyVector(700, 100));
    circleShape2.setPosition(circlePos2.x, circlePos2.y);

    RenderParticle c_rPC2 = RenderParticle(circleParticle2, &circleShape2);
    RenderParticles.push_back(&c_rPC2);
    //adds a force to the object directing to a specific local point location
    circleParticle2->AddForceOnPoint(MyVector(0, 10), MyVector(-10000, 0));

    //circle3
    CircleRb* circleParticle3 = new CircleRb();
    circleParticle3->initializeRb((int)particleType::Circ);
    circleParticle3->mass = 5.0f;
    circleParticle3->startPos = MyVector(700, -100);
    circleParticle3->position = MyVector(700, -100);
    circleParticle3->velocity = MyVector(0, 0);
    circleParticle3->acceleration = MyVector(0, 0);
    circleParticle3->lifeSpan = 1000.0f;
    circleParticle3->stationary = false;
    circleParticle3->dampening = 0.9;
    circleParticle3->restitution = 0.6;
    circleParticle3->radius = 20;
    particleList.push_back(circleParticle3);
    pWorld.addParticle(circleParticle3);

    sf::CircleShape circleShape3(circleParticle3->radius);
    circleShape3.setFillColor(sf::Color::Red);
    circleShape3.setOrigin(circleShape3.getRadius(), circleShape3.getRadius());
    shapeList.push_back(&circleShape3);
    MyVector circlePos3 = Utils::P6ToSFMLPoint(MyVector(700, -100));
    circleShape3.setPosition(circlePos3.x, circlePos3.y);

    RenderParticle c_rPC3 = RenderParticle(circleParticle3, &circleShape3);
    RenderParticles.push_back(&c_rPC3);
    //adds a force to the object directing to a specific local point location
    circleParticle3->AddForceOnPoint(MyVector(0, 10), MyVector(-10000, 0));

    //instantiation of the rectangularObjects with rigidBodies
    //rect 1
    RectPrismRb* rectParticle1 = new RectPrismRb();
    rectParticle1->w = 120;
    rectParticle1->h = 40;
    rectParticle1->mass = 5.0f;
    rectParticle1->startPos = MyVector(400, -100);
    rectParticle1->position = MyVector(400, -100);
    rectParticle1->velocity = MyVector(0, 0);
    rectParticle1->acceleration = MyVector(0, 0);
    rectParticle1->lifeSpan = 1000.0f;
    rectParticle1->stationary = false;
    rectParticle1->dampening = 0.9;
    rectParticle1->restitution = 0.6;
    rectParticle1->radius = sqrt(pow(rectParticle1->h, 2) + pow(rectParticle1->w, 2));
    rectParticle1->rotation = 45 * (acos(-1) / 180);
    rectParticle1->initializeRb((int)particleType::Rect);
    particleList.push_back(rectParticle1);
    pWorld.addParticle(rectParticle1);

    sf::RectangleShape rectShape1(sf::Vector2f(rectParticle1->w, rectParticle1->h));
    rectShape1.setFillColor(sf::Color::Blue);
    rectShape1.setOutlineColor(sf::Color::White);
    sf::Vector2f s1 = rectShape1.getSize();
    rectShape1.setOrigin(s1.x / 2, s1.y / 2); //suspicious
    rectList.push_back(&rectShape1);
    MyVector rectPos1 = Utils::P6ToSFMLPoint(MyVector(400, -00));
    rectShape1.setPosition(rectPos1.x, rectPos1.y);

    RenderParticle c_rPR1 = RenderParticle(rectParticle1, &rectShape1);
    RenderParticles.push_back(&c_rPR1);
    //adds a force to the object directing to a specific local point location
    rectParticle1->AddForceOnPoint(MyVector(0, 100), MyVector(10000, 0));
    
    //rect 2
    RectPrismRb* rectParticle2 = new RectPrismRb();
    rectParticle2->w = 120;
    rectParticle2->h = 40;
    rectParticle2->mass = 5.0f;
    rectParticle2->startPos = MyVector(400, -300);
    rectParticle2->position = MyVector(400, -300);
    rectParticle2->velocity = MyVector(0, 0);
    rectParticle2->acceleration = MyVector(0, 0);
    rectParticle2->lifeSpan = 1000.0f;
    rectParticle2->stationary = false;
    rectParticle2->dampening = 0.9;
    rectParticle2->restitution = 0.6;
    rectParticle2->radius = sqrt(pow(rectParticle2->h, 2) + pow(rectParticle2->w, 2));
    rectParticle2->rotation = 45 * (acos(-1) / 180);
    rectParticle2->initializeRb((int)particleType::Rect);
    particleList.push_back(rectParticle2);
    pWorld.addParticle(rectParticle2);

    sf::RectangleShape rectShape2(sf::Vector2f(rectParticle2->w, rectParticle2->h));
    rectShape2.setFillColor(sf::Color::Blue);
    rectShape2.setOutlineColor(sf::Color::White);
    sf::Vector2f s2 = rectShape2.getSize();
    rectShape2.setOrigin(s2.x / 2, s2.y / 2); //suspicious
    rectList.push_back(&rectShape2);
    MyVector rectPos2 = Utils::P6ToSFMLPoint(MyVector(400, -300));
    rectShape2.setPosition(rectPos2.x, rectPos2.y);

    RenderParticle c_rPR2 = RenderParticle(rectParticle2, &rectShape2);
    RenderParticles.push_back(&c_rPR2);
    //adds a force to the object directing to a specific local point location
    rectParticle2->AddForceOnPoint(MyVector(0, 100), MyVector(10000, 0));

    
    //rect 3
    RectPrismRb* rectParticle3 = new RectPrismRb();
    rectParticle3->w = 120;
    rectParticle3->h = 40;
    rectParticle3->mass = 5.0f;
    rectParticle3->startPos = MyVector(700, -300);
    rectParticle3->position = MyVector(700, -300);
    rectParticle3->velocity = MyVector(0, 0);
    rectParticle3->acceleration = MyVector(0, 0);
    rectParticle3->lifeSpan = 1000.0f;
    rectParticle3->stationary = false;
    rectParticle3->dampening = 0.9;
    rectParticle3->restitution = 0.6;
    rectParticle3->radius = sqrt(pow(rectParticle3->h,2) + pow(rectParticle3->w, 2));
    rectParticle3->rotation = 90 * (acos(-1) / 180);
    rectParticle3->initializeRb((int)particleType::Rect);
    particleList.push_back(rectParticle3);
    pWorld.addParticle(rectParticle3);

    sf::RectangleShape rectShape3(sf::Vector2f(rectParticle3->w, rectParticle3->h));
    rectShape3.setFillColor(sf::Color::Blue);
    rectShape3.setOutlineColor(sf::Color::White);
    sf::Vector2f s3 = rectShape3.getSize();
    rectShape3.setOrigin(s3.x / 2, s3.y / 2); //suspicious
    rectList.push_back(&rectShape3);
    MyVector rectPos3 = Utils::P6ToSFMLPoint(MyVector(700, -300));
    rectShape3.setPosition(rectPos3.x, rectPos3.y);

    RenderParticle c_rPR3 = RenderParticle(rectParticle3, &rectShape3);
    RenderParticles.push_back(&c_rPR3);
    //adds a force to the object directing to a specific local point location
    rectParticle3->AddForceOnPoint(MyVector(0, 100), MyVector(-10000, 0));

    int standing = 0;

    //clock/frame 
    using clock = high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    nanoseconds curr_ns(0ns);

    //particle effects properties
    int particleLimitSize = 25;

    sf::Event event;

    //MyVector startingPos = referencePoint1->position;
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
            if (!stopTimer)
                timer += (float)ms.count() / 1000;

            curr_ns -= timestep;

            //updates the rigidPoints of rectangleObjects per frame
            //also gets the current position of the particle and assigns it to the rectShape component
            //rect1
            rectParticle1->initializeRb((int)particleType::Rect);
            rectPos1 = Utils::P6ToSFMLPoint(rectParticle1->position);
            rectShape1.setPosition(rectPos1.x, rectPos1.y);
            rectShape1.setRotation(rectParticle1->rotation * (180 / acos(-1.0f)));

            //rect2
            rectParticle2->initializeRb((int)particleType::Rect);
            rectPos2 = Utils::P6ToSFMLPoint(rectParticle2->position);
            rectShape2.setPosition(rectPos2.x, rectPos2.y);
            rectShape2.setRotation(rectParticle2->rotation * (180 / acos(-1.0f)));

            //rect3
            rectParticle3->initializeRb((int)particleType::Rect);
            rectPos3 = Utils::P6ToSFMLPoint(rectParticle3->position);
            rectShape3.setPosition(rectPos3.x, rectPos3.y);
            rectShape3.setRotation(rectParticle3->rotation * (180 / acos(-1.0f)));

            //week 15
            //circle1
            circlePos1 = Utils::P6ToSFMLPoint(circleParticle1->position);
            circleShape1.setPosition(circlePos1.x, circlePos1.y);
            circleShape1.setRotation(circleParticle1->rotation * (180 / acos(-1.0f)));

            //circle2
            circlePos2 = Utils::P6ToSFMLPoint(circleParticle2->position);
            circleShape2.setPosition(circlePos2.x, circlePos2.y);
            circleShape2.setRotation(circleParticle2->rotation * (180 / acos(-1.0f)));

            //circle2
            circlePos3 = Utils::P6ToSFMLPoint(circleParticle3->position);
            circleShape3.setPosition(circlePos3.x, circlePos3.y);
            circleShape3.setRotation(circleParticle3->rotation * (180 / acos(-1.0f)));

            //cout << "X: " << rectParticle->position.x << " Y: " << rectParticle->position.y << endl;
            //cout << "X: " << rectShape.getPosition().x << " Y: " << rectShape.getPosition().y << endl;

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
            /*//Rotation lesson
            shurikenPos = Utils::P6ToSFMLPoint(originPoint->position);
            weightPos1 = Utils::P6ToSFMLPoint(referencePoint1->position);
            weightPos2 = Utils::P6ToSFMLPoint(referencePoint2->position);
            weightPos3 = Utils::P6ToSFMLPoint(referencePoint3->position);
            weightPos4 = Utils::P6ToSFMLPoint(referencePoint4->position);

            shuriken.setPosition(shurikenPos.x, shurikenPos.y);*/

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

            /*float rad_Angle = atan2f(referencePoint1->position.y - originPoint->position.y, referencePoint1->position.x - originPoint->position.x);
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
            shuriken.setRotation(-deg);*/

            //window.draw(shuriken);
            //window.draw(sampleShuriken);
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


/*//Circle2
CircleRb* circleParticle2 = new CircleRb();
sf::CircleShape circleShape2(20.f);
MyVector circlePos2 = Utils::P6ToSFMLPoint(MyVector(245, 20));
instantiateRB(pWorld, particleList, shapeList, circleParticle2, &circleShape2, RenderParticles,
    MyVector(600, 0), circlePos2, MyVector(0, 10), MyVector(-10000, 0), 20.f);*/
void instantiateRB(PhysicsWorld& pWorld, vector <MyParticle*>& particleList, vector <sf::CircleShape*>& shapeList, CircleRb* circle, sf::CircleShape* circleShape, std::list<RenderParticle*>& RenderParticles,
    MyVector position, MyVector circleUtilPos, MyVector locPoint, MyVector force, float radius)
{
    circle->initializeRb((int)particleType::Circ);
    circle->mass = 5.0f;
    circle->startPos = position;
    circle->position = position;
    circle->velocity = MyVector(0, 0);
    circle->acceleration = MyVector(0, 0);
    circle->lifeSpan = 1000.0f;
    circle->stationary = false;
    circle->dampening = 1;
    circle->restitution = 0.6;
    circle->radius = radius;
    particleList.push_back(circle);
    pWorld.addParticle(circle);

    circleShape->setFillColor(sf::Color::Red);
    circleShape->setOrigin(circleShape->getRadius(), circleShape->getRadius());
    shapeList.push_back(circleShape);
    circleShape->setPosition(circleUtilPos.x, circleUtilPos.y);

    RenderParticle c_rp = RenderParticle(circle, circleShape);
    RenderParticles.push_back(&c_rp);
    circle->AddForceOnPoint(locPoint, force);
}

