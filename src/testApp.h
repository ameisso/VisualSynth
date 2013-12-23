#pragma once

#include "ofMain.h"
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

		void addPoint(float x, float y, float z);

		// vector to store all values
		vector < ofPtr<Ball> >  theBalls;

		ofVbo vbo;
		ofShader shader;
		ofEasyCam camera;

		float camDist;

		ofTexture texture;

};
