#include "Circle.h"
Circle::Circle(int radius_, int circleWidth_, ofVec3f center_)
{
    radius=radius_;
    circleWidth=circleWidth_;
    center=center_;
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
}
void Circle::setRadius(int radius_)
{
    radius=radius_;
}
int Circle::getRadius()
{
    return radius;
}
