#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	receiver.setup( PORT );

	ofBackground( 30, 30, 130 );

}

//--------------------------------------------------------------
void testApp::update(){

	// check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage( &m );
		
		
		// check for mouse moved message
		if ( m.getAddress() == "/player/1/active" )
		{
			if( m.getArgAsInt32( 0 ) == 1 ){
				cout << "player 1 active" << endl;
			}
			else {
				cout << "player 1 inactive" << endl;
			}

		}
		

	}
}


//--------------------------------------------------------------
void testApp::draw(){



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
