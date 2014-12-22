#pragma once

#include "ofMain.h"
#include "ParticleSystem.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void mousePressed(int x, int y, int button);

	int kParticles;
	ParticleSystem particleSystem;
    
    ofShader billboardShader;
    ofImage texture;
    
    ofVboMesh billboards;
    ofImage image;
    bool isStart;
    
    void setParticleTexCoords(int i, float columnID, float rowID);
    void setParticleColor(int i, float r, float g, float b, float a) ;
    void setParticlePos(int i, float px, float py, float pz) ;
    int                     cellRows;
    int                     cellColls;
    float           texW, texH;
    float *dim;
};
