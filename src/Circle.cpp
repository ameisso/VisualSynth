#include "Circle.h"
Circle::Circle(int radius_, float circleWidth_, ofVec3f center_)
{
    radius=radius_;
    circleWidth=circleWidth_;
    center=center_;

    ofPoint p(center.x,center.y,1);
    ring.setCircleResolution(100);
    ring.circle(p,radius_+circleWidth_/2);
    ring.circle(p,radius_-circleWidth_/2);
	ring.close();
	hue=255;
	saturation=0;
	brightness=255;
    circleColor.setHsb(hue,saturation,brightness);
	ring.setFillColor(circleColor);
	ring.setFilled(true);
}
Circle::~Circle()
{
    //dtor
}
void Circle::update(ofVec3f center_, float circleWidth_, int radius_)
{
    center=center_;
	circleWidth = circleWidth_;
	radius = radius_;
    ring.clear();
    ofPoint p(center);
    ring.setCircleResolution(100);
    ring.circle(p,radius+circleWidth/2);
    ring.circle(p,radius-circleWidth/2);
	ring.close();
	ring.setFillColor(circleColor);
	ring.setFilled(true);
	//cout<<circleColor<<endl;
}

void Circle::setRadius(int radius_)
{
    radius=radius_;
}
int Circle::getRadius()
{
    return radius;
}

int Circle::getCircleWidth()
{
    return circleWidth;
}

ofPath Circle::getRing()
{
    return ring;
}
void Circle::setBrightness(int circlebrightness_)
{
	brightness=circlebrightness_;
	circleColor.setBrightness(circlebrightness_);
	cout<<"brightness dans cercle"<<brightness<<endl;
}
