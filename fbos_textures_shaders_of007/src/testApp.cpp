#include "testApp.h"

/*
 
 this is an example on how to use shaders, fbos, and textures. 
 with this you can apply shaders into fbo and load textures into the shaders. 
 display where and when you want the results. 
 apply a set of different shaders, 
 etc. 
 works in of007, macbook pro atiradeon1600. 
 hope this helps you all....
 http://www.tangiblex.net
 tangiblex@yahoo.com
 
 
 */

//--------------------------------------------------------------
void testApp::setup(){

	ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(34, 34, 34);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();

	
	// using the shader that passes through 10 blending modes a la photoshop. 
	// taken from 
	// http://www.ruudbijnen.nl/blog/2010/09/blending-textures-in-a-shader/
	//
	
	shader2.load("shaders/noise");
	shader.load("shaders/myShader");
	
	image1.loadImage("images/cat512.png");
	image2.loadImage("images/grass512.png");
	
	w = image1.getWidth();
	h = image2.getHeight();
		
	// lets allocate for the fbos. Check this: GL_RGBA....
	fbo1.allocate(512, 512, GL_RGBA);
	fbo2.allocate(512, 512, GL_RGBA);
	fbo3.allocate(512, 512, GL_RGBA);
	
	// some parameters for the shader. 
	 shadeContrast = 0.8;
	 shadeBrightness = 0.4;
	 shadeBlendMix = 0.5;
	// there are 10 diff. blend modes, you can swith them with key '.' and '-'  
	 shadeBlendMode = 0;
	
	// if we want to do the second "pass" in shaders-fbo.
	// switch with key 's'.
	
	doShader = true;	
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetColor(225);
	ofDrawBitmapString("'s' toggles shader number 2", 10, 20);
	ofDrawBitmapString("'.' and '-' to switch between blendmodes", 10, 40);


	ofSetColor(245, 58, 135);
	ofFill();
	
	
	// we begin fbo1 and draw the image. 
	fbo1.begin();
	ofSetColor(255);
	image2.draw(0, 0);
	fbo1.end();
	
	// fbo2: image2. 
	fbo2.begin();
	ofSetColor(255);
	image1.draw(0, 0);
	fbo2.end();
	
	// set tex1 and tex2: textures from fbos. 
	ofTexture tex1 = fbo1.getTextureReference();
	ofTexture tex2 = fbo2.getTextureReference();
		
	fbo3.begin();
	ofClear(0, 0, 0, 1); // we clear the fbo. 
	
	shader.begin(); // shader begin: set values. 
	
	shader.setUniform1f( "contrast",	shadeContrast );
    shader.setUniform1f( "brightness",	shadeBrightness );
    shader.setUniform1f( "blendmix",	shadeBlendMix );
    shader.setUniform1i( "blendmode",	shadeBlendMode );
	
	
	// so here is the trick, important one: 
	// tex1 is the texture from fbo1. 
	// we assign this to texture 0 , i.e. "textBase" in the shader. 
	// this is as drawing into the fbo  image1. (from shader point of view) 
	// so instead of making : 
	// fbo2.draw(0,0) after shader parameters we make this line:  
	shader.setUniformTexture("texBase",   tex1, 0 ); //look that is number 0: and 
													// textures 0 are the ones used do "draw". 
													// so we could do this making fbo1.draw.
	
	// now we have another texture in the shader: it will be number 1: 
    shader.setUniformTexture("texBlend",  tex2, 1 );

	// we did not draw anything into fbo3, so to make the shader draw, we need to make our quad.
	// coord for the texture and for the display. 
	// it could be a non-rectangle shape, or whatever...
	
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
	glTexCoord2f(w, 0); glVertex3f(w, 0, 0);
	glTexCoord2f(w, h); glVertex3f(w, h, 0);
	glTexCoord2f(0,h);  glVertex3f(0,h, 0);
	glEnd();
	
	
	// we do not need this:
	// fbo1.draw(0, 0);	
	// because we did this: 
	// shader.setUniformTexture("texBase",   tex1, 0 ); 
	
	
	shader.end(); // shader's end. 
	
	
	fbo3.end(); // end of the "containing" fbo. 
	
	
	// now we want to apply another shader, taking texture from this previous fbo3: 
	if(doShader){
		
	fbo3.begin();
	
	// texture tex3 == fbo3 (shaded)
	ofTexture tex3 = fbo3.getTextureReference();

	shader.begin();
	
	shader.setUniform1f( "contrast",	0.7 );
    shader.setUniform1f( "brightness",	0.5 );
    shader.setUniform1f( "blendmix",	0.7 );
    shader.setUniform1i( "blendmode",	shadeBlendMode);
	
	shader.setUniformTexture("texBase",   tex1, 0 );  // tex base is the same...
    shader.setUniformTexture("texBlend",  tex3, 1 ); //now the texture to blend is fbo3., so we apply shader 2 times....
	
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
	glTexCoord2f(w, 0); glVertex3f(w, 0, 0);
	glTexCoord2f(w, h); glVertex3f(w, h, 0);
	glTexCoord2f(0,h);  glVertex3f(0,h, 0);
	glEnd();	
	
	//image1.draw(0, 0);	
	
	shader.end(); 
	fbo3.end();
		
		
	}
	
	ofSetColor(255);
	//now we want to see the result: 
	fbo3.draw(0,50);
	
	
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	if( key == 's' ){
		doShader = !doShader;
	}	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
	switch(key){
	case '.':
	shadeBlendMode = MIN(shadeBlendMode + 1, 10);
	break;
	case '-':
	shadeBlendMode = MAX(shadeBlendMode - 1, 0);
	break;
	}
	
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

