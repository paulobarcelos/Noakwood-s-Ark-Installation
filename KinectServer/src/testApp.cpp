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

	
	kinect.init(false, false, false, true); // enable all captures
	kinect.open();

	player1Target = ofRectangle(0.125f, 0.25f, 0.25f, 0.5f);
	player2Target = ofRectangle(0.125f * 5.f, 0.25f, 0.25f, 0.5f);

	kinect.addKinectListener(this, &testApp::kinectPlugged, &testApp::kinectUnplugged);
	
	kinectSource = &kinect;
	angle = kinect.getCurrentAngle();
	kinect.setAngle(17);
	bPlugged = kinect.isConnected();
	nearClipping = kinect.getNearClippingDistance();
	farClipping = kinect.getFarClippingDistance();
	
	player1SkeletonIndex = -1;
	player2SkeletonIndex = -1;

	sender.setup( HOST, PORT );        

}

//--------------------------------------------------------------
void testApp::update() {
	if(kinect.isConnected()){
		kinectSource->update();
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
					}

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

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HEAD].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HEAD].y );

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SHOULDER_CENTER].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SHOULDER_CENTER].y );
			
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HIP_CENTER].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HIP_CENTER].y );

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SHOULDER_LEFT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SHOULDER_LEFT].y );

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_ELBOW_LEFT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_ELBOW_LEFT].y );

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_WRIST_LEFT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_WRIST_LEFT].y );
			
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HAND_LEFT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HAND_LEFT].y );

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HIP_LEFT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HIP_LEFT].y );

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_KNEE_LEFT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_KNEE_LEFT].y );
			
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_ANKLE_LEFT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_ANKLE_LEFT].y );

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_FOOT_LEFT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_FOOT_LEFT].y );

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SHOULDER_RIGHT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SHOULDER_RIGHT].y );

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_ELBOW_RIGHT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_ELBOW_RIGHT].y );

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_WRIST_RIGHT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_WRIST_RIGHT].y );
			
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HAND_RIGHT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HAND_RIGHT].y );

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HIP_RIGHT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HIP_RIGHT].y );

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_KNEE_RIGHT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_KNEE_RIGHT].y );
			
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_ANKLE_RIGHT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_ANKLE_RIGHT].y );
	
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_FOOT_RIGHT].x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_FOOT_RIGHT].y );	

			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SPINE].x - player1Target.getCenter().x );
			p1Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SPINE].y - player1Target.getCenter().y );
		}
		else{
			p1Message.addIntArg( 0 );
		}
        
        sender.sendMessage(p1Message);


		// Player 2
        index = player2SkeletonIndex;
		ofxOscMessage p2Message;
		p2Message.setAddress( "/player/2" );
		if( index != -1 && skeleton[index].TrackingState() == NUI_SKELETON_TRACKED){
			
			p2Message.addIntArg( 1 );
            
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SPINE].x );
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SPINE].y );

			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HEAD].x );
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HEAD].y );

			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SHOULDER_CENTER].x );
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SHOULDER_CENTER].y );
			
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HIP_CENTER].x );
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HIP_CENTER].y );

			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SHOULDER_LEFT].x );
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SHOULDER_LEFT].y );

			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_ELBOW_LEFT].x );
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_ELBOW_LEFT].y );

			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_WRIST_LEFT].x );
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_WRIST_LEFT].y );
			
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HAND_LEFT].x );
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HAND_LEFT].y );

			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HIP_LEFT].x );
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HIP_LEFT].y );

			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_KNEE_LEFT].x );
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_KNEE_LEFT].y );
			
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_ANKLE_LEFT].x );
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_ANKLE_LEFT].y );

			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_FOOT_LEFT].x );
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_FOOT_LEFT].y );

			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SHOULDER_RIGHT].x );
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SHOULDER_RIGHT].y );

			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_ELBOW_RIGHT].x );
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_ELBOW_RIGHT].y );

			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_WRIST_RIGHT].x );
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_WRIST_RIGHT].y );
			
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HAND_RIGHT].x );
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HAND_RIGHT].y );

			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HIP_RIGHT].x );
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_HIP_RIGHT].y );

			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_KNEE_RIGHT].x );
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_KNEE_RIGHT].y );
			
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_ANKLE_RIGHT].x );
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_ANKLE_RIGHT].y );
	
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_FOOT_RIGHT].x );
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_FOOT_RIGHT].y );	

			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SPINE].x - player2Target.getCenter().x );
			p2Message.addFloatArg( skeletonPoints[index][NUI_SKELETON_POSITION_SPINE].y - player2Target.getCenter().y );
		}
		else{
			p2Message.addIntArg( 0 );
		}
        
        sender.sendMessage(p2Message);
	}
}

//--------------------------------------------------------------
void testApp::draw() {
	if(kinect.isInited()){
		ofEnableAlphaBlending();
		ofSetColor(255);
			//kinect.drawDepth(0, 0, kinect.getDepthResolutionWidth(), kinect.getDepthResolutionHeight());
			//kinect.drawLabel(0, 0, kinect.getDepthResolutionWidth(), kinect.getDepthResolutionHeight());	

		ofDisableAlphaBlending();
	
		ofPushStyle();
			ofSetColor(255,0,0);
			ofNoFill();
			ofRect(player1Target.x * kinect.getDepthResolutionWidth(), player1Target.y * kinect.getDepthResolutionHeight(), player1Target.width * kinect.getDepthResolutionWidth(), player1Target.height * kinect.getDepthResolutionHeight() );
			ofRect(player2Target.x * kinect.getDepthResolutionWidth(), player2Target.y * kinect.getDepthResolutionHeight(), player2Target.width * kinect.getDepthResolutionWidth(), player2Target.height * kinect.getDepthResolutionHeight() );
		ofPopStyle();
	}	

	ofSetColor(0);
	stringstream reportStream;
	reportStream << " (press: < >), fps: " << ofGetFrameRate() << endl
				 << "press 'c' to close the stream and 'o' to open it again, stream is: " << kinect.isOpened() << endl
				 << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl
				 << "press LEFT and RIGHT to change the far clipping distance: " << farClipping << " mm" << endl
				 << "press '+' and '-' to change the near clipping distance: " << nearClipping << " mm" << endl;
	ofDrawBitmapString(reportStream.str(), 20, 652);
	
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
