#include "testApp.h"

void testApp::setup(){
    image.loadImage("bg.png");
    ofSetWindowShape(image.getWidth(), image.getHeight());
	kParticles = 8;
	float padding = 256;
	float maxVelocity = .5;
    billboards.getVertices().resize(kParticles * 1024);
    billboards.getColors().resize(kParticles * 1024);
    billboards.getNormals().resize(kParticles * 1024,ofVec3f(0));
	for(int i = 0; i < kParticles * 1024; i++) {
		float x = ofRandom(padding, ofGetWidth() - padding);
		float y = ofRandom(padding, ofGetHeight() - padding);
		float xv = ofRandom(-maxVelocity, maxVelocity);
		float yv = ofRandom(-maxVelocity, maxVelocity);
		Particle particle(x, y, xv, yv);
		particleSystem.add(particle);
        billboards.setColor(i, ofColor::fromHsb(ofRandom(96, 160), 255, 255));
        billboards.setNormal(i,ofVec3f(12,0,0));

	}

	particleSystem.setTimeStep(1);

    
    
    
	ofBackground(0, 0, 0);
    
    billboards.setUsage( GL_DYNAMIC_DRAW );
    billboards.setMode(OF_PRIMITIVE_POINTS);
    //billboardVbo.setVertexData(billboardVerts, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
    //billboardVbo.setColorData(billboardColor, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
    if(ofGetGLProgrammableRenderer()){
        billboardShader.load("shadersGL3/Billboard");
    }else{
        billboardShader.load("shadersGL2/Billboard");
    }
    ofDisableArbTex();
    texture.loadImage(".png");
    ofEnableAlphaBlending();
    isStart = false;

}

void testApp::update(){
    if(!isStart)
    {
        return;
    }
	particleSystem.setupForces();

	// apply per-particle forces
	for(int i = 0; i < particleSystem.size(); i++) {
		Particle& cur = particleSystem[i];
		// global force on other particles
		particleSystem.addRepulsionForce(cur, 3, 1);
		// forces on this particle
		cur.bounceOffWalls(0, 0, ofGetWidth(), ofGetHeight());
		cur.addDampingForce();
	}
	// single global forces
	particleSystem.addAttractionForce(ofGetWidth() / 2, 0, 1500, 0.01);
	particleSystem.addRepulsionForce(mouseX, mouseY, 100, 2);
	particleSystem.update();
    vector<Particle> &particles = particleSystem.getParticles();
    int n = particles.size();
    for(int i = 0; i < n; i++)
    {
        
        billboards.getVertices()[i] = particles[i];
        if(billboards.getVertices()[i].x>0 && billboards.getVertices()[i].x < image.getWidth() && billboards.getVertices()[i].y >0 && billboards.getVertices()[i].y < image.getHeight())
        {
            ofColor c = image.getColor(billboards.getVertices()[i].x, billboards.getVertices()[i].y);
            if(c.r > 50 && c.g > 50 && c.b > 50 )billboards.setColor(i, c);
        }
    }
}

void testApp::draw(){
    
    if(!isStart)
    {
        return;
    }
	ofSetColor(255, 255, 255);
	ofFill();
//	particleSystem.draw();
    
    billboardShader.begin();
    
    ofEnablePointSprites(); // not needed for GL3/4
    texture.getTextureReference().bind();
    billboards.draw();
    texture.getTextureReference().unbind();
    ofDisablePointSprites(); // not needed for GL3/4
    
    billboardShader.end();
    
	ofDrawBitmapString(ofToString(kParticles) + "k particles", 32, 32);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", 32, 52);
}

void testApp::keyPressed(int key){
	if(key == 'p')
		ofSaveFrame();
    if(key == ' ')
    {
        isStart = !isStart;
    }
}

void testApp::mousePressed(int x, int y, int button){
}
