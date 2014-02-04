#ifndef CIRCLE_H
#define CIRCLE_H
#include "ofMain.h"
class Circle
{
public:
    Circle(int radius_, float circleWidth_, ofVec3f center_);
    virtual ~Circle();
    void update(ofVec3f center_, float circleWidth_,int radius_);
    void setRadius(int radius_);
    int getRadius();
    int getCircleWidth();
    ofPath getRing();
	void setBrightness(int brightness);

protected:
private:
    int radius, circleWidth;
    ofVec3f center;
    int initialRadius;
    ofPath ring;
    //openGL
    ofPlanePrimitive circlePlane;
    ofImage texCircle;
	ofColor circleColor;
	int hue, saturation,brightness;
};

#endif // CIRCLE_H
