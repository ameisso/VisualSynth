#include "testApp.h"
#include "ofxOsc.h"
//--------------------------------------------------------------
void testApp::setup()
{
    ofSetWindowTitle("VisualSynth");
    cout<<"***********************VisualSynth******************************"<<endl;
    OscReceiver.setup(8000);
    readXmlSetup();
    OscReceiver.setup(oscReceivePort);
    oscSender.setup(oscSendAddress,oscSendPort);
    profZ=1000; //a modifier, pourra être dans le fichier de config.
    for (int i=0; i<nbSynthsForBalls;i++)
    {
        synthsForBalls.push_back(true);
    }
}

//--------------------------------------------------------------
void testApp::update()
{
    ofBackground(50,50,255);

    receiveOscMessage();

    for(vector< ofPtr<Ball> >::iterator it = theBalls.begin(); it != theBalls.end(); ++it)
        {
            (*it)->update();
            sendOscInfos((*it));
        }
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofSetHexColor(0xffffff);
    for(vector< ofPtr<Ball> >::iterator it = theBalls.begin(); it != theBalls.end(); ++it)
        {
            (*it)->draw();
        }
    // we display each ball

}
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    //cout<<"key pressed : "<<key<<endl;
    if (key==70||key==102)
    {
        cout<<"toggle fullscreen"<<endl;
        ofToggleFullscreen();
    }
    if (key == 32) //space to create a ring around the last ball
    {
        int ballCircleMargin = 10;
        int ringWidth = 10;
        (*theBalls.back()).addCircle((*theBalls.back()).getRadius()+ballCircleMargin,ringWidth);
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    int synthNbr=attributeSynth();
    cout<<"click "<<synthNbr<<endl;
    theBalls.push_back(ofPtr<Ball> (new Ball(x,y,0,synthNbr,pathToImages)));
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}
void testApp::receiveOscMessage()
{

    while(OscReceiver.hasWaitingMessages())
    {

        OscReceiver.getNextMessage(&OscReceivedMessage);
        cout<<"received a message : "<<ofToString(OscReceivedMessage.getAddress())<<endl;
        if(OscReceivedMessage.getAddress()=="/pad/1")
        {
            //cout<<"first Finger"<<endl;
            //Si jamais, on pourra toujours atribuer des paramètres différents en fonction du nombre de doigts.
            float xVal=OscReceivedMessage.getArgAsFloat(0)*ofGetWidth();
            float yVal=OscReceivedMessage.getArgAsFloat(1)*ofGetHeight();
            cout<<xVal<<" "<<yVal<<endl;
            theBalls.push_back(ofPtr<Ball> (new Ball (xVal,yVal,0,attributeSynth(),pathToImages)));
        }
        else if(OscReceivedMessage.getAddress()=="/pad/2")//deuxième doigt
        {
            theBalls.push_back(ofPtr<Ball> (new Ball (OscReceivedMessage.getArgAsFloat(0)*ofGetWidth(),OscReceivedMessage.getArgAsFloat(1)*ofGetHeight(),0,attributeSynth(),pathToImages)));
        }
        else if(OscReceivedMessage.getAddress()=="/pad/3")//troisième doigt
        {
            theBalls.push_back(ofPtr<Ball> (new Ball (OscReceivedMessage.getArgAsFloat(0)*ofGetWidth(),OscReceivedMessage.getArgAsFloat(1)*ofGetHeight(),0,attributeSynth(),pathToImages)));
        }
        else if(OscReceivedMessage.getAddress()=="/pad/4")//quatrième doigt
        {
            theBalls.push_back(ofPtr<Ball> (new Ball (OscReceivedMessage.getArgAsFloat(0)*ofGetWidth(),OscReceivedMessage.getArgAsFloat(1)*ofGetHeight(),0,attributeSynth(),pathToImages)));
        }
        else if(OscReceivedMessage.getAddress()=="/pad/5")//cinquième doigt
        {
            theBalls.push_back(ofPtr<Ball> (new Ball (OscReceivedMessage.getArgAsFloat(0)*ofGetWidth(),OscReceivedMessage.getArgAsFloat(1)*ofGetHeight(),0,attributeSynth(),pathToImages)));
        }
        else if (OscReceivedMessage.getAddress()=="/1/multifader2/1")//durée de vie de la balle
        {
            lifeSpeed=ofMap(OscReceivedMessage.getArgAsFloat(0),0,1,0.9,1);
        }
        else if (OscReceivedMessage.getAddress()=="/1/multifader2/2")//vitesse de déplacement
        {
           ballNoiseFactor=(int)200*OscReceivedMessage.getArgAsFloat(0);
        }
        else if (OscReceivedMessage.getAddress()=="/1/multifader2/3")//position en z
        {
           zPos=ofMap(OscReceivedMessage.getArgAsFloat(0),0,1,0,profZ);
        }
        else
            {
                cout<<"I don't know this message "<<endl;
            }
    }
}
//fonction qui va envoyer par osc tout les paramètres de la balle.
void testApp::sendOscInfos(ofPtr<Ball>& ballToSend)
{
    int synthNumber=ballToSend->getSynthNumber();
    string address="/ball"+ofToString(synthNumber)+"/radius";
    ofxOscMessage msgToSend = ofxOscMessage();
    msgToSend.setAddress(address);
    //-----
    msgToSend.addFloatArg(ballToSend->getPosition().x);
    msgToSend.addFloatArg(ballToSend->getPosition().y);
    msgToSend.addFloatArg(ballToSend->getPosition().z);
    msgToSend.addFloatArg(ballToSend->getRadius());
    //-----
    //ajouter ici les autre paramètres pour envoyer toutes les infos dans une seule trame ...après tout l'osc c'est fait pour ca.
    oscSender.sendMessage(msgToSend);
   //cout<<"Sended : "<<value<<" @"<< address<<endl;
}
int testApp::attributeSynth()
{
    //!!! on ne pourra pas créer plus de balles que de synthés !
    int i=ofRandom(nbSynthsForBalls);
    while(synthsForBalls[i]==false)
    {
        if(synthsForBalls[i]==true)
        {
            synthsForBalls[i]=false;
            break;
        }
        i=ofRandom(nbSynthsForBalls);
    }
    return i;
}
void testApp::readXmlSetup()
{
    ofFile file;
    //TODO block the process if the file is not there or is corrupted....
    //Test the file existance throw an error if not
    if(file.doesFileExist("config.xml",true)==true)
    {
        cout<<"config.xml found"<<endl;
        cout<<"*****************************************************************"<<endl<<endl;
    }
    else
    {
        cout<<"no config.xml file, default receive port : 8000"<<endl;
        oscReceivePort=8000;
        OscReceiver.setup(oscReceivePort);
        return;
    }

    file.open("config.xml");
    ofBuffer buffer=file.readToBuffer();
    ofXml configFile;
    configFile.loadFromBuffer(buffer.getText());
    configFile.setTo("osc");
    oscReceivePort=configFile.getIntValue("receivePort");
    cout<<"receivePort :"<<oscReceivePort<<endl;
    oscSendPort=configFile.getIntValue("sendPort");
    cout<<"sendPort :"<<oscSendPort<<endl;
    oscSendAddress=configFile.getValue("sendIp");
    cout<<"sendAddress :"<<oscSendAddress<<endl;
    configFile.setTo("../screen"); // go up and then down
    profZ=configFile.getIntValue("profondeurZ");
    cout<<"profZ :"<<profZ<<endl;
    configFile.setTo("../synth"); // go up and then down
    nbSynthsForBalls=configFile.getIntValue("nbSynthsForBalls");
    cout<<"nbSynthsForBalls :"<<nbSynthsForBalls<<endl;
    configFile.setTo("../textures");
    pathToImages=configFile.getValue("path");
    cout<<"path to Images :"<<pathToImages<<endl;

    file.close();
    buffer.clear();
    configFile.clear();
    cout<<endl<<"*****************************************************************"<<endl;
    cout<<"XML files read, objects created" <<endl;
}
