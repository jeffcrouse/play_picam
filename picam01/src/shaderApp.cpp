#include "shaderApp.h"

//--------------------------------------------------------------
void shaderApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetLogLevel("ofThread", OF_LOG_SILENT);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();
	ofSetFullscreen(true);
	
	doDrawInfo	= true;
		
	consoleListener.setup(this);
	
	omxCameraSettings.width = 640;
	omxCameraSettings.height = 480;
	omxCameraSettings.framerate = 30;
	omxCameraSettings.isUsingTexture = true;
	
	videoGrabber.setup(omxCameraSettings);

	shader.load("shaderExample");
	
	fbo.allocate(omxCameraSettings.width, omxCameraSettings.height);
	fbo.begin();
		ofClear(0, 0, 0, 0);
	fbo.end();
	
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);

	font[0].loadFont("verdana.ttf", 150);
	font[1].loadFont("verdana.ttf", 64);

	for(map<string, OMX_IMAGEFILTERTYPE>::iterator it = OMX_Maps::getInstance().imageFilters.begin();
				it!=OMX_Maps::getInstance().imageFilters.end(); ++it) {
        	ofLogNotice() << (*it).first;
   	}
		
}	

//--------------------------------------------------------------
void shaderApp::update()
{
	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);

		if(m.getAddress() == "/lineOne"){
			lineOne = m.getArgAsString(0);
		}

		if(m.getAddress() == "/lineTwo"){
			lineTwo = m.getArgAsString(0);
		}

		if(m.getAddress() == "/filter") {
			string newName = m.getArgAsString(0);
			for(map<string, OMX_IMAGEFILTERTYPE>::iterator it = OMX_Maps::getInstance().imageFilters.begin();
				it!=OMX_Maps::getInstance().imageFilters.end(); ++it)
			{
				string name = (*it).first;
				OMX_IMAGEFILTERTYPE filter = (*it).second;

				if (name == newName)  {
					currentFilter = newName;
					videoGrabber.applyImageFilter(filter);
				}
			}
		}
	}


	if (videoGrabber.isFrameNew())
	{

		fbo.begin();
		ofClear(0, 0, 0, 0);
		shader.begin();
			shader.setUniformTexture("tex0", videoGrabber.getTextureReference(), videoGrabber.getTextureID());
			shader.setUniform1f("time", ofGetElapsedTimef());
			shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
			videoGrabber.draw();
		shader.end();
		fbo.end();
	}

}


//--------------------------------------------------------------
void shaderApp::draw(){
	
	fbo.draw(0, 0, ofGetWidth(), ofGetHeight());		

	stringstream info;
	info << "APP FPS: " << ofGetFrameRate() << "\n";
	info << "Camera Resolution: " << videoGrabber.getWidth() << "x" << videoGrabber.getHeight()	<< " @ "<< videoGrabber.getFrameRate() <<"FPS"<< "\n";
	info << "CURRENT FILTER: " << currentFilter << "\n";

	if (doDrawInfo) 
	{
		ofDrawBitmapStringHighlight(info.str(), 100, 100, ofColor::black, ofColor::yellow);
	}

	ofRectangle box;
	int x;
	int y = font[0].getLineHeight()+50;

	box = font[0].getStringBoundingBox(lineOne, 0, 0);
	x = (ofGetWidth()/2.0) - (box.width/2.0);
	font[0].drawString(lineOne, x, y);
	
	y += font[1].getLineHeight()+50;
	box = font[1].getStringBoundingBox(lineTwo, 0, 0);
	x = (ofGetWidth()/2.0) - (box.width/2.0);
	font[1].drawString(lineOne, x, y);	
}

//--------------------------------------------------------------
void shaderApp::keyPressed  (int key)
{
	ofLogVerbose(__func__) << key;

}
