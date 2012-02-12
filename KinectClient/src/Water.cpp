#include "Water.h"

void Water::setup(b2World* world, float width, float height) {
    this->world = world;
    this->width = width;
    this->height = height;
    
    for(int i = 0; i < 800; i ++ ){
        float r = ofRandom(5, 8);
        WaterParticle circle;
        circle.setPhysics(10, 0, 0);
        circle.setup(world, i * -100, -100, r);
        circle.dead = true;
        circles.push_back(circle);
        
        circle.setData(new Data());
		Data * data = (Data*)circle.getData();
        data->type = WATER;
		data->isActive = false;
        data->label = i;
		
	}
    
    ofFbo::Settings s;
	s.width	= width;
	s.height = height;
	s.numColorbuffers = 2;
    waterFbo.allocate(s);
    blurShader.load("", "blur_frag.glsl");
    thresholdShader.load("", "threshold_frag.glsl");
	
    
}
void Water::update(){
    // add some circles every so often
    if(ofGetFrameNum()% 1 == 0) {
        WaterParticle* circlePtr = getNextCircle();
        if(circlePtr){
            circlePtr->setPosition(width / 2 + ofRandom(-300,300), height - 70 );
        }
    }  
}

void Water::draw(){
    waterFbo.begin();
    ofPushStyle();
    ofEnableAlphaBlending();
    ofClear(0,0,0,0);    
    for(int i=0; i<circles.size(); i++) {	
        circles[i].draw();
    }
    ofPopStyle();
    waterFbo.end();    
    
    waterFbo.begin();    
    blurShader.begin(); 
    glColor3f(1, 1, 1);    
    for(int i=0; i<4; i++) {
        int srcPos = i % 2;				// attachment to write to
        int dstPos = 1 - srcPos;		// attachment to read from
        glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + dstPos);	// write to this texture
        ofClear(0, 0, 0, 0);
        
        blurShader.setUniform1i("tex0", 0);
        blurShader.setUniform1f("sampleOffset", i*2+1);
        waterFbo.getTextureReference(srcPos).draw(0, 0, width, height);
    }
    blurShader.end();
    
    waterFbo.end();
    
    thresholdShader.begin();
    thresholdShader.setUniform1i("tex0", 0);
    thresholdShader.setUniform1f("brightPassThreshold", 0.5);
    waterFbo.draw(0,0, width, height);
    thresholdShader.end();
}

WaterParticle* Water::getNextCircle() {
    for(int i=0; i<circles.size(); i++) {
        if(circles[i].dead){
            circles[i].dead = false;
            circles[i].setVelocity(0,0);
            circles[i].enableGravity(true);
            return &(circles[i]);
        }
	}
    return NULL;
}
void Water::removeCircle(int label) {
	for(std::vector<WaterParticle>::iterator it = circles.begin(); it != circles.end(); ++it) {
        Data * data = (Data*)(*it).getData();
        if( data->label == label ){
            (*it).dead = true;
            (*it).setPosition(label * -100, -100);
            (*it).setVelocity(0,0);
            (*it).enableGravity(false);
            break;
        }
        
    }
}