#include "Ball.h"
#include "testApp.h"
Ball::Ball()
{
    //ctor
}

Ball::Ball(int refNumber_,int posx,int posy,int posz,int synthNumber_,string  pathToImage_,float r_, int noiseFactor_, float lifeSpeed_, float maxRadius_, float minRadius_, int nbCircles_, int circleWidth_, int circleWidthFactor_)
{
    refNumber=refNumber_;
    cout<<"newBall "<<refNumber;
    position.x=posx;
    position.y=posy;
    position.z=posz;
    pathToImage=pathToImage_;
    r=r_; //ball radius
    minRadius=minRadius_;
    maxRadius=maxRadius_;
    nbCircles=nbCircles_;
    noiseFactor=noiseFactor_;
    circleWidth=circleWidth_;
    circleWidthFactor=circleWidthFactor_;
    isDead=false;
    lifeSpeed=lifeSpeed_;
    synthNumber=synthNumber_;

    // image
    texBall.loadImage(pathToImage);

    // each ball has a plane
    ballPlane.set(100, 100); // initials values, change at the first display
    ballPlane.setPosition(200,200,10); //idem
    ballPlane.mapTexCoords(0, 0, texBall.getWidth(), texBall.getHeight());


    fadeIn(r);
    cout<<" : ["<<posx<<","<<posy<<","<<posz<<"] synth :"<<synthNumber<<endl;
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
        //cout << (*it)->getRadius() << endl;
    else if(position.x<0||position.x>ofGetWidth()||position.y<0||position.y>ofGetHeight())
    {
        fadeOut();
    }

    for(vector< ofPtr<Circle> >::iterator it = theCircles.begin(); it != theCircles.end(); ++it)
    {
        (*it)->update(position);//,(*it)->getRadius(), (*it)->getCircleWidth());
    }
}
void Ball::draw()
{
    texBall.bind();
    ballPlane.set(r,r);
    ballPlane.setPosition(position.x, position.y,position.z);
    ballPlane.draw();
    texBall.unbind();
    // we display each ring from each ball
    for(vector< ofPtr<Circle> >::iterator it = theCircles.begin(); it != theCircles.end(); ++it)
    {
        (*it)->getRing().draw();
    }
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
    removeCircles();
    removeLinks();
    r=0;
    isDead=true;
}
void Ball::removeCircles()
{
    theCircles.clear();
    nbCircles = 0;
}
void Ball::removeLinks()
{
    linksConnected.clear();
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
void Ball::addCircle(int circleRadius, int circleWidth)
{
    int radiusIncrease = 30;
    theCircles.push_back(ofPtr<Circle> (new Circle(circleRadius + nbCircles*radiusIncrease, circleWidth, position)));
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
void Ball::addConnectedLink(int refNumber)
{
    linksConnected.push_back(refNumber);
}


bool Ball::linkExist(Ball testedBall)
{
    //function that test if the ball exist or not.
    bool test=false;

    for(vector< int >::iterator it = linksConnected.begin(); it != linksConnected.end(); ++it)
    {
        //TODO chercher dans linksConnected si un lien avec la testedBall existe ou non
        if ((*it)==testedBall.getRefNumber())
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
void Ball::addLink(int otherBallRefNumber)
{
    linksConnected.push_back(otherBallRefNumber);
}
void Ball::removeLink(int otherBallRefNumber)
{
    for(vector< int >::iterator it = linksConnected.begin(); it != linksConnected.end(); ++it)
    {
        if((*it)==otherBallRefNumber)
        {
            linksConnected.erase(it);
        }
    }
}
int Ball::getRefNumber()
{
    return refNumber;
}
bool Ball::checkLink(int refNumber)
{
    bool test=false;
    for(vector< int >::iterator it = linksConnected.begin(); it != linksConnected.end(); ++it)
    {
        if((*it)==refNumber)
        {
            test=true;
            break;
        }
    }
    return test;
}
