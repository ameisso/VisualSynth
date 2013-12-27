#ifndef CIRCLE_H
#define CIRCLE_H
#include "ofMain.h"
class Circle
{
public:
    Circle(int radius_, int circleWidth_, ofVec3f center_);
    virtual ~Circle();
    void update(ofVec3f center_, int radius_, int circleWidth_);
    void setRadius(int radius_);
    int getRadius();
    ofPath getRing();

protected:
private:
    int radius, circleWidth;
    ofVec3f center;
    int initialRadius;
    ofPath ring;
    //openGL
    ofPlanePrimitive circlePlane;
    ofImage texCircle;
};

#endif // CIRCLE_H
