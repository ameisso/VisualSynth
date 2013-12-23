#ifndef BALL_H
#define BALL_H
#include "ofMain.h"
#include "Circle.h"
class Link;
class Ball
{
public:
    Ball();
    virtual ~Ball();
    Ball(int posx,int posy, int posz=0,float r_=5, float maxRadius_=50, float minRadius_=10, int nbCircles_=5, int noiseFactor_=100,int circleWidth_=5,int circleWidthFactor_=30);
    void fadeIn(float r);
    void fadeOut();
    void setPosition();
    void removeCircles();
    void removeLinks();
    ofVec3f getPosition();
    bool checkIfDead();
    void setRadius(float radius);
    void addCircle(int radius, int circleWidth);
    void setCircleWidthFactor(int factor);
    void setCircleWidth(int factor);
    void addConnectedLink(Link* link);
    bool linkExist(Ball testedBall);
    void setNoiseFactor(int nF);

protected:
private:

    ofVec3f position;//ball position
    ofVec3f velocity;//ball velocity
    float maxRadius;//max radius of the ball
    float minRadius;
    float r;//current radius of the bal    vector < ofPtr<Circle> >  theCircles;
    vector<ofPtr<Link> > linksConnected;
    vector<ofPtr<Circle> > theCircles;
    bool isDead;
    int nbCircles;
    int noiseFactor;//noise factor that make the ball move arround her position
    int circleWidth;
    int circleWidthFactor;
};

#endif // BALL_H
