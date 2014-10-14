#include "shaderApp.h"


bool ofxStringEndsWith(string &str, string& key) {
    size_t i = str.rfind(key);
    return (i != string::npos) && (i == (str.length() - key.length()));
}

//--------------------------------------------------------------
void shaderApp::onVideoEnd(ofxOMXPlayerListenerEventData& e)
{
	ofLogVerbose(__func__) << " RECEIVED";
	displayMode = MODE_CAMERA;
}


//--------------------------------------------------------------
void shaderApp::setup()
{
	//ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetLogLevel("ofThread", OF_LOG_SILENT);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();
	ofSetFullscreen(true);
	
	doDrawInfo = false;
	
	OMXCameraSettings omxCameraSettings;
	omxCameraSettings.width = 640;
	omxCameraSettings.height = 480;
	omxCameraSettings.framerate = 30;
	omxCameraSettings.isUsingTexture = true;
	
	videoGrabber.setup(omxCameraSettings);

	shader.load("shaderExample");
	
	ofLogNotice() << "Allocating FBOs";
	camFbo.allocate(omxCameraSettings.width, omxCameraSettings.height);
	camFbo.begin();
		ofClear(0, 0, 0, 0);
	camFbo.end();
	
	overlayFbo.allocate(omxCameraSettings.width, omxCameraSettings.height);
	overlayFbo.begin();
		ofClear(0, 0, 0, 0);
	overlayFbo.end();

	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);

	ofLogNotice() << "Loading fonts";
	font[0].loadFont("fonts/Lubalin Graph Bold.ttf", 120);
	font[1].loadFont("fonts/Lubalin Graph Demi Regular.ttf", 32);

	ofLogNotice() << "Listing filters";
	for(map<string, OMX_IMAGEFILTERTYPE>::iterator it = OMX_Maps::getInstance().imageFilters.begin();
				it!=OMX_Maps::getInstance().imageFilters.end(); ++it) {
        	ofLogNotice() << (*it).first;
   	}

	ofLogNotice() << "Listing videos";
	ofDirectory dir;
	dir.listDir("videos/");
	for(int i = 0; i < (int)dir.size(); i++){
		string path = dir.getName(i);
		ofLogNotice() << path;
	}

	gifFrame = 0;
	frameRate = 0.5;
	displayMode = MODE_CAMERA;
}	

