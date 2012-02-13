#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	receiver.setup( PORT );
    ofSetVerticalSync(true);
    
    int w = 1028;
    int h = 600;
    
    ofSetWindowShape(w, h);    
    game.setup(w,h);
}

//--------------------------------------------------------------
void testApp::update(){
    
    while( receiver.hasWaitingMessages() )
	{
		ofxOscMessage m;
		receiver.getNextMessage( &m );
        game.setData(m);
	}
    
    game.update();
    
    
}


//--------------------------------------------------------------
void testApp::draw(){
    game.draw();
    
    ofPushStyle();
    ofSetColor(255);
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 20,20);
    ofPopStyle();
}
