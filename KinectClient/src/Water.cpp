#include "Water.h"

void Water::setup(b2World* world, float width, float height) {
    this->world = world;
    this->width = width;
    this->height = height;
    
    for(int i = 0; i < 800; i ++ ){
        float r = ofRandom(5, 8);
        WaterParticle* particle = new WaterParticle();
        particle->setPhysics(10, 0, 0);
        particle->setup(world, width/2, 2 * height + i -100, r);
              
        particle->setData(new Data());
		Data * data = (Data*)particle->getData();
        data->type = WATER;
        data->label = i;
        
        particles.push_back(particle);
	}
    
    ofFbo::Settings s;
	s.width	= width;
	s.height = height;
	s.numColorbuffers = 2;
    waterFbo.allocate(s);
    blurShader.load("", "blur_frag.glsl");
    thresholdShader.load("", "threshold_frag.glsl");
	
    reset();
}
void Water::reset(){
    for(int i=0; i<particles.size(); i++) {
        inactivateParticle(particles[i]);
	}
}

void Water::update(){
    // add some particles every so often
    if(ofGetFrameNum()% 1 == 0) {
        WaterParticle* particle = getNextParticle();
        if(particle){
            particle->setPosition(width / 2 + ofRandom(-300,300), height - 50 );
            particle->setVelocity(0,0);
        }
    }  
}

void Water::draw(){
    waterFbo.begin();
    ofPushStyle();
    ofEnableAlphaBlending();
    ofClear(0,0,0,0);    
    for(int i=0; i<particles.size(); i++) {	
        particles[i]->draw();
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

WaterParticle* Water::getNextParticle() {
    for(int i=0; i<particles.size(); i++) {
        if(particles[i]->dead){
            particles[i]->dead = false;
            particles[i]->setVelocity(0,0);
            particles[i]->enableGravity(true);
            return particles[i];
        }
	}
    return NULL;
}
void Water::removeParticle(int label) {
	for(std::vector<WaterParticle*>::iterator it = particles.begin(); it != particles.end(); ++it) {
        Data * data = (Data*)(*it)->getData();
        if( data->label == label ){
            inactivateParticle(*it);
            break;
        }
        
    }
}

void Water::inactivateParticle(WaterParticle* particle){
    Data* data = (Data*)particle->getData();
    particle->dead = true;
    particle->setPosition(width/2, 2 * height + data->label * 100);
    particle->setVelocity(0,0);
    particle->enableGravity(false);
}