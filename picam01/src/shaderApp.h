#pragma once

#include "ofMain.h"
#include "ofAppEGLWindow.h"
#include "ofxRPiCameraVideoGrabber.h"
#include "OMX_Maps.h"
#include "ofxOsc.h"
#include "ofxOMXPlayer.h"

#define PORT 12345

class shaderApp : public ofBaseApp {

	public:

	void setup();
	void update();
	void draw();

	ofxRPiCameraVideoGrabber videoGrabber;
	bool doDrawInfo;
	
	ofFbo camFbo;
	ofFbo overlayFbo;
	ofShader shader;

	
	ofTrueTypeFont font[2];

	ofxOscReceiver receiver;

	string lineOne;
	string lineTwo;

	ofxOMXPlayer omxPlayer;

	bool bVideoPlaying;

	string currentFilter;
	string currentVideo;
};

