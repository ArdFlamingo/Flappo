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
    for (auto & particleSet : particles.particleArray)
    {
        particleSet.size = random (4, 8);
        particleSet.direction = random(0, 2);
        particleSet.x = random (particlePlayer.x - particlePlayer.radius, particlePlayer.x + (particlePlayer.radius + 1));
        particleSet.xForce = random (0.1, 0.3);
        particleSet.y = random (particlePlayer.y - particlePlayer.radius, particlePlayer.y + (particlePlayer.radius + 1));
        particleSet.thrust = random (0.1, 0.5);
        particleSet.counter = random (180, 241);
    }
}

void Particles::Particle::update()
{   
    for (auto & particleSet : particles.particleArray)
    {
        particleSet.yVelocity += particles.gravity * particleSet.size;
        particleSet.yVelocity -= particleSet.thrust / particleSet.size;
        
        if (particleSet.direction == 0)
        {
            particleSet.xVelocity += particleSet.xForce;
        }

        if (particleSet.direction == 1)
        {
            particleSet.xVelocity += -particleSet.xForce;
        }

        particleSet.x += particleSet.xVelocity;
        particleSet.y += particleSet.yVelocity;
    }

    if (this->counter > 0) counter--;

    if (render())
    {
        render();
    }
}

bool Particles::Particle::render()
{
    for (uint8_t i = 0; i < 10; i++)
    {
        Arduboy2::drawRect(particles.particleArray[i].x, particles.particleArray[i].y, particles.particleArray[i].size, particles.particleArray[i].size);
    }

    return (this->counter > 0);
}