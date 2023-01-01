#pragma once

#include <stdint.h>

class Particles
{
    private:

        float gravity = 0.1;

    public:

        void setup();

        struct Particle    //this code is a modified example by @filmote from this thread: https://community.arduboy.com/t/how-to-get-started-with-particles/9190/3
        {
            private:

                float x;
                float xVelocity;
                float xForce;

                float y;
                float yVelocity;

                float thrust;

                uint8_t size;
        
                uint8_t counter;
                uint8_t direction;    //if 0, direction will be right. if 1, direction will be left

            public:

                float getX()                            { return this->x; }
                float getY()                            { return this->y; } 
                float getXVelocity()                    { return this->xVelocity; }
                float getYVelocity()                    { return this->yVelocity; }
                float getSize()                         { return this->size; }
                uint8_t getCounter()                    { return this->counter; }
                uint8_t getDirection()                  { return this->direction; }

                void setX(float value)                  { this->x = value; }
                void setY(float value)                  { this->y = value; }
                void setXVelocity(float value)          { this->xVelocity = value; }
                void setYVelocity(float value)          { this->yVelocity = value; }
                void setCounter(uint8_t value)          { this->counter = value; }
                void setDirection(uint8_t value)        { this->direction = value; }

            public:
        
                void initialize();
                void update();
                bool render();
        };

        Particle particleArray[10];    
};
