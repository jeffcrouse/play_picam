#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetLogLevel("ofThread", OF_LOG_SILENT);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();


	consoleListener.setup(this);

	omxCameraSettings.width = 1280;
	omxCameraSettings.height = 720;
	omxCameraSettings.framerate = 30;
	omxCameraSettings.isUsingTexture = true;

	videoGrabber.setup(omxCameraSettings);
	filterCollection.setup();

	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);

	font.loadFont("verdana.ttf", 48);
	current_msg_string = "none";
}

//--------------------------------------------------------------
void ofApp::update(){
	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);

		// check for mouse moved message
		if(m.getAddress() == "/message"){
			current_msg_string = m.getArgAsString(0);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(225);
	videoGrabber.draw();

	font.drawString(current_msg_string, 30, 35);
}

//--------------------------------------------------------------
void ofApp::onCharacterReceived(SSHKeyListenerEventData& e)
{
	keyPressed((int)e.character);
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	ofLogVerbose(__func__) << key;

	if (key == 'e')
	{
		videoGrabber.applyImageFilter(filterCollection.getNextFilter());
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
