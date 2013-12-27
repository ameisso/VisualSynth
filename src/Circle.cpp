#include "Circle.h"
Circle::Circle(int radius_, int circleWidth_, ofVec3f center_)
{
    radius=radius_;
    circleWidth=circleWidth_;
    center=center_;

    ofPoint p(center.x,center.y,1);
    ring.circle(p,radius+circleWidth/2);
    ring.circle(p,radius-circleWidth/2);
	ring.close();
    ofColor c(255, 0,0);
	ring.setFillColor(c);
	ring.setFilled(true);
}
Circle::~Circle()
{
    //dtor
}
void Circle::update(ofVec3f center_, int radius_, int circleWidth_)
{
    radius=radius_;
    initialRadius=radius;
    circleWidth=circleWidth_;
    center=center_;

    ofPoint p(center.x,center.y);
    ring.circle(p,radius+circleWidth/2);
    ring.circle(p,radius-circleWidth/2);
	ring.close();
    ofColor c(255, 0,0);    // ring color ! -> new parameter ?
	ring.setFillColor(c);
	ring.setFilled(true);
}

void Circle::setRadius(int radius_)
{
    radius=radius_;
}
int Circle::getRadius()
{
    return radius;
}

ofPath Circle::getRing()
{
    return ring;
}
