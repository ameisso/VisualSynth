#pragma once
#define nbSynthsForBalls 10//nombre de synthétiseurs dans le reaktor attribué aux balles.
#include "ofMain.h"
//#include "ofAddons.h"
#include "ofxOsc.h"
#include "Ball.h"
#include "Circle.h"
#include "Link.h"
#include <list>
class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void receiveOscMessage();
		void sendOscInfos(ofPtr<Ball>&ballToSend);
        int attributeSynth();//fonction qui attribue un synthétiseur à une balle.
    private :
        bool synthsForBalls[nbSynthsForBalls]={true};//tableau qui dit si un synthétiseur est attribué à une balle ou non.
        //!!!!passer en list list <bool>synthsForBalls;
        int profZ; //profondeur en z de la fenetre (jusqu'à ou on peut créer des balles en profondeur
        int lifeSpeed;
        int ballNoiseFactor; //détermine la vitesse de déplacement des balles.
        float zPos; //position en z de la balle.
		// vector to store all balls
		vector < ofPtr<Ball> >  theBalls;
        //OSC
        ofxOscReceiver OscReceiver;
        ofxOscMessage OscReceivedMessage;
        ofxOscSender oscSender;
};
