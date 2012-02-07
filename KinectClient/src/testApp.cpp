#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	receiver.setup( PORT );
    ofSetVerticalSync(true);
    
    player = new Player();
    player->setup();
    player->position.x = ofGetWidth()/2;
    player->position.y = ofGetHeight()/2;
}

//--------------------------------------------------------------
void testApp::update(){
	while( receiver.hasWaitingMessages() )
	{
		ofxOscMessage m;
		receiver.getNextMessage( &m );
		
		if ( m.getAddress() == "/player/1" ){
			player->update(m);
		}

	}
}


//--------------------------------------------------------------
void testApp::draw(){
  player->draw();
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
