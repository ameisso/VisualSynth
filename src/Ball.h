#ifndef BALL_H
#define BALL_H
#include "ofMain.h"
#include "Circle.h"
#include "Link.h"
class Link;
class Ball
{
public:
    Ball();
    virtual ~Ball();
    Ball(int posx,int posy, int posz ,int synthNumber_,float r_=5, int noiseFactor_=100,float maxRadius_=50, float minRadius_=10, int nbCircles_=5,int circleWidth_=5,int circleWidthFactor_=30, float lifeSpeed=0.995);
    void update();
    void draw();

    void fadeIn(float r);
    void fadeOut();
    void setPosition();
    void removeCircles();
    void removeLinks();
    ofVec3f getPosition();
    bool checkIfDead();
    float getRadius();
    void setRadius(float radius);
    void addCircle(int radius, int circleWidth);
    void setCircleWidthFactor(int factor);
    void setCircleWidth(int factor);
    void addConnectedLink(Link* link);
    bool linkExist(Ball testedBall);
    void setNoiseFactor(int nF);
    Ball operator=(const Ball&);
    int getSynthNumber();
    int getNbCircles();
    vector<ofPtr<Circle> > getTheCircles();



protected:
private:

    ofVec3f position;//ball position
    ofVec3f velocity;//ball velocity
    float maxRadius;//max radius of the ball
    float minRadius;
    float r;//current radius of the bal
    float lifeSpeed;
    vector<ofPtr<Link> > linksConnected;
    vector<ofPtr<Circle> > theCircles;
    bool isDead;
    int nbCircles;
    int noiseFactor;//noise factor that make the ball move arround her position
    int circleWidth;
    int circleWidthFactor;
    int synthNumber;//numéro du synthétiseur qui est attribué à la balle.
};

#endif // BALL_H
