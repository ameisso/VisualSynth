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
    //profZ=500; //a modifier, pourra être dans le fichier de config.
    for (int i=0; i<nbSynthsForBalls;i++)
    {
        synthsForBalls.push_back(true);
    }
    ballNoiseFactor=200;
    lifeSpeed=0.995;
    minDistToLink=100;
    maxDistToUnlink=200;
    showLinks=true;
}

//--------------------------------------------------------------
void testApp::update()
{
    receiveOscMessage();
    for(vector< ofPtr<Ball> >::iterator it = theBalls.begin(); it != theBalls.end(); ++it)
	{
		(*it)->update();
		sendOscInfos((*it));
		if(showLinks)
		{
			for(vector< ofPtr<Ball> >::iterator sit = theBalls.begin(); sit != theBalls.end(); ++sit)
			{
				if((*sit)->getPosition().distance((*it)->getPosition())<minDistToLink)
				{
					(*it)->addLink((*sit)->getRefNumber());
				}
				else if((*it)->checkLink((*sit)->getRefNumber())&&(*sit)->getPosition().distance((*it)->getPosition())>maxDistToUnlink)
				{
					(*it)->removeLink((*sit)->getRefNumber());
					//cout<<"link removed ["<<(*sit)->getRefNumber()<<","<<(*it)->getRefNumber()<<"]"<<endl;
				}
			}
		}
		if((*it)->checkIfDead()==true)
		{
			theBalls.erase(it);
			break;
		}
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofBackground(0);
    ofSetHexColor(0xffffff);
    for(vector< ofPtr<Ball> >::iterator it = theBalls.begin(); it != theBalls.end(); ++it)
	{
		(*it)->draw();
		if(showLinks)
		{
			for(vector< ofPtr<Ball> >::iterator sit = it; sit != theBalls.end(); ++sit)
			{
				if((*it)->checkLink((*sit)->getRefNumber()))
				{
					ofVec3f p1,p2,d,n;
					p1=(*it)->getPosition();
					p2=(*sit)->getPosition();
					d = p2-p1;
					n.x = d.y;
					n.y = -d.x;
					ofPoint cp1 = ofPoint(p1 + 0.4*d+0.2*n);
					//ofPoint cp2 = ofPoint(p1+0.9*d+0.1*n);
					ofPath link;
					ofLine(p1,p1);
					link.setFilled(false);
					link.quadBezierTo(p1,cp1,p2);
					//link.close();
					//link.quadBezierTo(p2-0.1*d,cp2,p1+0.1*d);
					link.draw();
					
				}
			}
		}
	}
    // we display each ball
	
	
}
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    cout<<"key pressed : "<<key<<endl;
    if (key==70||key==102)
    {
        cout<<"toggle fullscreen"<<endl;
        ofToggleFullscreen();
    }
	
    if (key == 32) //space to create a ring around the last ball
    {
        if (theBalls.size() != 0)
        {
			int ringWidth = 5;
            (*theBalls.back()).addCircle((*theBalls.back()).getRadius(),ringWidth);
        }
    }
    if (key == 114||key==82)// r or R
    {
        cout<<"all ball unloaded"<<endl;
		theBalls.clear();
    }
    if (key == 108||key==76)// l or L
    {
        //affiche ou non les liens
        showLinks=!showLinks;
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
	/* cout<<"Synth "<<synthNbr<<endl;
	 cout<<"NF"<<ballNoiseFactor<<endl;
	 cout<<"LS"<<lifeSpeed<<endl;*/
    string path=pathToImages+"/"+theTextures[synthNbr];
    theBalls.push_back(ofPtr<Ball> (new Ball(refNumber,x,y,0,synthNbr,path,ofRandom(10,50),ballNoiseFactor,lifeSpeed)));
    refNumber+=1;
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
        //cout<<"received a message : "<<ofToString(OscReceivedMessage.getAddress())<<endl;
        if(OscReceivedMessage.getAddress()=="/pad/1"||OscReceivedMessage.getAddress()=="/1/multixy1/1")
        {
            //cout<<"first Finger"<<endl;
            //Si jamais, on pourra toujours atribuer des paramètres différents en fonction du nombre de doigts.
            float xVal=OscReceivedMessage.getArgAsFloat(0)*ofGetWidth();
            float yVal=OscReceivedMessage.getArgAsFloat(1)*ofGetHeight();
            //cout<<xVal<<" "<<yVal<<endl;
            int synthNbr=attributeSynth();
            string path=pathToImages+"/"+theTextures[synthNbr];
            theBalls.push_back(ofPtr<Ball> (new Ball (refNumber,xVal,yVal,zPos,synthNbr,path,ofRandom(10,50),ballNoiseFactor,lifeSpeed)));
            refNumber+=1;
        }
        else if(OscReceivedMessage.getAddress()=="/pad/2"||OscReceivedMessage.getAddress()=="/1/multixy/2")//deuxième doigt
        {
            int synthNbr=attributeSynth();
            string path=pathToImages+"/"+theTextures[synthNbr];
            theBalls.push_back(ofPtr<Ball> (new Ball (refNumber,OscReceivedMessage.getArgAsFloat(0)*ofGetWidth(),OscReceivedMessage.getArgAsFloat(1)*ofGetHeight(),zPos,synthNbr,path,ofRandom(10,50),ballNoiseFactor,lifeSpeed)));
            refNumber+=1;
        }
        else if(OscReceivedMessage.getAddress()=="/pad/3"||OscReceivedMessage.getAddress()=="/1/multixy/3")//troisième doigt
        {
            int synthNbr=attributeSynth();
            string path=pathToImages+"/"+theTextures[synthNbr];
            theBalls.push_back(ofPtr<Ball> (new Ball (refNumber,OscReceivedMessage.getArgAsFloat(0)*ofGetWidth(),OscReceivedMessage.getArgAsFloat(1)*ofGetHeight(),zPos,synthNbr,path,ofRandom(10,50),ballNoiseFactor,lifeSpeed)));
            refNumber+=1;
        }
        else if(OscReceivedMessage.getAddress()=="/pad/4"||OscReceivedMessage.getAddress()=="/1/multixy/4")//quatrième doigt
        {
            int synthNbr=attributeSynth();
            string path=pathToImages+"/"+theTextures[synthNbr];
            theBalls.push_back(ofPtr<Ball> (new Ball (refNumber,OscReceivedMessage.getArgAsFloat(0)*ofGetWidth(),OscReceivedMessage.getArgAsFloat(1)*ofGetHeight(),zPos,synthNbr,path,ofRandom(10,50),ballNoiseFactor,lifeSpeed)));
            refNumber+=1;
        }
        else if(OscReceivedMessage.getAddress()=="/pad/5"||OscReceivedMessage.getAddress()=="/1/multixy/5")//cinquième doigt
        {
            int synthNbr=attributeSynth();
            string path=pathToImages+"/"+theTextures[synthNbr];
            theBalls.push_back(ofPtr<Ball> (new Ball (refNumber,OscReceivedMessage.getArgAsFloat(0)*ofGetWidth(),OscReceivedMessage.getArgAsFloat(1)*ofGetHeight(),zPos,synthNbr,path,ofRandom(10,50),ballNoiseFactor,lifeSpeed)));
            refNumber+=1;
        }
        else if (OscReceivedMessage.getAddress()=="/1/multifader1/1")//durée de vie de la balle
        {
            lifeSpeed=ofMap(OscReceivedMessage.getArgAsFloat(0),0,1,0.9,1);
            cout<<"lifeSpeed :"<<lifeSpeed<<endl;
        }
        else if (OscReceivedMessage.getAddress()=="/1/multifader1/2")//vitesse de déplacement
        {
			ballNoiseFactor=(int)200*OscReceivedMessage.getArgAsFloat(0);
            cout<<"noiseFactor :"<<ballNoiseFactor<<endl;
        }
        else if (OscReceivedMessage.getAddress()=="/1/multifader1/3")//position en z
        {
			zPos=ofMap(OscReceivedMessage.getArgAsFloat(0),0,1,0,profZ);
            cout<<"Zpos :"<<zPos<<endl;
        }
        else
		{
			cout<<"I don't know this message :"<<ofToString(OscReceivedMessage.getAddress())<<endl;
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
    pathToImages=configFile.getValue("path");
    cout<<"path to Images :"<<pathToImages<<endl;
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
    int nbTextures=configFile.getNumChildren();
    cout<<"NB textures : "<<nbTextures<<endl;
    for(int i=0; i<nbTextures; i++)
    {
        theTextures.push_back(configFile.getValue("image["+ofToString(i)+"]"));
        cout<<"texture["<<ofToString(i)<<"]"<<configFile.getValue("image["+ofToString(i)+"]")<<endl;
    }
	
    file.close();
    buffer.clear();
    configFile.clear();
    cout<<endl<<"*****************************************************************"<<endl;
    cout<<"XML files read, objects created" <<endl;
}
