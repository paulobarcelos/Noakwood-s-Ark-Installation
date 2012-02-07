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

		ofxOscMessage player1ActiveMsg;
		player1ActiveMsg.setAddress( "/player/1/active" );
		if( player1SkeletonIndex != -1 && skeleton[player1SkeletonIndex].TrackingState() == NUI_SKELETON_TRACKED){
			
			player1ActiveMsg.addIntArg( 1 );

			ofxOscMessage p1spine;
			p1spine.setAddress( "/player/1/spine" );
			p1spine.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_SPINE].x );
			p1spine.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_SPINE].y );

			ofxOscMessage p1shoulderCenter;
			p1shoulderCenter.setAddress( "/player/1/shoulderCenter" );
			p1shoulderCenter.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_SHOULDER_CENTER].x );
			p1shoulderCenter.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_SHOULDER_CENTER].y );
			
			ofxOscMessage p1hipCenter;
			p1hipCenter.setAddress( "/player/1/hipCenter" );
			p1hipCenter.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_HIP_CENTER].x );
			p1hipCenter.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_HIP_CENTER].y );

			ofxOscMessage p1shoulderLeft;
			p1shoulderLeft.setAddress( "/player/1/shoulderLeft" );
			p1shoulderLeft.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_SHOULDER_LEFT].x );
			p1shoulderLeft.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_SHOULDER_LEFT].y );

			ofxOscMessage p1hipLeft;
			p1hipLeft.setAddress( "/player/1/hipLeft" );
			p1hipLeft.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_HIP_LEFT].x );
			p1hipLeft.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_HIP_LEFT].y );

			ofxOscMessage p1elbowLeft;
			p1elbowLeft.setAddress( "/player/1/elbowLeft" );
			p1elbowLeft.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_ELBOW_LEFT].x );
			p1elbowLeft.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_ELBOW_LEFT].y );

			ofxOscMessage p1wristLeft;
			p1wristLeft.setAddress( "/player/1/wristLeft" );
			p1wristLeft.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_WRIST_LEFT].x );
			p1wristLeft.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_WRIST_LEFT].y );
			
			ofxOscMessage p1handLeft;
			p1handLeft.setAddress( "/player/1/handLeft" );
			p1handLeft.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_HAND_LEFT].x );
			p1handLeft.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_HAND_LEFT].y );

			ofxOscMessage p1kneeLeft;
			p1kneeLeft.setAddress( "/player/1/kneeLeft" );
			p1kneeLeft.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_KNEE_LEFT].x );
			p1kneeLeft.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_KNEE_LEFT].y );
			
			ofxOscMessage p1ankleLeft;
			p1ankleLeft.setAddress( "/player/1/ankleLeft" );
			p1ankleLeft.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_ANKLE_LEFT].x );
			p1ankleLeft.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_ANKLE_LEFT].y );
			
			ofxOscMessage p1footLeft;
			p1footLeft.setAddress( "/player/1/footLeft" );
			p1footLeft.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_FOOT_LEFT].x );
			p1footLeft.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_FOOT_LEFT].y );

			ofxOscMessage p1shoulderRight;
			p1shoulderRight.setAddress( "/player/1/shoulderRight" );
			p1shoulderRight.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_SHOULDER_RIGHT].x );
			p1shoulderRight.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_SHOULDER_RIGHT].y );

			ofxOscMessage p1hipRight;
			p1hipRight.setAddress( "/player/1/hipRight" );
			p1hipRight.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_HIP_RIGHT].x );
			p1hipRight.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_HIP_RIGHT].y );

			ofxOscMessage p1elbowRight;
			p1elbowRight.setAddress( "/player/1/elbowRight" );
			p1elbowRight.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_ELBOW_RIGHT].x );
			p1elbowRight.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_ELBOW_RIGHT].y );

			ofxOscMessage p1wristRight;
			p1wristRight.setAddress( "/player/1/wristRight" );
			p1wristRight.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_WRIST_RIGHT].x );
			p1wristRight.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_WRIST_RIGHT].y );
			
			ofxOscMessage p1handRight;
			p1handRight.setAddress( "/player/1/handRight" );
			p1handRight.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_HAND_RIGHT].x );
			p1handRight.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_HAND_RIGHT].y );

			ofxOscMessage p1kneeRight;
			p1kneeRight.setAddress( "/player/1/kneeRight" );
			p1kneeRight.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_KNEE_RIGHT].x );
			p1kneeRight.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_KNEE_RIGHT].y );
			
			ofxOscMessage p1ankleRight;
			p1ankleRight.setAddress( "/player/1/ankleRight" );
			p1ankleRight.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_ANKLE_RIGHT].x );
			p1ankleRight.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_ANKLE_RIGHT].y );
			
			ofxOscMessage p1footRight;
			p1footRight.setAddress( "/player/1/footRight" );
			p1footRight.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_FOOT_RIGHT].x );
			p1footRight.addFloatArg( skeletonPoints[player1SkeletonIndex][NUI_SKELETON_POSITION_FOOT_RIGHT].y );


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
			sender.sendMessage(p1footRight);
		}
		else{
			player1ActiveMsg.addIntArg( 0 );
			sender.sendMessage( player1ActiveMsg );
		}

		ofxOscMessage player2ActiveMsg;
		player2ActiveMsg.setAddress( "/player/2/active" );
		if( player2SkeletonIndex != -1 && skeleton[player2SkeletonIndex].TrackingState() == NUI_SKELETON_TRACKED){
			
			player2ActiveMsg.addIntArg( 1 );

			ofxOscMessage p2spine;
			p2spine.setAddress( "/player/2/spine" );
			p2spine.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_SPINE].x );
			p2spine.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_SPINE].y );

			ofxOscMessage p2shoulderCenter;
			p2shoulderCenter.setAddress( "/player/2/shoulderCenter" );
			p2shoulderCenter.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_SHOULDER_CENTER].x );
			p2shoulderCenter.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_SHOULDER_CENTER].y );
			
			ofxOscMessage p2hipCenter;
			p2hipCenter.setAddress( "/player/2/hipCenter" );
			p2hipCenter.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_HIP_CENTER].x );
			p2hipCenter.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_HIP_CENTER].y );

			ofxOscMessage p2shoulderLeft;
			p2shoulderLeft.setAddress( "/player/2/shoulderLeft" );
			p2shoulderLeft.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_SHOULDER_LEFT].x );
			p2shoulderLeft.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_SHOULDER_LEFT].y );

			ofxOscMessage p2hipLeft;
			p2hipLeft.setAddress( "/player/2/hipLeft" );
			p2hipLeft.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_HIP_LEFT].x );
			p2hipLeft.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_HIP_LEFT].y );

			ofxOscMessage p2elbowLeft;
			p2elbowLeft.setAddress( "/player/2/elbowLeft" );
			p2elbowLeft.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_ELBOW_LEFT].x );
			p2elbowLeft.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_ELBOW_LEFT].y );

			ofxOscMessage p2wristLeft;
			p2wristLeft.setAddress( "/player/2/wristLeft" );
			p2wristLeft.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_WRIST_LEFT].x );
			p2wristLeft.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_WRIST_LEFT].y );
			
			ofxOscMessage p2handLeft;
			p2handLeft.setAddress( "/player/2/handLeft" );
			p2handLeft.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_HAND_LEFT].x );
			p2handLeft.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_HAND_LEFT].y );

			ofxOscMessage p2kneeLeft;
			p2kneeLeft.setAddress( "/player/2/kneeLeft" );
			p2kneeLeft.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_KNEE_LEFT].x );
			p2kneeLeft.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_KNEE_LEFT].y );
			
			ofxOscMessage p2ankleLeft;
			p2ankleLeft.setAddress( "/player/2/ankleLeft" );
			p2ankleLeft.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_ANKLE_LEFT].x );
			p2ankleLeft.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_ANKLE_LEFT].y );
			
			ofxOscMessage p2footLeft;
			p2footLeft.setAddress( "/player/2/footLeft" );
			p2footLeft.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_FOOT_LEFT].x );
			p2footLeft.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_FOOT_LEFT].y );

			ofxOscMessage p2shoulderRight;
			p2shoulderRight.setAddress( "/player/2/shoulderRight" );
			p2shoulderRight.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_SHOULDER_RIGHT].x );
			p2shoulderRight.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_SHOULDER_RIGHT].y );

			ofxOscMessage p2hipRight;
			p2hipRight.setAddress( "/player/2/hipRight" );
			p2hipRight.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_HIP_RIGHT].x );
			p2hipRight.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_HIP_RIGHT].y );

			ofxOscMessage p2elbowRight;
			p2elbowRight.setAddress( "/player/2/elbowRight" );
			p2elbowRight.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_ELBOW_RIGHT].x );
			p2elbowRight.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_ELBOW_RIGHT].y );

			ofxOscMessage p2wristRight;
			p2wristRight.setAddress( "/player/2/wristRight" );
			p2wristRight.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_WRIST_RIGHT].x );
			p2wristRight.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_WRIST_RIGHT].y );
			
			ofxOscMessage p2handRight;
			p2handRight.setAddress( "/player/2/handRight" );
			p2handRight.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_HAND_RIGHT].x );
			p2handRight.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_HAND_RIGHT].y );

			ofxOscMessage p2kneeRight;
			p2kneeRight.setAddress( "/player/2/kneeRight" );
			p2kneeRight.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_KNEE_RIGHT].x );
			p2kneeRight.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_KNEE_RIGHT].y );
			
			ofxOscMessage p2ankleRight;
			p2ankleRight.setAddress( "/player/2/ankleRight" );
			p2ankleRight.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_ANKLE_RIGHT].x );
			p2ankleRight.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_ANKLE_RIGHT].y );
			
			ofxOscMessage p2footRight;
			p2footRight.setAddress( "/player/2/footRight" );
			p2footRight.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_FOOT_RIGHT].x );
			p2footRight.addFloatArg( skeletonPoints[player2SkeletonIndex][NUI_SKELETON_POSITION_FOOT_RIGHT].y );


			sender.sendMessage(player2ActiveMsg);
			sender.sendMessage(p2spine);
			sender.sendMessage(p2hipCenter);
			sender.sendMessage(p2shoulderLeft);
			sender.sendMessage(p2elbowLeft);
			sender.sendMessage(p2wristLeft);
			sender.sendMessage(p2handLeft);
			sender.sendMessage(p2hipLeft);
			sender.sendMessage(p2kneeLeft);
			sender.sendMessage(p2ankleLeft);
			sender.sendMessage(p2footLeft);
			sender.sendMessage(p2shoulderRight);
			sender.sendMessage(p2elbowRight);
			sender.sendMessage(p2wristRight);
			sender.sendMessage(p2handRight);
			sender.sendMessage(p2hipRight);
			sender.sendMessage(p2kneeRight);
			sender.sendMessage(p2ankleRight);
			sender.sendMessage(p2footRight);
		}
		else{
			player1ActiveMsg.addIntArg( 0 );
			sender.sendMessage( player1ActiveMsg );
		}
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
