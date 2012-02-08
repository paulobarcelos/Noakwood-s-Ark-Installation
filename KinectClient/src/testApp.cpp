#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	receiver.setup( PORT );
    ofSetVerticalSync(true);
    
    int w = 1024;
    int h = 728;
    ofSetWindowShape(w, h);
    
    box2d.init();
	box2d.setGravity(0, 10);
	box2d.createBounds(0,0,w,h);
	box2d.setFPS(30.0);
    
    player = new Player();
    player->setup(box2d.getWorld());
    player->position.x = w/4;
    player->position.y = h*0.7;
}

//--------------------------------------------------------------
void testApp::update(){
    while( receiver.hasWaitingMessages() )
	{
		ofxOscMessage m;
		receiver.getNextMessage( &m );
		
		if ( m.getAddress() == "/player/1" ){
			player->setData(m);
            test.setPosition(player->elbowLeftToWristLeft.center);

		}

	}
    
    player->update();
    
    box2d.update();
}


//--------------------------------------------------------------
void testApp::draw(){
    player->draw();
    
    for(int i=0; i<circles.size(); i++) {
        ofFill();
		ofSetHexColor(0xf6c738);
		circles[i].draw();
	}
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
    float r = 10;		// a random radius 4px - 20px
    ofxBox2dCircle circle;
    circle.setPhysics(3.0, 0.5, 0.5);
    circle.setup(box2d.getWorld(), mouseX, mouseY, r);
    circles.push_back(circle);
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
