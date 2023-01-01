#include "Particles.h"
#include "Game.h"
#include <Arduboy2.h>

Particles particles;
Particles::Particle particle;

Game::Player particlePlayer;

void Particles::setup()
{
    Arduboy2::initRandomSeed();
}

void Particles::Particle::initialize()
{
        particle.size = random (1, 4);
        particle.direction = random(0, 2);
        particle.x = random (particlePlayer.x - particlePlayer.radius, particlePlayer.x + (particlePlayer.radius + 1));
        particle.xForce = random (0.1, 0.3);
        particle.y = random (particlePlayer.y - particlePlayer.radius, particlePlayer.y + (particlePlayer.radius + 1));
        particle.thrust = random (0.1, 0.5);
        particle.counter = random (180, 241);
}

void Particles::Particle::update()
{ 
        particle.yVelocity += particles.gravity * particle.size;
        particle.yVelocity -= particle.thrust / particle.size;
        
        if (particle.direction == 0)
        {
            particle.xVelocity += particle.xForce;
        }

        if (particle.direction == 1)
        {
            particle.xVelocity += -particle.xForce;
        }

        particle.x += particle.xVelocity;
        particle.y += particle.yVelocity;

        if (particle.counter > 0) 
        {
            particle.counter--;
        }
}

bool Particles::Particle::render()
{
    return (this->counter > 0);
}