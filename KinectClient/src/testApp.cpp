#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	receiver.setup( PORT );
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    
    int w = 612;
    int h = 768;
    ofSetWindowShape(w, h);
    
    box2d.init();
	box2d.setGravity(0, 10);
	box2d.createBounds(0,0,w,h);
	box2d.setFPS(30.0);
    
    
    // Bear skin
    PlayerSkin bear;
    bear.spineToShoulderCenter.set("bear/spineToShoulderCenter.png", 70);
    bear.spineToHipCenter.set("blank.png",0,0,0,0);
    
    bear.hipCenterToHipLeft.set("blank.png", 0, 0, 0, 30);
    bear.hipCenterToHipRight.set("blank.png", 0, 0, 0, 30);
    
    bear.shoulderCenterToHead.set("bear/shoulderCenterToHead.png", 15);
    bear.shoulderCenterToShoulderLeft.set("blank.png", 0, 0, 0, 60);
    bear.shoulderCenterToShoulderRight.set("blank.png", 0, 0, 0, 60);
    
    bear.shoulderLeftToElbowLeft.set("bear/shoulderLeftToElbowLeft.png", 20);
    bear.elbowLeftToWristLeft.set("bear/elbowLeftToWristLeft.png", 20);
    bear.wristLeftToHandLeft.set("bear/wristLeftToHandLeft.png", 10);
    
    bear.shoulderRightToElbowRight.set("bear/shoulderRightToElbowRight.png", 20);
    bear.elbowRightToWristRight.set("bear/elbowRightToWristRight.png", 20);
    bear.wristRightToHandRight.set("bear/wristRightToHandRight.png", 10);
    
    bear.hipLeftToKneeLeft.set("bear/hipLeftToKneeLeft.png", 20);
    bear.kneeLeftToAnkleLeft.set("bear/kneeLeftToAnkleLeft.png", 20);
    bear.ankleLeftToFootLeft.set("bear/ankleLeftToFootLeft.png", 10);
    
    bear.hipRightToKneeRight.set("bear/hipRightToKneeRight.png", 20);
    bear.kneeRightToAnkleRight.set("bear/kneeRightToAnkleRight.png", 20);
    bear.ankleRightToFootRight.set("bear/ankleRightToFootRight.png",10);
    
    bear.movingLimit.set( 600, (bear.hipLeftToKneeLeft.length + bear.kneeLeftToAnkleLeft.length) * 2 );
    
    player1 = new Player();
    player1->setup(box2d.getWorld(), bear);
    
    player2 = new Player();
    player2->setup(box2d.getWorld(), bear);
    
    // Setup the game
    game1 = new PlayerGame();
    game1->setup(box2d.getWorld(), player1);
    game1->position.x = w/2;
    game1->position.y = h*0.65;
    
    // Create a lot of particles
    for (int i = 0; i < 500; i++) {
        float r = ofRandom(8, 13);
        ofxBox2dCircle circle;
        circle.setPhysics(100, 0.3, 0);
        circle.setup(box2d.getWorld(), ofRandom(0, w), h, r);
        circles.push_back(circle);
    }
    
}

//--------------------------------------------------------------
void testApp::update(){
    while( receiver.hasWaitingMessages() )
	{
		ofxOscMessage m;
		receiver.getNextMessage( &m );
		
		if ( m.getAddress() == "/player/1" ){
			game1->setData(m);
		}else if ( m.getAddress() == "/player/2" ){
			player2->setData(m);
		}
	}
    
    game1->update();
    player2->update();
    
    box2d.update();
}


//--------------------------------------------------------------
void testApp::draw(){
    game1->draw();
    player2->draw();
    
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
    float r = ofRandom(5, 8);		// a random radius 4px - 20px
    ofxBox2dCircle circle;
    circle.setPhysics(10, 0.3, 0);
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
