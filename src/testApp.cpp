#include "testApp.h"

void testApp::setup(){
    image.loadImage("bg.png");
    texture.loadImage("particleGrid.png");
    ofDisableArbTex();
    texture.loadImage("particleGrid.png");
    ofEnableAlphaBlending();
    texW = texture.getWidth();
    texH = texture.getHeight();

    cellRows  = 2;
    cellColls = 2;
    ofSetWindowShape(image.getWidth(), image.getHeight());
	kParticles = 8;
    
	float padding = 256;
	float maxVelocity = .5;
    billboards.getVertices().resize(kParticles * 1024);
    billboards.getColors().resize(kParticles * 1024);
    billboards.getTexCoords().resize(kParticles * 1024*4);
    billboards.getNormals().resize(kParticles * 1024,ofVec3f(0));
	for(int i = 0; i < kParticles * 1024; i++) {
		float x = ofRandom(padding, ofGetWidth() - padding);
		float y = ofRandom(padding, ofGetHeight() - padding);
		float xv = ofRandom(-maxVelocity, maxVelocity);
		float yv = ofRandom(-maxVelocity, maxVelocity);
		Particle particle(x, y, xv, yv);
		particleSystem.add(particle);
        billboards.setColor(i, ofColor::fromHsb(0, 0, 255));
        billboards.setNormal(i,ofVec3f(12,0,0));
        setParticleTexCoords(i, (int)ofRandom(0, 2 ), (int)ofRandom(0, 2));

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

    isStart = false;
    

}
void testApp::setParticleTexCoords(int i, float columnID, float rowID) {

    if(i < 0)                               i = 0;
    if(i > kParticles * 1024)   i = kParticles * 1024;

    if(columnID > cellColls) columnID = cellColls;
    if(rowID    > cellRows)  rowID    = cellRows;

    if(columnID < 0) columnID = 0;
    if(rowID < 0)    rowID    = 0;



    // get the cell image width
    float cellWidth  = texW / cellRows;
    float cellHeight = texH / cellColls;

    float row = rowID;
    float col = columnID;

    // P1
    billboards.getTexCoords()[(i*4)+0].x = (cellWidth * row)         / texW;
    billboards.getTexCoords()[(i*4)+0].y = (cellHeight * col)        / texH;

    // P2
    billboards.getTexCoords()[(i*4)+1].x = ((cellWidth * row)  +   cellWidth)    / texW;
    billboards.getTexCoords()[(i*4)+1].y = (cellHeight * col)        / texH;

    // P2
    billboards.getTexCoords()[(i*4)+2].x = ((cellWidth * row) + cellWidth)           / texW;
    billboards.getTexCoords()[(i*4)+2].y = ((cellHeight * col) + cellHeight) / texH;

    // P2
    billboards.getTexCoords()[(i*4)+3].x = (cellWidth * row)         / texW;
    billboards.getTexCoords()[(i*4)+3].y = ((cellHeight * col)+cellHeight)   / texH;
    
    
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
	particleSystem.addAttractionForce(ofGetWidth() / 2, ofGetHeight() / 2, 1500, 0.01);
	particleSystem.addRepulsionForce(mouseX, mouseY, 100, 2);
	particleSystem.update();
    vector<Particle> &particles = particleSystem.getParticles();
    int n = particles.size();
    for(int i = 0; i < n; i++)
    {
        
        billboards.getVertices()[i] = particles[i];
//        if(billboards.getVertices()[i].x>0 && billboards.getVertices()[i].x < image.getWidth() && billboards.getVertices()[i].y >0 && billboards.getVertices()[i].y < image.getHeight())
//        {
//            ofColor c = image.getColor(billboards.getVertices()[i].x, billboards.getVertices()[i].y);
//            if(c.r > 50 && c.g > 50 && c.b > 50 )billboards.setColor(i, c);
//        }
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
