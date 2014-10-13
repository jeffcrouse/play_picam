#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ConsoleListener.h"
#include "ofxRPiCameraVideoGrabber.h"
#include "ImageFilterCollection.h"

#define PORT 12345

class ofApp : public ofBaseApp, public SSHKeyListener{
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofTrueTypeFont font;
		ofxOscReceiver receiver;

		string current_msg_string;

		void onCharacterReceived(SSHKeyListenerEventData& e);
		ConsoleListener consoleListener;
		ofxRPiCameraVideoGrabber videoGrabber;

		OMXCameraSettings omxCameraSettings;
		ImageFilterCollection filterCollection;
};
