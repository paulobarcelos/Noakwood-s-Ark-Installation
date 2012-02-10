#pragma once

#include "ofMain.h"

class testApp : public ofBaseApp{
	
	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased (int key);

		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofTrueTypeFont font;
		ofShader shader;
		bool doShader;
	ofShader shader2;
	ofImage image1, image2;
	ofFbo fbo1, fbo2, fbo3;
	float shadeContrast;
	float shadeBrightness;
	float shadeBlendMix;
	int shadeBlendMode;
	
	float w, h;	
};

