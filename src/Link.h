#ifndef LINK_H
#define LINK_H
#include "ofMain.h"
#include "Ball.h"

class Ball;//inclue une classe pour permettre la compilation, mais le compilateur le trouvera plus tard
class Link
{
    public:
        Link(Ball &ball1_, Ball &ball2_);
        virtual ~Link();
        bool checkIfDead();
        void setIsDead(bool boolean);
        void getFirstBall(Ball & firstBall);
        void getSecondBall(Ball & secondBall);
    protected:
    private:
    Ball *ball1;//ball connected to the link
    Ball *ball2;
    bool isDead;
    void toString();
};

#endif // LINK_H
