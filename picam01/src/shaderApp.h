#pragma once

#include "ofMain.h"
#include "ofAppEGLWindow.h"
#include "ofxRPiCameraVideoGrabber.h"
#include "OMX_Maps.h"
#include "ofxOsc.h"

#define PORT 12345

struct TextLine {
	string text;
	ofColor color;
};

class shaderApp : public ofBaseApp {

	public:

		void setup();
		void update();
		void draw();
		void keyPressed(int key);

	ofxRPiCameraVideoGrabber videoGrabber;
	bool doDrawInfo;
	
	ofFbo fbo;
	ofShader shader;
	string currentFilter;
	OMXCameraSettings omxCameraSettings;
	ofTrueTypeFont font[2];

	ofxOscReceiver receiver;

	string lineOne;
	string lineTwo;

	ofVideoPlayer vid;
};

