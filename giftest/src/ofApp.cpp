#include "ofApp.h"

string basename(string url) {
    vector<string> pieces = ofSplitString(url, "/");
    return pieces[pieces.size()-1];
}
string download(string url) {
    string path = basename(url);
    ofHttpResponse resp = ofSaveURLTo(url, basename(url));
    return path;
}

//--------------------------------------------------------------
void ofApp::setup(){
    
    //string path = "tumblr_nd3po2OJpN1s141c3o1_r4 500.gif";
    //string path = "tumblr_nbyrlaLLDw1s141c3o1_r1_400.gif";
    
    string url = "http://33.media.tumblr.com/fd522c0cb9ab10cadd2bef56db6f1364/tumblr_nbnl04Iy1o1s141c3o3_500.gif";
    
    string path = basename(url);
    if(!ofFile::doesFileExist(path, true)) {
        path = download(url);
    }
    
    gif.load(path);
}

//--------------------------------------------------------------
void ofApp::update(){
    gif.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    gif.draw(20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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


