#ifndef LINK_H
#define LINK_H
#include "ofMain.h"
#include "Ball.h"
class Ball;//inclue une classe pour permettre la compilation, mais le compilateur le trouvera plus tars
class Link
{
    public:
        Link(Ball ball1_, Ball ball2_);
        virtual ~Link();
        bool checkIfDead();
        void setIsDead(bool boolean);
        Ball getBall(int i);
    protected:
    private:
    Ball ball1;//ball connected to the link
    Ball ball2;
    bool isDead;
};

#endif // LINK_H
