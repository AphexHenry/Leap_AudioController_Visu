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
    mScale.push_back(41 + 24);
    mScale.push_back(42 + 24);
    
    mGui.setup();
    ofxLabel * lXLabel = new ofxLabel();
    *lXLabel = std::string("");
    mGui.add(lXLabel);
    for(int i = 0; i < FINGER_COUNT; i++)
    {
//        mProgXButton[i].addListener(this, &ofApp::progXSend);
        mGui.add(mProgXButton[i].setup("prog X " + to_string(i), false));
    }
    
    ofxLabel * lYLabel = new ofxLabel();
    *lYLabel = std::string("");
    mGui.add(lYLabel);
    
    for(int i = 0; i < FINGER_COUNT; i++)
    {
        mGui.add(mProgYButton[i].setup("prog Y " + to_string(i), false));
    }
    
    for(int i = 0; i < FINGER_COUNT; i++)
    {
        mGui.add(mNoteButton[i].setup("note " + to_string(i), false));
    }
    mGui.add(mPitchMod.setup("pitch", 0, -50, 50));
}

void ofApp::checkToggles()
{
    for(int i = 0; i < FINGER_COUNT; i++)
    {
        if(mProgXButton[i] == true) {
            mProgXButton[i] = false;
            sendControlChange(i, 50, true);
        }
    }
    
    for(int i = 0; i < FINGER_COUNT; i++)
    {
        if(mProgYButton[i] == true) {
            mProgYButton[i] = false;
            sendControlChange(i, 50, false);
        }
    }
    
    for(int i = 0; i < FINGER_COUNT; i++)
    {
        if(mNoteButton[i] == true) {
            mNoteButton[i] = false;
            sendNote(i, 50);
        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
    int note = 0;
    int velocity = 0;
    
    // check for waiting messages
    while(mOSCReceiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        mOSCReceiver.getNextMessage(m);
        
        // check for mouse moved message
        std::string lAdress = m.getAddress();
        
        note = m.getArgAsInt(0) % 5;
        int channel = getControlIndex(note);
        
        if(m.getAddress() == "/note")
        {
            velocity = m.getArgAsInt32(1);
            sendNote(note, velocity);
        }
        else if(m.getAddress() == "/controls")
        {
            int lX = m.getArgAsInt32(1);
            int lY = m.getArgAsInt32(1);
            int lNewNote = lY / 5.f - 5;
            sendNote(note, 127, lNewNote);
            sendControlChange(note, lX, true);
            sendControlChange(note, lY, false);
        }
    }
    
    checkToggles();
}

void ofApp::sendControlChange(int indexControl, int aValue, bool isX) {
    int channel = 1;//getControlIndex(indexControl);
    indexControl = isX ? indexControl * 2 : indexControl * 2 + 1;
    midiOut.sendControlChange(channel, indexControl, aValue);
}

void ofApp::sendNote(int indexControl, int aValue, int aAddNote) {

    int channel = getControlIndex(indexControl);
    int lIndexNote = max(min(indexControl + aAddNote, (int)mScale.size() - 1), 0);
    if(abs(lIndexNote - mLastNote[indexControl]) >= 2 || !aValue) {
        mLastNote[indexControl] = lIndexNote;
        // previous note off.
        int noteOff = mScale[mLastNote[indexControl]] + 12 * indexControl + mPitchMod;
        midiOut.sendNoteOff(channel, noteOff,  0);
        
        int note = mScale[lIndexNote] + 12 * indexControl + mPitchMod;
        std::cout << "note on. channel " << channel << " note " << note << " value " << aValue << std::endl;
        midiOut.sendNoteOn(channel, note,  aValue);
    }
//    int note = mScale[lIndexNote] + 24;
//    midiOut.sendNoteOn(channel, note,  aValue);
}

//--------------------------------------------------------------
void ofApp::draw(){
    // let's see something
    ofSetColor(0);
    mGui.draw();
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
