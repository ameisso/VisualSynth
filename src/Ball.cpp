#include "Ball.h"


Ball::Ball()
{
    //constructeur par défaut
}
Ball::Ball(int posx,int posy,int posz,int synthNumber_,float r_, int noiseFactor_, float maxRadius_, float minRadius_, int nbCircles_, int circleWidth_, int circleWidthFactor_, float lifeSpeed_)
{

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
    lifeSpeed=lifeSpeed_;
    synthNumber=synthNumber_;

    fadeIn(r);
    cout<<"newBall : ["<<posx<<","<<posy<<","<<posz<<"] synth :"<<synthNumber<<endl;
}
void Ball::update()
{
    float sgn=ofRandom(1)-0.5;//On tire au sort le signe de la vibration
    velocity.x+=sgn*ofNoise(velocity.x)*noiseFactor*0.01;
    sgn=ofRandom(1)-0.5;
    velocity.y+=sgn*ofNoise(velocity.y)*noiseFactor*0.01;
    sgn=ofRandom(1)-0.5;
    velocity.z+=sgn*ofNoise(velocity.z)*noiseFactor*0.01;
    position+=velocity;
    r*=lifeSpeed;
    //si le rayon dépasse les bornes, on fade out puis on supprime
    if(r<minRadius||r>maxRadius)
    {
        fadeOut();
    }
    //cout<<"updated"<<ofToString(position)<<endl;
}
void Ball::draw()
{


}

Ball::~Ball()
{
    removeCircles();
    removeLinks();
}
void Ball::fadeIn(float radius)
{
    //ball fade in when created, make the radius and the opacity grow to the defined radius in parameter.
    isDead=false;
}
void  Ball::fadeOut()
{
  isDead=true;
}
void Ball::removeCircles()
{
    theCircles.clear();
    nbCircles = 0;
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
    int radiusIncrease = 30;
    theCircles.push_back(ofPtr<Circle> (new Circle(radius + nbCircles*radiusIncrease, circleWidth, position)));
    nbCircles ++;
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
int Ball::getSynthNumber()
{
    return synthNumber;
}

int Ball::getNbCircles()
{
    return nbCircles;
}

vector<ofPtr<Circle> > Ball::getTheCircles()
{
    return theCircles;
}
