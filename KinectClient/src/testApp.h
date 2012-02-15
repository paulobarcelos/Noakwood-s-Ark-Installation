#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "Game.h"
#include "ofxGLWarper.h"

#define PORT 12345

class testApp : public ofBaseApp{
	public:

		void setup();
		void update();
		void draw();
        void keyPressed(int key);

		ofxOscReceiver receiver;  
    
        int w, h;
        Game game;
        ofxGLWarper warper;
    
};