//--------------------------------------------------------------
void shaderApp::update()
{
	bool textChanged = false;

	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);

		if(m.getAddress() == "/text"){
			lineOne = ofToString(m.getArgAsInt32(0));
			lineTwo = m.getArgAsString(1);
			textChanged = true;
		}

		if(m.getAddress() == "/filter") {
			string filterToUse = m.getArgAsString(0);
			for(map<string, OMX_IMAGEFILTERTYPE>::iterator it = OMX_Maps::getInstance().imageFilters.begin();
				it!=OMX_Maps::getInstance().imageFilters.end(); ++it)
			{
				string name = (*it).first;
				OMX_IMAGEFILTERTYPE filter = (*it).second;

				if (ofToLower(name) == ofToLower(filterToUse))  {
					currentFilter = filterToUse;
					videoGrabber.applyImageFilter(filter);
				}
			}
		}
		
		if(m.getAddress() == "/info") {
			int i = m.getArgAsInt32(0);
			doDrawInfo = i;
		}

		if(m.getAddress() == "/video") {
			string videoToPlay = m.getArgAsString(0);
			ofDirectory dir;
			dir.listDir("videos/");

			for(int i = 0; i < (int)dir.size(); i++){
				string name = dir.getName(i);
				if(name==videoToPlay) {
					ofLogNotice("!!!") << "playing " << name;

					string videoPath = ofToDataPath(dir.getPath(i), true);
					
					ofxOMXPlayerSettings settings;
					settings.videoPath = videoPath;
					settings.useHDMIForAudio = true;	//default true
					settings.enableTexture = true;		//default true
					settings.enableLooping = false;		//default true
					settings.enableAudio = true;		//default true, save resources by disabling
					settings.listener = this;
					//settings.doFlipTexture = true;		//default false
					
					//so either pass in the settings
					omxPlayer.setup(settings);
					
					currentVideo = name;
					displayMode = MOVE_VIDEO;
				}
			}
		}
		
		if(m.getAddress() == "/image") {
			string url = m.getArgAsString(0);

			if(ofxStringEndsWith(url, "gif")) {
				
				vector<string> pieces = ofSplitString(url, "/");
				int len = pieces.size();
				string path = ofToDataPath("images/"+pieces[len-1]);
				ofLogNotice() << "Saving " << url << " to " << path;
				ofSaveURLTo(url, path);

				gifFrame = 0;
				gifloader.load(path);
				gifAdvanceFrame = ofGetElapsedTimef() + gif_frameRate;
				displayMode = MODE_IMAGE;
			}
		}
	}


	if (videoGrabber.isFrameNew() && !bVideoPlaying)
	{
		camFbo.begin();
		ofClear(0, 0, 0, 0);
		shader.begin();
			shader.setUniformTexture("tex0", videoGrabber.getTextureReference(), videoGrabber.getTextureID());
			shader.setUniform1f("time", ofGetElapsedTimef());
			shader.setUniform2f("resolution", videoGrabber.getWidth(), videoGrabber.getHeight());
			videoGrabber.draw();
		shader.end();
		camFbo.end();
	}

	if(textChanged)
	{
		overlayFbo.begin();
			ofClear(0, 0, 0, 0);
			ofRectangle box;
			int x;
			int y = font[0].getLineHeight()+50;
		
			box = font[0].getStringBoundingBox(lineOne, 0, 0);
			x = (overlayFbo.getWidth()/2.0) - (box.width/2.0);
			ofSetColor(ofColor::black);
			font[0].drawString(lineOne, x+2, y+2);
			ofSetColor(ofColor::white);
			font[0].drawString(lineOne, x, y);
			
			y += font[1].getLineHeight()+50;
			box = font[1].getStringBoundingBox(lineTwo, 0, 0);
			x = (overlayFbo.getWidth()/2.0) - (box.width/2.0);
			
			ofSetColor(ofColor::black);
			font[1].drawString(lineTwo, x+2, y+2);
			ofSetColor(ofColor::white);
			font[1].drawString(lineTwo, x, y);


		overlayFbo.end();
	}

}


//--------------------------------------------------------------
void shaderApp::draw(){
	float now = ofGetElapsedTimef();

	switch(displayMode) {
	case MODE_CAMERA:
		camFbo.draw(0, 0, ofGetWidth(), ofGetHeight());
		break;
	case MODE_VIDEO:
		omxPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
		break;
	case MODE_IMAGE:
		if(now > gifAdvanceFrame) {
			gifFrame = (gifFrame+1) % gifLoader.pages.size();
			gifAdvanceFrame = ofGetElapsedTimef() + gifFrameRate;
		}
		gifLoader.pages[gif_frame].draw(0, 0, ofGetWidth(), ofGetHeight());
		break;
	}
	
	overlayFbo.draw(0, 0, ofGetWidth(), ofGetHeight());


	stringstream info;
	info << "APP FPS: " << ofGetFrameRate() << "\n";
	info << "Camera Resolution: " << videoGrabber.getWidth() << "x" << videoGrabber.getHeight()	<< " @ "<< videoGrabber.getFrameRate() <<"FPS"<< "\n";
	info << "CURRENT FILTER: " << currentFilter << "\n";
	info << "CURRENT VIDEO: " << currentVideo << " ";
	info << "(" << omxPlayer.getCurrentFrame() << "/" << omxPlayer.getTotalNumFrames() << ")" << endl;

	if (doDrawInfo) 
	{
		ofDrawBitmapStringHighlight(info.str(), 100, 100, ofColor::black, ofColor::yellow);
	}	
}
