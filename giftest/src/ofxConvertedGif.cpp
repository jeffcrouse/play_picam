//
//  ofxConvertedGif.cpp
//  giftest
//
//  Created by Jeffrey Crouse on 10/15/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "ofxConvertedGif.h"

string q(string in) {
    return "\""+in+"\"";
}

//--------------------------------------------------------------
string ofxTrimStringRight(string str) {
    size_t endpos = str.find_last_not_of(" \t\r\n");
    return (string::npos != endpos) ? str.substr( 0, endpos+1) : str;
}

//--------------------------------------------------------------
bool ofxStringEndsWith(string &str, string& key) {
    size_t i = str.rfind(key);
    return (i != string::npos) && (i == (str.length() - key.length()));
}

//--------------------------------------------------------------
bool ofxStringStartsWith(string &str, string &key) {
    return str.find(key) == 0;
}

//--------------------------------------------------------------
void ofxConvertedGif::load(string gif) {
    
    // 1. Make a temporary directory for all of the frames
    cmd << MKTEMP << " -d /tmp/" << ofGetTimestampString();
    string tmpdir = ofxSystemCall( cmd.str() );
    ofLogNotice("ofxConvertedGif") << tmpdir;
    cmd.str("");
    
    
    // 2. Export all of the frames
    string input = q(ofToDataPath(gif, true));
    string output = q(tmpdir+"/frame%04d.png");
    cmd << CONVERT << " " << input << " -coalesce " << output;
    ofLogNotice("ofxConvertedGif") << cmd.str();
    ofxSystemCall( cmd.str() );
    cmd.str("");
    
    
    // 3. Read in all of the frames;
    ofDirectory dir;
    //dir.allowExt("png");
    dir.listDir(tmpdir);
    dir.sort();
    ofLogNotice("ofxConvertedGif") << "found " << dir.size() << " frames";
    if( dir.size() ){
        
        for(int i=0; i<(int)dir.size(); i++) {
            string path = dir.getPath(i);
            ofLogNotice("ofxConvertedGif") << path;
            ofImage* img = new ofImage();
            img->loadImage(path);
            frames.push_back(img);
        }
    }
    
    dir.remove(true);
    
    
    cmd << IDENTIFY << " " << q(ofToDataPath(gif, true));
    ofLogNotice("ofxConvertedGif") << cmd.str();
    
    
    string info = ofxSystemCall(cmd.str());
    vector<string> lines = ofSplitString(info, "\n");
    for(int i=0; i<lines.size(); i++) {
        // TO DO: Error checking!!! So unsafe.
        vector<string> columns = ofSplitString(lines[i], " ");
        int len = columns.size();
        string timecode = columns[len-1];
        vector<string> pieces = ofSplitString(timecode, ":");
        len = pieces.size();
        float delay = 0;
        delay += ofToFloat( pieces[len-1] );
        delay += ofToInt( pieces[len-2] ) * 60;
        delay = max(delay, 0.01f);

        frameTimes.push_back( delay );
        ofLogNotice() << i << " - " << timecode << " = " << delay;
    }
    
    
    f = 0;
    t = 0;
    speed = 0.5;
}

//--------------------------------------------------------------
void ofxConvertedGif::update() {
    float now = ofGetElapsedTimef();
    if(now > t) {
        f++;
        f %= frames.size();
        t = now + (frameTimes[f] / speed);
    }
}

//--------------------------------------------------------------
void ofxConvertedGif::draw(int x, int y) {
    frames[f]->draw(x, y);
}

//--------------------------------------------------------------
string ofxConvertedGif::ofxSystemCall(string command) {
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    //result.erase(remove(result.begin(), result.end(), '\n'), result.end());
    return ofxTrimStringRight(result);
}