#ifndef _TEST_APP
#define _TEST_APP
#include "ofMain.h"
#include "ofxBox2d.h"

// ------------------------------------------------- a simple extended box2d circle
class CustomParticle : public ofxBox2dCircle {
	
public:
	float c;
	CustomParticle() {
		c = (int)ofRandom(30, 100);
	}
	ofColor color;
	void draw() {
		float radius = getRadius();
		
		glPushMatrix();
		glTranslatef(getPosition().x, getPosition().y, 0);
		
		ofSetColor(c, c, c);
		ofFill();
		ofCircle(0, 0, radius);	
		
		glPopMatrix();
		
	}
};

// -------------------------------------------------

class testApp : public ofBaseApp {
	
public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void resized(int w, int h);
	
	float px, py;
	bool							bDrawLines;
	bool							bMouseForce;
	
	ofxBox2d						box2d;			  //	the box2d world
	vector		<CustomParticle>	customParticles;  //	this is a custom particle the extends a cirlce
	
	ofxBox2dLine					lineStrip;		  //	a linestrip for drawing
	
};

#endif
