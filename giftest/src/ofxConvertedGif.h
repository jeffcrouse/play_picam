//
//  ofxConvertedGif.h
//  giftest
//
//  Created by Jeffrey Crouse on 10/15/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "ofMain.h"

#define CONVERT "/usr/local/bin/convert"
#define IDENTIFY "/usr/local/bin/identify"
#define MKTEMP "/usr/bin/mktemp"

class ofxConvertedGif
{
public:
    void load(string path);
    void update();
    void draw(int x, int y);
    
    
    string ofxSystemCall(string command);
    vector<ofImage*> frames;
    vector<float> frameTimes;
    int f;
    float t;
    float speed;
    stringstream cmd;
};

