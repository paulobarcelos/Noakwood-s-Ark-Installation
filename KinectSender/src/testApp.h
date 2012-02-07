/******************************************************************/
/**
 * @file	testApp.h
 * @brief	Example for ofxKinectNui addon
 * @note
 * @todo
 * @bug	
 *
 * @author	sadmb
 * @date	Oct. 28, 2011
 */
/******************************************************************/
#pragma once

#include "ofxKinectNui.h"
#include "ofMain.h"
#include "ofxKinectNuiPlayer.h"
#include "ofxKinectNuiRecorder.h"
#include "Player.h"

#include "ofxOsc.h"


#define HOST "192.168.0.77"
#define PORT 12345

class testApp : public ofBaseApp {
	public:

		void setup();
		void update();
		void draw();

		void exit();

		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void kinectPlugged();
		void kinectUnplugged();


		ofxKinectNui kinect;
		ofxBase3DVideo* kinectSource;

		Player * players;
		ofPoint player1Location;
		ofPoint player2Location;

		ofRectangle player1Target;
		ofRectangle player2Target;

		int	player1SkeletonIndex;
		int	player2SkeletonIndex;

		bool bPlugged;
		bool bUnplugged;
		
		unsigned short nearClipping;
		unsigned short farClipping;
		int angle;
		
		int mRotationX, mRotationY;

		ofxOscSender sender;


		ofImage test;
};
