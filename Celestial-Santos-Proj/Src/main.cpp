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
    Utils::offset = MyVector(0, 0);
    DragForceGenerator df = DragForceGenerator(0,0); //setted to zero(0); kinetic friction

    //storage of the particles and shapes object
    vector <MyParticle*> particleList;
    vector <sf::CircleShape*> shapeList;

    //for rods connections
    Cable r1 = Cable();
    Cable r2 = Cable();
    Cable r3 = Cable();
    Cable r4 = Cable();
    Cable r5 = Cable();

    //for anchors
    MyParticle* anchor1 = new MyParticle(1.0f, MyVector(200, 200), MyVector(0, 0), MyVector(0, 0), 100, true);
    pWorld.addParticle(anchor1);

    MyParticle* anchor2 = new MyParticle(1.0f, MyVector(260, 200), MyVector(0, 0), MyVector(0, 0), 100, true);
    pWorld.addParticle(anchor2);

    MyParticle* anchor3 = new MyParticle(1.0f, MyVector(320, 200), MyVector(0, 0), MyVector(0, 0), 100, true);
    pWorld.addParticle(anchor3);

    MyParticle* anchor4 = new MyParticle(1.0f, MyVector(380, 200), MyVector(0, 0), MyVector(0, 0), 100, true);
    pWorld.addParticle(anchor4);

    MyParticle* anchor5 = new MyParticle(1.0f, MyVector(440, 200), MyVector(0, 0), MyVector(0, 0), 100, true);
    pWorld.addParticle(anchor5);

    //for the balls
    instantiateParticles(RenderParticles, pWorld, 25.0f, MyVector(200, 400),
        MyVector(0, 0), MyVector(0, 0), 100, df, particleList, shapeList);
    instantiateParticles(RenderParticles, pWorld, 25.0f, MyVector(260, 400),
        MyVector(0, 0), MyVector(0, 0), 100, df, particleList, shapeList);
    instantiateParticles(RenderParticles, pWorld, 25.0f, MyVector(320, 400),
        MyVector(0, 0), MyVector(0, 0), 100, df, particleList, shapeList);
    instantiateParticles(RenderParticles, pWorld, 25.0f, MyVector(380, 400),
        MyVector(0, 0), MyVector(0, 0), 100, df, particleList, shapeList);
    instantiateParticles(RenderParticles, pWorld, 25.0f, MyVector(440, 400),
        MyVector(0, 0), MyVector(0, 0), 100, df, particleList, shapeList);

    //anchor and balls connections
    r1.particles[0] = anchor1;
    r1.particles[1] = particleList[0];
    r2.particles[0] = anchor2;
    r2.particles[1] = particleList[1];
    r3.particles[0] = anchor3;
    r3.particles[1] = particleList[2];
    r4.particles[0] = anchor4;
    r4.particles[1] = particleList[3];
    r5.particles[0] = anchor5;
    r5.particles[1] = particleList[4];

    //sets the settings for rods; problem here; last element of Links doesn't trigger the length limit
    r1.length = 200;
    pWorld.Links.push_back(&r1);
    r2.length = 200;
    pWorld.Links.push_back(&r2);
    r3.length = 200;
    pWorld.Links.push_back(&r3);
    r4.length = 200;
    pWorld.Links.push_back(&r4);
    r5.length = 200;
    pWorld.Links.push_back(&r5);
    r5.length = 200;

    //line/cable representation
    sf::VertexArray line1(sf::Lines, 2);
    line1[0].position = sf::Vector2f(200, 200);
    line1[0].color = sf::Color::White;

    sf::VertexArray line2(sf::Lines, 2);
    line2[0].position = sf::Vector2f(260, 200);
    line2[0].color = sf::Color::White;

    sf::VertexArray line3(sf::Lines, 2);
    line3[0].position = sf::Vector2f(320, 200);
    line3[0].color = sf::Color::White;

    sf::VertexArray line4(sf::Lines, 2);
    line4[0].position = sf::Vector2f(380, 200);
    line4[0].color = sf::Color::White;

    sf::VertexArray line5(sf::Lines, 2);
    line5[0].position = sf::Vector2f(440, 200);
    line5[0].color = sf::Color::White;

    //Adds a dragforce Generator
    pWorld.forceRegistry.Add(particleList[0], &df);    //adds the friction
    pWorld.forceRegistry.Add(particleList[1], &df);    //adds the friction
    pWorld.forceRegistry.Add(particleList[2], &df);    //adds the friction
    pWorld.forceRegistry.Add(particleList[3], &df);    //adds the friction
    pWorld.forceRegistry.Add(particleList[4], &df);    //adds the friction

    //clock/frame 
    using clock = high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    nanoseconds curr_ns(0ns);

    //particle effects properties
    int particleLimitSize = 25;

    sf::Event event;

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


            //Updates the shapes and particles
            pWorld.Update((float)ms.count() / 1000);
            line1[1].position = sf::Vector2f(particleList[0]->position.x, particleList[0]->position.y);
            line2[1].position = sf::Vector2f(particleList[1]->position.x, particleList[1]->position.y);
            line3[1].position = sf::Vector2f(particleList[2]->position.x, particleList[2]->position.y);
            line4[1].position = sf::Vector2f(particleList[3]->position.x, particleList[3]->position.y);
            line5[1].position = sf::Vector2f(particleList[4]->position.x, particleList[4]->position.y);

            curr_ns -= timestep;

            while (window.pollEvent(event))
            {
                switch (event.type)
                {
                case sf::Event::Closed:
                    window.close();
                    break;
                }
                if (sf::Keyboard::isKeyPressed && event.key.code == sf::Keyboard::Key::Space)
                {
                    particleList[0]->AddForce(MyVector(-35000, 0));
                }
            }
            window.clear();
            //iterates the particles then draw
            for (std::list<RenderParticle*>::iterator i = RenderParticles.begin();
                i != RenderParticles.end(); i++)
            {
                (*i)->Draw(&window);
            }
            window.draw(line1);
            window.draw(line2);
            window.draw(line3);
            window.draw(line4);
            window.draw(line5);
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

