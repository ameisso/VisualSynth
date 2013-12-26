#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "Ball.h"
#include "Circle.h"
#include "Link.h"
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

    private :
        int profZ; //profondeur en z de la fenetre (jusqu'à ou on peut créer des balles en profondeur
        int lifeSpeed;
        int ballNoiseFactor; //détermine la vitesse de déplacement des balles.
        float zPos; //position en z de la balle.
		// vector to store all balls
		vector < ofPtr<Ball> >  theBalls;
        //OSC
        ofxOscReceiver OscReceiver;
        ofxOscMessage OscReceivedMessage;


        ofPlanePrimitive ballPlane;
        ofPlanePrimitive circlePlane;
		ofImage texBall;
		ofImage texCircle;
};
