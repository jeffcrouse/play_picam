#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetLogLevel("ofThread", OF_LOG_SILENT);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();

	//consoleListener.setup(this);

	omxCameraSettings.width = 640;
	omxCameraSettings.height = 480;
	omxCameraSettings.framerate = 30;
	omxCameraSettings.isUsingTexture = false;

	videoGrabber.setup(omxCameraSettings);
	videoGrabber.setFrameStabilization(true);
	videoGrabber.setFlickerCancellation(OMX_COMMONFLICKERCANCEL_AUTO);

	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);

	font.loadFont("verdana.ttf", 48);

	for(map<string, OMX_IMAGEFILTERTYPE>::iterator it = OMX_Maps::getInstance().imageFilters.begin();
				it!=OMX_Maps::getInstance().imageFilters.end(); ++it)
    {
        ofLogNotice() << (*it).first;
    }

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
			TextLine line;
			line.text = m.getArgAsString(0);
			line.color.r = m.getArgAsInt32(1);
			line.color.g = m.getArgAsInt32(2);
			line.color.b = m.getArgAsInt32(3);
			lines.push_back( line );
			if(lines.size()>10) {
				lines.erase(lines.begin());
			}
		}


		if(m.getAddress() == "/filter") {
			string newName = m.getArgAsString(0);
			for(map<string, OMX_IMAGEFILTERTYPE>::iterator it = OMX_Maps::getInstance().imageFilters.begin();
				it!=OMX_Maps::getInstance().imageFilters.end(); ++it)
			{
				string name = (*it).first;
				OMX_IMAGEFILTERTYPE filter = (*it).second;

				if (name == newName)  {
					videoGrabber.applyImageFilter(filter);
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(225);
	videoGrabber.draw();

	int y = 60;
	for(int i=0; i<lines.size(); i++) {
		TextLine& line = lines[i];

		ofPushStyle();
		ofSetColor(line.color);

		ofRectangle box = font.getStringBoundingBox(line.text, 0, 0);
		int x = (ofGetWidth()/2.0) - (box.width/2.0);

		font.drawString(line.text, x, y);
		y += 50;

		ofPopStyle();
	}

}

//--------------------------------------------------------------
// void ofApp::onCharacterReceived(SSHKeyListenerEventData& e)
// {
// 	keyPressed((int)e.character);
// }


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	ofLogVerbose(__func__) << key;
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
