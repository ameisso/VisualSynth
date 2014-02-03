#include "Link.h"


Link::Link(Ball ball1_, Ball ball2_)
{
    ball1=ball1_;
    ball2=ball2_;
    ball1.addConnectedLink();
    ball2.addConnectedLink(this);
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
Ball Link::getBall(int i)
{
    if(i==0)return ball1;
    else if (i==1) return ball2;
}
