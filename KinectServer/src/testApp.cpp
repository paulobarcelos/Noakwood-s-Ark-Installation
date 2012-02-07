/******************************************************************/
/**
 * @file	testApp.cpp
 * @brief	Example for ofxKinectNui addon
 * @note
 * @todo
 * @bug	
 *
 * @author	sadmb
 * @date	Oct. 28, 2011
 */	
/******************************************************************/
#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(60);
	ofSetVerticalSync(true);

	player1Location = ofPoint(0,0);
	
	kinect.init(false, true, true, true); // enable all captures
	kinect.open();
	player1Location = ofPoint( kinect.getDepthResolutionWidth()/4, kinect.getDepthResolutionHeight()/2);

	player1Target = ofRectangle(0.125f, 0.25f, 0.25f, 0.5f);
	player2Target = ofRectangle(0.125f * 5.f, 0.25f, 0.25f, 0.5f);

	kinect.addKinectListener(this, &testApp::kinectPlugged, &testApp::kinectUnplugged);
	
	kinectSource = &kinect;
	angle = kinect.getCurrentAngle();
	kinect.setAngle(20);
	bPlugged = kinect.isConnected();
	nearClipping = kinect.getNearClippingDistance();
	farClipping = kinect.getFarClippingDistance();
	
	players = new Player[kinect::nui::SkeletonFrame::SKELETON_COUNT];
	for(int i = 0; i < kinect::nui::SkeletonFrame::SKELETON_COUNT; i++){
		players[i].setup();
	}

	player1SkeletonIndex = -1;
	player2SkeletonIndex = -1;

	sender.setup( HOST, PORT );

}

//--------------------------------------------------------------
void testApp::update() {
	kinectSource->update();

	ofxOscMessage m;
		m.setAddress( "/cu" );
		m.addFloatArg(5.f);
		sender.sendMessage(m);
}

//--------------------------------------------------------------
void testApp::draw() {
	if(kinect.isInited()){
		ofEnableAlphaBlending();
			kinect.drawDepth(0, 0, kinect.getDepthResolutionWidth(), kinect.getDepthResolutionHeight());
			kinect.drawLabel(0, 0, kinect.getDepthResolutionWidth(), kinect.getDepthResolutionHeight());	
		ofDisableAlphaBlending();
	
		ofPushStyle();
			ofSetColor(255,0,0);
			ofNoFill();
			ofRect(player1Target.x * kinect.getDepthResolutionWidth(), player1Target.y * kinect.getDepthResolutionHeight(), player1Target.width * kinect.getDepthResolutionWidth(), player1Target.height * kinect.getDepthResolutionHeight() );
			ofRect(player2Target.x * kinect.getDepthResolutionWidth(), player2Target.y * kinect.getDepthResolutionHeight(), player2Target.width * kinect.getDepthResolutionWidth(), player2Target.height * kinect.getDepthResolutionHeight() );
		ofPopStyle();
	}	

	if(kinect.isInited() && kinect.grabsSkeleton()){
		ofPoint** skeletonPoints = kinect.getSkeletonPoints();

		// Get the skeleton data of next frame
		kinect::nui::SkeletonFrame skeleton = kinect.kinect.Skeleton().GetNextFrame();
		if(skeleton.IsFoundSkeleton()){
			skeleton.TransformSmooth();
			for(int i = 0; i < kinect::nui::SkeletonFrame::SKELETON_COUNT; i++){
				ofPoint* playerPoints = new ofPoint[kinect::nui::SkeletonData::POSITION_COUNT];

				if( skeleton[i].TrackingState() == NUI_SKELETON_TRACKED){

					for(int j = 0; j < kinect::nui::SkeletonData::POSITION_COUNT; j++){
						kinect::nui::SkeletonData::Point p = skeleton[i].TransformSkeletonToDepthImage(j);
						skeletonPoints[i][j] = ofPoint(p.x, p.y, p.depth);
						playerPoints[j] = skeletonPoints[i][j];
					}

					// check if spine is inside the player area
					if( player1Target.inside( playerPoints[NUI_SKELETON_POSITION_SPINE] ) ){
						player1SkeletonIndex = i;
					}

					if( i != player1SkeletonIndex && player2Target.inside( playerPoints[NUI_SKELETON_POSITION_SPINE] ) ){
						player2SkeletonIndex = i;
						break;
					}



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
		}

        int index;
        
        // Player 1
        index = player1SkeletonIndex;
		ofxOscMessage p1Message;
		p1Message.setAddress( "/player/1" );
		if( index != -1 && skeleton[index].TrackingState() == NUI_SKELETON_TRACKED){
			
			p1Message.addIntArg( 1 );
            
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SPINE].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SPINE].y );

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SHOULDER_CENTER].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SHOULDER_CENTER].y );
			
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HIP_CENTER].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HIP_CENTER].y );

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SHOULDER_LEFT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SHOULDER_LEFT].y );

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HIP_LEFT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HIP_LEFT].y );

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_ELBOW_LEFT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_ELBOW_LEFT].y );

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_WRIST_LEFT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_WRIST_LEFT].y );
			
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HAND_LEFT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HAND_LEFT].y );

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_KNEE_LEFT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_KNEE_LEFT].y );
			
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_ANKLE_LEFT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_ANKLE_LEFT].y );

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_FOOT_LEFT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_FOOT_LEFT].y );

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SHOULDER_RIGHT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SHOULDER_RIGHT].y );

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HIP_RIGHT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HIP_RIGHT].y );

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_ELBOW_RIGHT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_ELBOW_RIGHT].y );

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_WRIST_RIGHT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_WRIST_RIGHT].y );
			
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HAND_RIGHT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HAND_RIGHT].y );

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_KNEE_RIGHT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_KNEE_RIGHT].y );
			
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_ANKLE_RIGHT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_ANKLE_RIGHT].y );
	
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_FOOT_RIGHT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_FOOT_RIGHT].y );		
			
		}
		else{
			p1Message.addIntArg( 0 );
		}
        
        sender.sendMessage(p1Message);

	
	}
	
}


//--------------------------------------------------------------
void testApp::exit() {
	kinect.setAngle(0);
	kinect.close();
	kinect.removeKinectListener(this);

}

//--------------------------------------------------------------
void testApp::keyPressed (int key) {
	switch(key){
	
	case 'o':
	case 'O':
		kinect.open();
		break;
	case 'c':
	case 'C':
		kinect.close();
		break;
	case OF_KEY_UP:
		angle++;
		if(angle > 27){
			angle = 27;
		}
		kinect.setAngle(angle);
		break;
	case OF_KEY_DOWN:
		angle--;
		if(angle < -27){
			angle = -27;
		}
		kinect.setAngle(angle);
		break;
	case OF_KEY_LEFT:
		if(farClipping > nearClipping + 10){
			farClipping -= 10;
			kinectSource->setFarClippingDistance(farClipping);
		}
		break;
	case OF_KEY_RIGHT:
		if(farClipping < 4000){
			farClipping += 10;
			kinectSource->setFarClippingDistance(farClipping);
		}
		break;
	case '-':
		if(nearClipping >= 10){
			nearClipping -= 10;
			kinectSource->setNearClippingDistance(nearClipping);
		}
		break;
	case '+':
		if(nearClipping < farClipping - 10){
			nearClipping += 10;
			kinectSource->setNearClippingDistance(nearClipping);
		}
		break;
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {
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
void testApp::kinectPlugged(){
	
}

//--------------------------------------------------------------
void testApp::kinectUnplugged(){
	bPlugged = false;
}
