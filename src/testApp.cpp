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
    for (int i=0; i<nbSynthsForBalls;i++)
    {
        synthsForBalls.push_back(true);
    }
    ballNoiseFactor=100;
    lifeSpeed=0.998;
	circleWidth=3;
	circleIncrease=10;
	nbCircles=0;
	curvedLinks=true;
	curveFactor=1;
	curvePosition=0;
	curveAmplitude=0;
    showLinks=true;
    cout<<"END OF INIT"<<endl;
}

//--------------------------------------------------------------
void testApp::update()
{
    receiveOscMessage();
	sendOscGeneral();
    for(vector< ofPtr<Ball> >::iterator it = theBalls.begin(); it != theBalls.end(); ++it)
        {
			(*it)->setNoiseFactor(ballNoiseFactor);
			(*it)->setLifeSpeed(lifeSpeed);
            (*it)->update();
            sendOscInfos((*it));
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
            if((*it)->checkIfDead()==true)
            {
                 theBalls.erase(it);
                 break;
            }
        }

    for(vector< ofPtr<Ball> >::iterator it = permanentBalls.begin(); it != permanentBalls.end(); ++it)
        {
            if ((*it)->getRadius() > 180 )
            {
                (*it)->setRadius((*it)->getRadius()+ofRandom(-2,2));
            }

            else
			{
                (*it)->setRadius((*it)->getRadius()+ofRandom(2,6));
			}
			sendOscInfos(*it);
		}
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofBackground(0);

    for(vector< ofPtr<Ball> >::iterator it = permanentBalls.begin(); it != permanentBalls.end(); ++it)
        {
            (*it)->draw();
        }


    for(vector< ofPtr<Ball> >::iterator it = theBalls.begin(); it != theBalls.end(); ++it)
	{
		(*it)->draw();
		if(showLinks)
		{
			for(vector< ofPtr<Ball> >::iterator sit = it; sit != theBalls.end(); ++sit)
			{
				if((*it)->checkLink((*sit)->getRefNumber()))
				{
                    ofSetHexColor(0xffffff);
					ofVec3f p1,p2,d,n;
					if(curvedLinks)
					{
						curvePosition=ofRandom(curveFactor);
						p1=(*it)->getPosition();
						p2=(*sit)->getPosition();
						d = p2-p1;  // vecteur p1p2
						n.x = d.y;  // n : vecteur normal a d
						n.y = -d.x;
						ofPoint cp1 = ofPoint(p1 + curvePosition*d+curveAmplitude*n); // point de controle
						ofPoint cp2 = ofPoint(p1 + (1-curvePosition)*d-curveAmplitude*n);
						//ofPoint cp3 = ofPoint(p1 + 0.8*d+0.5*n);
						ofPolyline link;
						link.quadBezierTo(p1,cp1,p1+0.5*d);
						link.quadBezierTo(p1+0.5*d,cp2,p2);
						link.draw();
					}
					else
					{
						p1=(*it)->getPosition();
						p2=(*sit)->getPosition();
						ofLine(p1,p2);
					}

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
	if (key == 99||key==67)// c or C
    {
		cout<<"curvedLinks "<<ofToString(curvedLinks)<<endl;
        curvedLinks=!curvedLinks;
    }
    if (key == 80||key==112)// p or P
    {
		if(permanentBalls.size()!=0)
		{
			permanentBalls.clear();
			cout<<"permanent balls removed"<<endl;
		}
		else
		{
		cout<<"permanantBalls created "<<endl;
        addPermanentBalls();
		}
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

//--------------------------------------------------------------r
void testApp::mousePressed(int x, int y, int button)
{
	int synthNbr=attributeSynth();
	string path=pathToImages+"/"+theTextures[0];
	theBalls.push_back(ofPtr<Ball> (new Ball (refNumber,x,y,0,synthNbr,path,ballNoiseFactor,lifeSpeed,nbCircles,circleWidth,circleIncrease,50,10)));
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
void testApp::addPermanentBalls()
{
    int immortal = 1;
    int permanentBallCenterX1 = 0.25*ofGetWindowWidth();
    int permanentBallCenterX2 = 0.75*ofGetWindowWidth();
    int permanentBallCenterY = 0.5*ofGetWindowHeight();
    string path=pathToImages+"/"+theTextures[0];
    permanentBalls.push_back(ofPtr<Ball> (new Ball(100,permanentBallCenterX1,permanentBallCenterY,0,100,path,0,immortal,0,0,0,400,400)));
    permanentBalls.push_back(ofPtr<Ball> (new Ball(101,permanentBallCenterX2,permanentBallCenterY,0,101,path,0,immortal,0,0,0,400,400)));

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
            theBalls.push_back(ofPtr<Ball> (new Ball (refNumber,xVal,yVal,zPos,synthNbr,path,ballNoiseFactor,lifeSpeed,nbCircles,circleWidth,circleIncrease,50,10)));
            refNumber+=1;
        }
        else if(OscReceivedMessage.getAddress()=="/pad/2"||OscReceivedMessage.getAddress()=="/1/multixy/2")//deuxième doigt
        {
            int synthNbr=attributeSynth();
            string path=pathToImages+"/"+theTextures[synthNbr];
            theBalls.push_back(ofPtr<Ball> (new Ball (refNumber,OscReceivedMessage.getArgAsFloat(0)*ofGetWidth(),OscReceivedMessage.getArgAsFloat(1)*ofGetHeight(),zPos,synthNbr,path,ballNoiseFactor,lifeSpeed,0,0,0,50,10)));
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
		else if (OscReceivedMessage.getAddress()=="/1/ringFader/1")//nombre de cercles
		{
			nbCircles=ofMap(OscReceivedMessage.getArgAsFloat(0),0,1,0,maxCircles);
            cout<<"nbCircles :"<<nbCircles<<endl;
        }
		else if (OscReceivedMessage.getAddress()=="/1/ringFader/2")//largeur de cercles
        {
			circleWidth=ofMap(OscReceivedMessage.getArgAsFloat(0),0,1,1,maxCircleWidth);
            cout<<"CircleWidth :"<<circleWidth<<endl;
        }
		else if (OscReceivedMessage.getAddress()=="/1/ringFader/3")
		{
			circleIncrease=ofMap(OscReceivedMessage.getArgAsFloat(0),0,1,0,maxCircleIncrease);
            cout<<"circleIncrease :"<<circleIncrease<<endl;
        }
		else if (OscReceivedMessage.getAddress()=="/link/1")
		{
			curveFactor=ofMap(OscReceivedMessage.getArgAsFloat(0),0,1,0,0.5);
		}
		else if (OscReceivedMessage.getAddress()=="/link/2")
		{
			curveAmplitude=ofMap(OscReceivedMessage.getArgAsFloat(0),0,1,0,0.5);
		}
		else if (OscReceivedMessage.getAddress()=="/reset")
		{
			theBalls.clear();
			cout<<"all ball  unloaded"<<endl;
        }
		else if (OscReceivedMessage.getAddress()=="/toggleLink")
		{
			if(OscReceivedMessage.getArgAsFloat(0)==1)
			{
			cout<<"toggleLinks"<<endl;
			showLinks=!showLinks;
			}
        }
		else if (OscReceivedMessage.getAddress()=="/curvedLinks")
		{
			if(OscReceivedMessage.getArgAsFloat(0)==1)
			{
				cout<<"toggleCurvedLinks"<<endl;
				curvedLinks=!curvedLinks;
			}
        }
		else if (OscReceivedMessage.getAddress()=="/toggleCircles")
		{
			for(vector< ofPtr<Ball> >::iterator it = theBalls.begin(); it != theBalls.end(); ++it)
			{
				(*it)->removeCircles();
			}
        }
        else
		{
			cout<<"I don't know this message :"<<ofToString(OscReceivedMessage.getAddress())<<endl;
		}
    }
}
void testApp::sendOscGeneral()
{
	string address="/Links";
    ofxOscMessage msgToSend = ofxOscMessage();
    msgToSend.setAddress(address);
	if(curvedLinks==true)
	{
	msgToSend.addFloatArg(curvePosition*2);
	msgToSend.addFloatArg(curveFactor*2);
	}
	else
	{
	msgToSend.addFloatArg(0);
	msgToSend.addFloatArg(0);
	}
	oscSender.sendMessage(msgToSend);
	address="/BallGeneral";
    msgToSend.setAddress(address);
	msgToSend.addFloatArg(theBalls.size());
	oscSender.sendMessage(msgToSend);

}
//fonction qui va envoyer par osc tout les paramètres de la balle.
void testApp::sendOscInfos(ofPtr<Ball>& ballToSend)
{
    int synthNumber=ballToSend->getSynthNumber();
    string address="/ball"+ofToString(synthNumber);
    ofxOscMessage msgToSend = ofxOscMessage();
    msgToSend.setAddress(address);
    //-----
	float dead;
	if (ballToSend->checkIfDead())
	{
		dead=0.0;
	}
	else
	{
		dead=1.0;
	}
	msgToSend.addFloatArg(dead);
	msgToSend.addFloatArg(ballToSend->getRadius());
    //msgToSend.addFloatArg(ballToSend->getPosition().x);
    //msgToSend.addFloatArg(ballToSend->getPosition().y);
    //msgToSend.addFloatArg(ballToSend->getPosition().z);
	//msgToSend.addFloatArg(ballToSend->getVelocity().x);
    //msgToSend.addFloatArg(ballToSend->getVelocity().y);
    //msgToSend.addFloatArg(ballToSend->getVelocity().z);
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
	maxCircles=configFile.getIntValue("maxCircles");
	maxCircleWidth=configFile.getIntValue("maxCircleWidth");
	maxCircleIncrease=configFile.getIntValue("maxCircleIncrease");
	minDistToLink=configFile.getIntValue("minDistToLink");
	maxDistToUnlink=configFile.getIntValue("maxDistToUnlink");
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
    int nbTextures=configFile.getNumChildren()-1;
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
