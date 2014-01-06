#include "Link.h"


Link::Link(int refBall1, int refBall2)
{
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

