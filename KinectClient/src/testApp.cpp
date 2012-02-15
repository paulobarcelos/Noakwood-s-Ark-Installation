#include "testApp.h"

void testApp::setup(){
    w = 1024;
    h = 600;    
	
    ofSetVerticalSync(true);
    ofSetFullscreen(true);   
    ofSetWindowShape(w, h);
    ofBackground(0, 0, 0);
    
    receiver.setup( PORT );
    
    game.setup(w,h);
    
    warper.setup(0,0,w,h);
}

void testApp::update(){
    
    while( receiver.hasWaitingMessages() )
	{
		ofxOscMessage m;
		receiver.getNextMessage( &m );
        game.setData(m);
	}
    
    game.update();
}

void testApp::draw(){
    warper.begin();
    game.draw();
    
    // Quick n'dirty mask as FBO didin't wanna play with me tonight
    ofPushStyle();
    ofSetColor(0, 0, 0);
    ofRect(0, -500, w + 500, 500);
    ofRect(w, 0, 500, h + 500);
    ofRect(- 500, h, w + 500, 500);
    ofRect(- 500, -500, 500, h + 500);
    ofPopStyle();
    
    warper.draw();
    warper.end();
}

void testApp::keyPressed(int key){
    switch (key) {
        case ' ':
            ofToggleFullscreen();
            break;
        case 'w':            
            if (warper.isActive()) {
                warper.deactivate();
            }else {
                warper.activate();
            }
            break;
    }
}
