#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "Game.h"

#define PORT 12345

class testApp : public ofBaseApp{
	public:

		void setup();
		void update();
		void draw();

		ofxOscReceiver receiver;  
    
        Game game;
    
};
