#include "ofApp.h"

#define PORT 1315

//--------------------------------------------------------------
void ofApp::setup(){
    // print the available output ports to the console
    midiOut.listPorts(); // via instance
    //ofxMidiOut::listPorts(); // via static too
    
    // connect
    midiOut.openVirtualPort("LeapMidiOut"); // open a virtual port
    
    mOSCReceiver.setup(PORT);
    
    mScale.push_back(32);
    mScale.push_back(37);
    mScale.push_back(39);
    mScale.push_back(41);
    mScale.push_back(42);
    mScale.push_back(32 + 12);
    mScale.push_back(37 + 12);
    mScale.push_back(39 + 12);
    mScale.push_back(41 + 12);
    mScale.push_back(42 + 12);
    mScale.push_back(32 + 24);
    mScale.push_back(37 + 24);
    mScale.push_back(39 + 24);
}

//--------------------------------------------------------------
void ofApp::update(){
    int channel = 1;
    int note = 0;
    int velocity = 0;
    
    
    
    // check for waiting messages
    while(mOSCReceiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        mOSCReceiver.getNextMessage(m);
        
        // check for mouse moved message
        std::string lAdress = m.getAddress();
        
        int note = m.getArgAsInt32(0) % 12;
        
        if(m.getAddress() == "/note")
        {
            velocity = m.getArgAsInt32(1);
            note = mScale[note] + 24;
            midiOut.sendNoteOn(channel, note,  velocity);
            std::cout << note << " " << velocity << std::endl;
        }
        else if(m.getAddress() == "/controls")
        {
            int lX = m.getArgAsInt32(1);
            int lY = m.getArgAsInt32(1);
            midiOut.sendControlChange(channel, note * 2, lX);
            midiOut.sendControlChange(channel, note * 2 + 1, lY);
        }
    }
    
}


//--------------------------------------------------------------
void ofApp::draw(){
    // let's see something
    ofSetColor(0);
    stringstream text;
    text << "connected to port " << midiOut.getPort()
    << " \"" << midiOut.getName() << "\"" << endl
    << "is virtual?: " << midiOut.isVirtual() << endl << endl;
    ofDrawBitmapString(text.str(), 20, 20);
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
