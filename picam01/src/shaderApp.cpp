#include "shaderApp.h"



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


	ofLogNotice() << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);


	ofLogNotice() << "Loading fonts";
	font[0].loadFont("fonts/Lubalin Graph Bold.ttf", 120);
	font[1].loadFont("fonts/Lubalin Graph Demi Regular.ttf", 32);

	ofLogNotice() << "Listing filters";
	map<string, OMX_IMAGEFILTERTYPE>::iterator it;
	for(it = OMX_Maps::getInstance().imageFilters.begin();
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

	displayMode = MODE_CAMERA;
}	

//--------------------------------------------------------------
void shaderApp::update()
{
	float now = ofGetElapsedTimef();
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
			map<string, OMX_IMAGEFILTERTYPE>::iterator it;
			for(it = OMX_Maps::getInstance().imageFilters.begin();
				it!=OMX_Maps::getInstance().imageFilters.end(); ++it) {
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
				if(name == videoToPlay) {
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
					displayMode = MODE_VIDEO;
				}
			}
		}
		
		if(m.getAddress() == "/gif") {
			string url = m.getArgAsString(0);
			loadGIF(url);
		}
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


	if (displayMode == MODE_CAMERA && videoGrabber.isFrameNew())
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

	/*
	if(displayMode==MODE_GIF && now > gifAdvanceFrame) {

		int totalFrames = 10;
		gifFrame = (gifFrame+1) % totalFrames;
		
		ofLogNotice() << "Advancing GIF frame ("<< gifFrame << "/" << totalFrames << ")";

		int w = 0; //file.getWidth();
		int h = 0; //file.getHeight();

		if(h > w) {
			gifBounds.height = ofGetHeight();
			gifBounds.width = (ofGetHeight()/h) * w;
		} else {
			gifBounds.width = ofGetWidth();
			gifBounds.height = (ofGetWidth()/w) * h;
		}

		gifBounds.x = (ofGetWidth()/2.0) - (gifBounds.width/2.0);
		gifBounds.y = (ofGetHeight()/2.0) - (gifBounds.height/2.0);
		
		gifAdvanceFrame = now + gifFrameRate;

	}
	*/

}

//--------------------------------------------------------------
void shaderApp::loadGIF(string url) {
	string gif = "gif";
	if(ofxStringEndsWith(url, gif)==false) {
		return;
	}

	vector<string> pieces = ofSplitString(url, "/");
	int len = pieces.size();
	string name = pieces[len-1];
	string path = ofToDataPath("images/"+name);

		
	if(ofFile::doesFileExist(path, false)==false) {
		ofLogNotice() << "Saving " << url << " to " << path;
		ofSaveURLTo(url, path);

	} else {
		ofLogNotice() << name << "Already exists!";
	}


	currentImage = name;
	displayMode = MODE_GIF;


}

//--------------------------------------------------------------
void shaderApp::threadedFunction() {
	if(lock()) {
		

		
		unlock();
	}
}

//--------------------------------------------------------------
void shaderApp::draw(){
	ofBackground(0);

	switch(displayMode) {
		case MODE_CAMERA:
			camFbo.draw(0, 0, ofGetWidth(), ofGetHeight());
			break;

		case MODE_VIDEO:
			omxPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
			break;

		case MODE_IMAGE:

			break;

		default:
			ofLogWarning() << "INVALID DISPLAY MODE!";
			break;
	}
	
	overlayFbo.draw(0, 0, ofGetWidth(), ofGetHeight());


	stringstream info;
	info << "APP FPS: " << ofGetFrameRate() << "\n";
	info << "Camera Resolution: " << videoGrabber.getWidth() << "x" << videoGrabber.getHeight()	<< " @ "<< videoGrabber.getFrameRate() <<"FPS"<< "\n";
	info << "CURRENT FILTER: " << currentFilter << "\n";
	info << "CURRENT VIDEO: " << currentVideo << " ";
	info << "(" << omxPlayer.getCurrentFrame() << "/" << omxPlayer.getTotalNumFrames() << ")" << endl;
	info << "CURRENT IMAGE: " << currentImage << endl;
	info << "DISPLAY MODE: " << displayMode << endl;
	if (doDrawInfo) 
	{
		ofDrawBitmapStringHighlight(info.str(), 100, 100, ofColor::black, ofColor::yellow);
	}	
}

//--------------------------------------------------------------
bool ofxStringEndsWith(string &str, string& key) {
    size_t i = str.rfind(key);
    return (i != string::npos) && (i == (str.length() - key.length()));
}

//--------------------------------------------------------------
void shaderApp::onVideoEnd(ofxOMXPlayerListenerEventData& e) {
	ofLogVerbose(__func__) << " RECEIVED";
	displayMode = MODE_CAMERA;
}

//--------------------------------------------------------------
string ofxSystemCall(string command) {
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    result.erase(remove(result.begin(), result.end(), '\n'), result.end());
    return result;
}
