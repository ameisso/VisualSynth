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


    // BALLES
    Ball(int refNumber,int posx,int posy, int posz ,int synthNumber_,string pathToImage_, int noiseFactor_=100, float lifeSpeed=0.995,int nbCircles_=0,int circleWidth_=5,int circleWidthFactor_=30,float maxRadius_=100, float minRadius_=10, float brightness_=255);
	
    void update();
    void fadeIn(float r);
    void fadeOut();
	void setLifeSpeed(float life);
	void setNoiseFactor(float factor);
    ofVec3f getPosition();
	ofVec3f getVelocity();
    bool checkIfDead();
    float getRadius();
    int getSynthNumber();
    int refNumber;
    int getRefNumber();
    void setRadius(float radius);
    void setNoiseFactor(int nF);
    //CERCLES
    void removeCircles();
    void addCircle(int circleRadius, int circleWidth);
    void setCircleWidth(float factor);
    int getNbCircles();
    vector<ofPtr<Circle> > getTheCircles();
	void setCircleBrightness(int brightness_);

    //LIENS
    void removeLinks();
    void addConnectedLink(int refNumber);
    bool linkExist(Ball testedBall);
    void addLink(int otherBallRefNumber);
    void removeLink(int otherBallRefNumber);
    bool checkLink(int refNumber);

    //AFFICHAGE
    void draw();
    void setBrightness(float newBrigthness);


protected:
private:

    bool isDead;
    int synthNumber;//numéro du synthétiseur qui est attribué à la balle.

    // DIMENSIONS
    ofVec3f position;//ball position
    ofVec3f velocity;//ball velocity
    float lifeSpeed;
    float maxRadius;//max radius of the ball
    float minRadius;
    float r;//current radius of the ball vector < ofPtr<Circle> > theCircles;
    int noiseFactor;//noise factor that make the ball move arround her position

    // CERCLES
    vector<ofPtr<Circle> > theCircles;
    int nbCircles;
    float circleWidth;
    int circleWidthFactor;

    // LIENS
    vector<int> linksConnected;


    // AFFICHAGE
    ofColor ballColor;
    float hue;
    float brightness;
    float saturation;
    string pathToImage;
    ofPlanePrimitive ballPlane;
    ofImage texBall;

    const int listHue[10] = {5, 10, 22, 28, 32, 34, 37, 39, 42, 44};
};

#endif // BALL_H
