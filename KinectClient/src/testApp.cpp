#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	receiver.setup( PORT );
    ofSetVerticalSync(true);
    
    player = new Player();
    player->setup();
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
				
			}
		}
        
        /*
         sender.sendMessage(player1ActiveMsg);
         sender.sendMessage(p1spine);
         sender.sendMessage(p1hipCenter);
         sender.sendMessage(p1shoulderLeft);
         sender.sendMessage(p1elbowLeft);
         sender.sendMessage(p1wristLeft);
         sender.sendMessage(p1handLeft);
         sender.sendMessage(p1hipLeft);
         sender.sendMessage(p1kneeLeft);
         sender.sendMessage(p1ankleLeft);
         sender.sendMessage(p1footLeft);
         sender.sendMessage(p1shoulderRight);
         sender.sendMessage(p1elbowRight);
         sender.sendMessage(p1wristRight);
         sender.sendMessage(p1handRight);
         sender.sendMessage(p1hipRight);
         sender.sendMessage(p1kneeRight);
         sender.sendMessage(p1ankleRight);
         sender.sendMessage(p1footRight);*/
        
        /*ofPoint center;
         float playerLowerHeight = players[i].hipLeftToKneeLeft.length + players[i].kneeLeftToAnkleLeft.length;
         
         center.set(ofGetWidth()/2, ofGetHeight()/3 + (playerPoints[NUI_SKELETON_POSITION_SPINE].y * 2) * playerLowerHeight);
         
         players[i].calculateLimb(players[i].spineToShoulderCenter, center, playerPoints[NUI_SKELETON_POSITION_SPINE], playerPoints[NUI_SKELETON_POSITION_SHOULDER_CENTER]);
         players[i].calculateLimb(players[i].spineToHipCenter, center, playerPoints[NUI_SKELETON_POSITION_SPINE], playerPoints[NUI_SKELETON_POSITION_HIP_CENTER]);
         
         players[i].calculateLimb(players[i].hipCenterToHipLeft, players[i].spineToHipCenter.end, playerPoints[NUI_SKELETON_POSITION_HIP_CENTER], playerPoints[NUI_SKELETON_POSITION_HIP_LEFT]);
         players[i].calculateLimb(players[i].hipCenterToHipRight, players[i].spineToHipCenter.end, playerPoints[NUI_SKELETON_POSITION_HIP_CENTER], playerPoints[NUI_SKELETON_POSITION_HIP_RIGHT]);
         
         players[i].calculateLimb(players[i].shoulderCenterToHead, players[i].spineToShoulderCenter.end, playerPoints[NUI_SKELETON_POSITION_SHOULDER_CENTER], playerPoints[NUI_SKELETON_POSITION_HEAD]);
         players[i].calculateLimb(players[i].shoulderCenterToShoulderLeft, players[i].spineToShoulderCenter.end, playerPoints[NUI_SKELETON_POSITION_SHOULDER_CENTER], playerPoints[NUI_SKELETON_POSITION_SHOULDER_LEFT]);
         players[i].calculateLimb(players[i].shoulderCenterToShoulderRight, players[i].spineToShoulderCenter.end, playerPoints[NUI_SKELETON_POSITION_SHOULDER_CENTER], playerPoints[NUI_SKELETON_POSITION_SHOULDER_RIGHT]);
         
         players[i].calculateLimb(players[i].shoulderLeftToElbowLeft, players[i].shoulderCenterToShoulderLeft.end, playerPoints[NUI_SKELETON_POSITION_SHOULDER_LEFT], playerPoints[NUI_SKELETON_POSITION_ELBOW_LEFT]);
         players[i].calculateLimb(players[i].elbowLeftToWristLeft, players[i].shoulderLeftToElbowLeft.end, playerPoints[NUI_SKELETON_POSITION_ELBOW_LEFT], playerPoints[NUI_SKELETON_POSITION_WRIST_LEFT]);
         players[i].calculateLimb(players[i].wristLeftToHandLeft, players[i].elbowLeftToWristLeft.end, playerPoints[NUI_SKELETON_POSITION_WRIST_LEFT], playerPoints[NUI_SKELETON_POSITION_HAND_LEFT]);
         
         players[i].calculateLimb(players[i].shoulderRightToElbowRight, players[i].shoulderCenterToShoulderRight.end, playerPoints[NUI_SKELETON_POSITION_SHOULDER_RIGHT], playerPoints[NUI_SKELETON_POSITION_ELBOW_RIGHT]);
         players[i].calculateLimb(players[i].elbowRightToWristRight, players[i].shoulderRightToElbowRight.end, playerPoints[NUI_SKELETON_POSITION_ELBOW_RIGHT], playerPoints[NUI_SKELETON_POSITION_WRIST_RIGHT]);
         players[i].calculateLimb(players[i].wristRightToHandRight, players[i].elbowRightToWristRight.end, playerPoints[NUI_SKELETON_POSITION_WRIST_RIGHT], playerPoints[NUI_SKELETON_POSITION_HAND_RIGHT]);
         
         players[i].calculateLimb(players[i].hipLeftToKneeLeft, players[i].hipCenterToHipLeft.end, playerPoints[NUI_SKELETON_POSITION_HIP_LEFT], playerPoints[NUI_SKELETON_POSITION_KNEE_LEFT]);
         players[i].calculateLimb(players[i].kneeLeftToAnkleLeft, players[i].hipLeftToKneeLeft.end, playerPoints[NUI_SKELETON_POSITION_KNEE_LEFT], playerPoints[NUI_SKELETON_POSITION_ANKLE_LEFT]);
         players[i].calculateLimb(players[i].ankleLeftToFootLeft, players[i].kneeLeftToAnkleLeft.end, playerPoints[NUI_SKELETON_POSITION_ANKLE_LEFT], playerPoints[NUI_SKELETON_POSITION_FOOT_LEFT]);
         
         players[i].calculateLimb(players[i].hipRightToKneeRight, players[i].hipCenterToHipRight.end, playerPoints[NUI_SKELETON_POSITION_HIP_RIGHT], playerPoints[NUI_SKELETON_POSITION_KNEE_RIGHT]);
         players[i].calculateLimb(players[i].kneeRightToAnkleRight, players[i].hipRightToKneeRight.end, playerPoints[NUI_SKELETON_POSITION_KNEE_RIGHT], playerPoints[NUI_SKELETON_POSITION_ANKLE_RIGHT]);
         players[i].calculateLimb(players[i].ankleRightToFootRight, players[i].kneeRightToAnkleRight.end, playerPoints[NUI_SKELETON_POSITION_ANKLE_RIGHT], playerPoints[NUI_SKELETON_POSITION_FOOT_RIGHT]);
         
         
         players[i].drawLimb(players[i].spineToShoulderCenter);
         players[i].drawLimb(players[i].spineToHipCenter);
         
         players[i].drawLimb(players[i].hipCenterToHipLeft);
         players[i].drawLimb(players[i].hipCenterToHipRight);
         
         players[i].drawLimb(players[i].shoulderCenterToHead);
         players[i].drawLimb(players[i].shoulderCenterToShoulderLeft);
         players[i].drawLimb(players[i].shoulderCenterToShoulderRight);
         
         players[i].drawLimb(players[i].shoulderLeftToElbowLeft);
         players[i].drawLimb(players[i].elbowLeftToWristLeft);
         players[i].drawLimb(players[i].wristLeftToHandLeft);
         
         players[i].drawLimb(players[i].shoulderRightToElbowRight);
         players[i].drawLimb(players[i].elbowRightToWristRight);
         players[i].drawLimb(players[i].wristRightToHandRight);
         
         players[i].drawLimb(players[i].hipLeftToKneeLeft);
         players[i].drawLimb(players[i].kneeLeftToAnkleLeft);
         players[i].drawLimb(players[i].ankleLeftToFootLeft);
         
         players[i].drawLimb(players[i].hipRightToKneeRight);
         players[i].drawLimb(players[i].kneeRightToAnkleRight);
         players[i].drawLimb(players[i].ankleRightToFootRight);*/
		

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
