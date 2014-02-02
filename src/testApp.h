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
		int refNumber;
    private :
        vector <bool>synthsForBalls;
		void sendOscInfos(ofPtr<Ball>&ballToSend);
		void sendOscGeneral();
        int attributeSynth();//fonction qui attribue un synthétiseur à une balle.
        void readXmlSetup();

	//BALLS
		int profZ; //profondeur en z de la fenetre (jusqu'à ou on peut créer des balles en profondeur
		float lifeSpeed;
		int ballNoiseFactor; //détermine la vitesse de déplacement des balles.
		float zPos; //position en z de la balle.
		int nbSynthsForBalls;
	//LINKS
		int minDistToLink;
		int maxDistToUnlink;
		bool showLinks;
		bool curvedLinks;
		float curveFactor;
		float curvePosition;
		float curveAmplitude;
	//CIRCLES
		int maxCircles;//nombre max de cercles par balle
		int nbCircles;//nombre de cercles.
		int circleWidth;//largeur de cercles
		int circleIncrease;//largeur entre deux cercles.
		int maxCircleWidth;
		int maxCircleIncrease;
	//TEXTURES
        string pathToImages;
        string imageName;
		// vector to store all balls
		vector < ofPtr<Ball> > theBalls;
		vector < ofPtr<Ball> > permanentBalls;
		vector < ofPtr<Link> > theLinks;
		vector < string > theTextures;
	//OSC
        ofxOscReceiver OscReceiver;
        ofxOscMessage OscReceivedMessage;
        ofxOscSender oscSender;
        int oscReceivePort;
        int oscSendPort;
        string oscSendAddress;
		
};
