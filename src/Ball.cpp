#include "Ball.h"

Ball::Ball()
{
    //ctor
}
Ball::Ball(int posx,int posy, int posz,float r_, float maxRadius_, float minRadius_, int nbCircles_, int noiseFactor_, int circleWidth_, int circleWidthFactor_)
{
    cout<<"newBall : "<<posx<<" "<<posy<<" "<<posz<<endl;
    position.x=posx;
    position.y=posy;
    position.z=posz;
    r=r_; //ball radius
    minRadius=minRadius_;
    maxRadius=maxRadius_;
    nbCircles=nbCircles_;
    noiseFactor=noiseFactor_;
    circleWidth=circleWidth_;
    circleWidthFactor=circleWidthFactor_;
    isDead=false;

    fadeIn(r);
}

Ball::~Ball()
{
    removeCircles();
    removeLinks();
}
void Ball::fadeIn(float radius)
{
    //ball fade in when created, make the radius and the opacity grow to the defined radius in parameter.
}
void  Ball::fadeOut()
{

}
void Ball::removeCircles()
{
    theCircles.clear();
}
void Ball::removeLinks()
{
    for(vector< ofPtr<Link> >::iterator it = linksConnected.begin(); it != linksConnected.end(); ++it)
    {
        (*it)->setIsDead(true);
    }
}
ofVec3f Ball::getPosition()
{
    return position;
}
float Ball::getRadius()
{
    return r;
}
bool Ball::checkIfDead()
{
    return isDead;
}
void Ball::setRadius(float radius)
{
    if (radius>maxRadius||radius<minRadius)
    {
        isDead=true;
    }
    else
    {
        r=radius;
    }
}
void Ball::addCircle(int radius, int circleWidth)
{
    theCircles.push_back(ofPtr<Circle> (new Circle(radius, circleWidth, position)));
}
void Ball::setCircleWidthFactor(int factor)
{
    circleWidthFactor=factor;
}
void Ball::setCircleWidth(int factor)
{
    circleWidth=factor;
}
void Ball::addConnectedLink(Link *link)
{
    linksConnected.push_back(ofPtr<Link>(link));
}


bool Ball::linkExist(Ball testedBall)
{
    //function that test if the ball exist or not.
    bool test=false;

    for(vector< ofPtr<Link> >::iterator it = linksConnected.begin(); it != linksConnected.end(); ++it)
    {
        //TODO chercher dans linksConnected si un lien avec la testedBall existe ou non
        if (true)
        {
            test=true;
            //println("ball1 :"+ball1+" "+" Ball2 :"+ball2);
            break;
        }
    }
    return test;
}
void Ball::setNoiseFactor(int nF)
{
    noiseFactor=nF;
}

