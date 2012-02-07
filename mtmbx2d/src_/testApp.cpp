#include "testApp.h"
#include "stdio.h"

//--------------------------------------------------------------
void testApp::setup() {
	
	
	ofSetVerticalSync(true);
	ofBackground(255, 255, 255);
	ofSetLogLevel(OF_LOG_NOTICE);
	
	bDrawLines  = false;
	bMouseForce = false;
	
	box2d.init();
	box2d.setGravity(0, 10);
	box2d.createFloor();
	//box2d.checkBounds(true);
	box2d.setFPS(30.0);
	
	
	// lets draw a simple wave
	ofPoint p;
	float y = 0;
	lineStrip.setWorld(box2d.getWorld());
	lineStrip.clear();
	for(int i=0; i<50; i++) {
		p.x += 18;
		p.y += cos(y)*ofRandom(10, 40);
		y += i * ofRandom(10, 30);

		lineStrip.addPoint(30+p.x, (ofGetHeight()/2)+p.y);
	}
	lineStrip.createShape();
	
}

//--------------------------------------------------------------
void testApp::update() {
	
	
	
	box2d.update();
	
	if(bMouseForce) {
		float strength = 8.0f;
		float damping  = 0.7f;
		
		for(int i=0; i<customParticles.size(); i++) {
			//customParticles[i].addAttractionPoint(mouseX, mouseY, strength);
			//customParticles[i].addDamping(damping, damping);
		}
		
	}
	
}


//--------------------------------------------------------------
void testApp::draw() {
	
	
	for(int i=0; i<customParticles.size(); i++) {
		customParticles[i].draw();
	}
	
	lineStrip.draw();
	box2d.draw();
	
	px = mouseX;
	py = mouseY;
	
	
	string info = "";
	info += "Press [s] to draw a line strip ["+ofToString(bDrawLines)+"]\n"; 
	info += "Press [f] to toggle Mouse Force ["+ofToString(bMouseForce)+"]\n"; 
	info += "Press [z] for custom particle\n";
	info += "Total Bobies: "+ofToString(box2d.getBodyCount())+"\n";
	info += "FPS: "+ofToString(ofGetFrameRate())+"\n";
	ofSetColor(0, 0, 0);
	ofDrawBitmapString(info, 30, 30);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	
	
	if(key == 'z') {
		float r = ofRandom(3, 10);		// a random radius 4px - 20px
		CustomParticle p;
		p.setPhysics(0.4, 0.1, 0.9);
		p.setup(box2d.getWorld(), mouseX, mouseY, r);
		p.color.r = ofRandom(20, 100);
		p.color.g = 0;//ofRandom(0, 255);
		p.color.b = ofRandom(150, 255);
		customParticles.push_back(p);
	}	
	
	
	if(key == 's') bDrawLines = !bDrawLines;
	if(key == 'f') bMouseForce = !bMouseForce;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
	
	if(bDrawLines) {
		lineStrip.addPoint(x, y);
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	
	if(bDrawLines) {
		lineStrip.setWorld(box2d.getWorld());
		lineStrip.clear();
	}
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	if(bDrawLines) lineStrip.createShape();
	
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){
	
}

