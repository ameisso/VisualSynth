#ifndef LINK_H
#define LINK_H
#include "ofMain.h"
#include "Ball.h"

class Ball;//inclue une classe pour permettre la compilation, mais le compilateur le trouvera plus tard
class Link
{
    public:
        Link(int refBall1_, int refBall2_);
        virtual ~Link();
        bool checkIfDead();
        void setIsDead(bool boolean);

    protected:
    private:
    int refBall1,refBall2;
    bool isDead;
    void toString();
};

#endif // LINK_H
