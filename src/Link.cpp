#include "Link.h"


Link::Link(Ball &ball1_, Ball &ball2_)
{
    ball1 =new Ball();
    ball2 =new Ball();

    ball1=&ball1_;
    ball2=&ball2_;
    ball1->addConnectedLink(this);
    ball2->addConnectedLink(this);
    isDead=false;
}
Link::~Link()
{
    //dtor
}
bool Link::checkIfDead()
{
    return isDead;
}
void Link::setIsDead(bool boolean)
{
    isDead=boolean;
}
void Link::getFirstBall(Ball &firstBall)
{
    //TODO retourner la ball1 si besoin
}
void Link::getSecondBall(Ball &secondBall)
{
    //TODO retourner la ball2 si besoin
}

