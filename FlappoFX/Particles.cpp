#include <Arduboy2.h>
#include "Particles.h"

Game::Player gamePlayer;


void Particles::updateParticles()
{
    for (auto & particle : particleArray)
    {
        particle.yVelocity += this->gravity * particle.size;
        
        particle.yVelocity -= particle.yForce / (particle.size * 0.5);
        particle.xVelocity += particle.xForce / (particle.size * 0.15);

        particle.xForce *= 0.95;
        particle.yForce *= 0.95;

        particle.x += particle.xVelocity;
        particle.y += particle.yVelocity;

        if (particle.isActive())
            particle.counter--;
    }
}

void Particles::drawParticles()
{
    for (auto & particle : this->particleArray)
        if (particle.isActive())
            Arduboy2::drawRect(particle.x, particle.y, particle.size, particle.size);
}

void Particles::resetParticles()
{
    for (auto & particle : particleArray)
    {
        particle.size = random(2, 7);
        particle.counter = random(85, 100);

        particle.x = random(playerX - 5, playerX + 5 + 1);
        particle.y = random(playerY - 5, playerY + 5 + 1);

        particle.xForce = random(-5, 6) / 100.0;
        particle.yForce = random(25, 50) / 100.0;
    }
}