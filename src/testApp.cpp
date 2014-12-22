#include "testApp.h"

void testApp::setup(){
    image.loadImage("bg.png");
    ofSetWindowShape(image.getWidth(), image.getHeight());
	kParticles = 8;
	float padding = 256;
	float maxVelocity = .5;
    billboards.getVertices().resize(kParticles * 1024*4);
    billboards.getColors().resize(kParticles * 1024*4);
    billboards.getTexCoords().resize(kParticles * 1024*4);
    billboards.getNormals().resize(kParticles * 1024,ofVec3f(0));
    dim = new float[kParticles * 1024];
	for(int i = 0; i < kParticles * 1024 ; i++) {
		float x = ofRandom(padding, ofGetWidth() - padding);
		float y = ofRandom(padding, ofGetHeight() - padding);
		float xv = ofRandom(-maxVelocity, maxVelocity);
		float yv = ofRandom(-maxVelocity, maxVelocity);
        dim[i] = ofRandom(12.0, 30.0);
		Particle particle(x, y, xv, yv);
		particleSystem.add(particle);
//        billboards.setColor(i, ofColor::fromHsb(ofRandom(96, 160), 255, 255));
        billboards.setNormal(i,ofVec3f(12,0,0));
        setParticleTexCoords(i, (int)ofRandom(0, 3), (int)ofRandom(0, 2));
        setParticleColor(i, 255,255,255,255);
        setParticlePos(i,x, y,0);
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
    texture.loadImage("particleGrid.png");
    
    texW = texture.getWidth();
    texH = texture.getHeight();
    
    cellRows  = 2;
    cellColls = 2;
    
    ofEnableAlphaBlending();
    isStart = false;

}
// ------------------------------------------ set texture coords
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
    billboards.getTexCoords()[(i*4)+1].x = ((cellWidth * row)        + cellWidth)    / texW;
    billboards.getTexCoords()[(i*4)+1].y = (cellHeight * col)        / texH;
    
    // P2
    billboards.getTexCoords()[(i*4)+2].x = ((cellWidth * row) + cellWidth)           / texW;
    billboards.getTexCoords()[(i*4)+2].y = ((cellHeight * col) + cellHeight) / texH;
    
    // P2
    billboards.getTexCoords()[(i*4)+3].x = (cellWidth * row)         / texW;
    billboards.getTexCoords()[(i*4)+3].y = ((cellHeight * col)+cellHeight)   / texH;
    
    
}

// ------------------------------------------ set color
void testApp::setParticleColor(int i, float r, float g, float b, float a) {
    
    
    if(i < 0) i = 0;
    if(i > kParticles * 1024) i = kParticles * 1024;
    
    
    // Color 1
    billboards.getColors()[(i*4)+0].r = r;
    billboards.getColors()[(i*4)+0].g = g;
    billboards.getColors()[(i*4)+0].b = b;
    billboards.getColors()[(i*4)+0].a = a;
    
    // Color 2
    billboards.getColors()[(i*4)+1].r = r;
    billboards.getColors()[(i*4)+1].g = g;
    billboards.getColors()[(i*4)+1].b = b;
    billboards.getColors()[(i*4)+1].a = a;
    
    // Color 3
    billboards.getColors()[(i*4)+2].r = r;
    billboards.getColors()[(i*4)+2].g = g;
    billboards.getColors()[(i*4)+2].b = b;
    billboards.getColors()[(i*4)+2].a = a;
    
    // Color 4
    billboards.getColors()[(i*4)+3].r = r;
    billboards.getColors()[(i*4)+3].g = g;
    billboards.getColors()[(i*4)+3].b = b;
    billboards.getColors()[(i*4)+3].a = a;
    
}

// ------------------------------------------ set position
void testApp::setParticlePos(int i, float px, float py, float pz) {
    
    if(i < 0)                               i = 0;
    if(i > kParticles * 1024)   i = kParticles * 1024;
    
    
    // P1
    billboards.getVertices()[(i*4)+0].x = px;
    billboards.getVertices()[(i*4)+0].y = py;
    billboards.getVertices()[(i*4)+0].z = pz;
    
    // P2
    billboards.getVertices()[(i*4)+1].x = px + dim[i];
    billboards.getVertices()[(i*4)+1].y = py;
    billboards.getVertices()[(i*4)+1].z = pz;
    
    // P2
    billboards.getVertices()[(i*4)+2].x = px + dim[i];
    billboards.getVertices()[(i*4)+2].y = py + dim[i];
    billboards.getVertices()[(i*4)+2].z = pz;
    
    // P2
    billboards.getVertices()[(i*4)+3].x = px;
    billboards.getVertices()[(i*4)+3].y = py + dim[i];
    billboards.getVertices()[(i*4)+3].z = pz;
    
    
    
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
	particleSystem.addAttractionForce(ofGetWidth() / 2, ofGetHeight() / 2 , 1500, 0.01);
	particleSystem.addRepulsionForce(mouseX, mouseY, 100, 2);
	particleSystem.update();
    vector<Particle> &particles = particleSystem.getParticles();
    int n = particles.size();
    for(int i = 0; i < n; i++)
    {
        
//        billboards.getVertices()[i] = particles[i];
        setParticlePos(i,particles[i].x,particles[i].y,particles[i].z);
        if(particles[i].x>0 && particles[i].x < image.getWidth() && particles[i].y >0 && particles[i].y < image.getHeight())
        {
            ofColor c = image.getColor(particles[i].x, particles[i].y);
            if(c.r > 50 && c.g > 50 && c.b > 50 )setParticleColor(i, c.r, c.g, c.b, c.a);
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
