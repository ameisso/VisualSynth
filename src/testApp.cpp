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
	oscFeedBack.setup(oscControlerIP,oscControlerPort);
    for (int i=0; i<nbSynthsForBalls;i++)
    {
        synthsForBalls.push_back(true);
    }
    ballNoiseFactor=0;
    lifeSpeed=1;//varie entre 0.9 et 1
	circleWidth=0;
	circleIncrease=0;
	nbCircles=0;
	curvedLinks=false;
	curveFactor=0;
	curvePosition=0;
	curveAmplitude=0;
    showLinks=false;
	linkColorHue=255;
	linkColorSaturation=0;
	linkColorBrightness=255;
	circleBrightness=0;
	ballBrightness=0;
	useCam=false;
	sendFeedback();
    cout<<"END OF INIT"<<endl;
}

//--------------------------------------------------------------
void testApp::update()
{
    receiveOscMessage();
	sendOscGeneral();
	sendFeedback();
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
	if(useCam)
	{
	cam.begin();
		if (camMove)//si la camera bouge
		{
			camCurrentPos+=camSpeed*camPos;
			cam.move(camCurrentPos);
			camCurrentLookat+=camSpeed*camLookAt;
			cam.lookAt(camCurrentLookat);
			cout<<"camSpeed : "<<camSpeed<<endl;
		}
	}
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
                    ofSetColor(linkColor);
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
	if(useCam)
	{
    cam.end();
	}
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
		setup();
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
	cam.lookAt(ofVec3f(x,y,x));
}

//--------------------------------------------------------------r
void testApp::mousePressed(int x, int y, int button)
{
	int synthNbr=attributeSynth();
	string path=pathToImages+"/"+theTextures[0];
	theBalls.push_back(ofPtr<Ball> (new Ball (refNumber,x,y,0,synthNbr,path,ballNoiseFactor,lifeSpeed,nbCircles,circleWidth,circleIncrease,50,10,ballBrightness)));
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
			//cout<<"first Finger"<<OscReceivedMessage.getArgAsFloat(0)<<endl;
            //Si jamais, on pourra toujours atribuer des param�tres diff�rents en fonction du nombre de doigts.
            float xVal=OscReceivedMessage.getArgAsFloat(1)*ofGetWidth();
            float yVal=ofGetHeight()-OscReceivedMessage.getArgAsFloat(0)*ofGetHeight();
			if(lastBallPosition.x!=xVal&&lastBallPosition.y!=yVal)
			{
				lastBallPosition.x=xVal;
				lastBallPosition.y=yVal;
				//cout<<xVal<<" "<<yVal<<endl;
				int synthNbr=attributeSynth();
				string path=pathToImages+"/"+theTextures[synthNbr];
				theBalls.push_back(ofPtr<Ball> (new Ball (refNumber,xVal,yVal,zPos,synthNbr,path,ballNoiseFactor,lifeSpeed,nbCircles,circleWidth,circleIncrease,50,10,ballBrightness)));
				refNumber+=1;
				ofxOscMessage msgToSend = ofxOscMessage();
				msgToSend.setAddress("/newBall");
				msgToSend.addFloatArg(refNumber);
				oscSender.sendMessage(msgToSend);
			}
        }
        else if(OscReceivedMessage.getAddress()=="/pad/2"||OscReceivedMessage.getAddress()=="/1/multixy/2")//deuxi�me doigt
        {
            int synthNbr=attributeSynth();
            string path=pathToImages+"/"+theTextures[synthNbr];
            theBalls.push_back(ofPtr<Ball> (new Ball (refNumber,OscReceivedMessage.getArgAsFloat(1)*ofGetWidth(),ofGetHeight()-OscReceivedMessage.getArgAsFloat(0)*ofGetHeight(),zPos,synthNbr,path,ballNoiseFactor,lifeSpeed,0,0,0,50,10,ballBrightness)));
            refNumber+=1;
        }
		else if (OscReceivedMessage.getAddress()=="/1/multifader1/1")//dur�e de vie de la balle
        {
            lifeSpeed=ofMap(OscReceivedMessage.getArgAsFloat(0),0,1,0.9,1);
            cout<<"lifeSpeed :"<<lifeSpeed<<endl;
        }
        else if (OscReceivedMessage.getAddress()=="/1/multifader1/2")//vitesse de d�placement
        {
			ballNoiseFactor=ofMap(OscReceivedMessage.getArgAsFloat(0),0,1,0,200);
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
			circleWidth=ofMap(OscReceivedMessage.getArgAsFloat(0),0,1,2,maxCircleWidth);
            cout<<"CircleWidth :"<<circleWidth<<endl;
			for(vector< ofPtr<Ball> >::iterator it = theBalls.begin(); it != theBalls.end(); ++it)
			{
				(*it)->setCircleWidth(circleWidth);
			}
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
			setup();//on r�tablit l'�tat initial.
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
		else if (OscReceivedMessage.getAddress()=="/toggleCam")
		{
			if(OscReceivedMessage.getArgAsFloat(0)==1)
			{
				
				useCam=!useCam;
				if (useCam)
				{
					cam.setupPerspective();
					cout<<"cam enabled @ "<<camPos<<" viewing "<<camLookAt<<endl;
					camCurrentPos.zero();
					camCurrentLookat.zero();
					//cam.setPosition(0,0,0); // where are we?
					//cam.lookAt(ofVec3f(0,0,500)); // what are we looking at?
					ofxOscMessage msgToSend = ofxOscMessage();
					msgToSend = ofxOscMessage();//attention, r�initialise le message.
					msgToSend.setAddress("/xyPos");
					msgToSend.addFloatArg(0);
					msgToSend.addFloatArg(0);
					oscFeedBack.sendMessage(msgToSend);
					msgToSend = ofxOscMessage();//attention, r�initialise le message.
					msgToSend.setAddress("/xyLookat");
					msgToSend.addFloatArg(0);
					msgToSend.addFloatArg(0);
					oscFeedBack.sendMessage(msgToSend);
					msgToSend = ofxOscMessage();//attention, r�initialise le message.
					msgToSend.setAddress("/zPos");
					msgToSend.addFloatArg(0);
					oscFeedBack.sendMessage(msgToSend);
					msgToSend = ofxOscMessage();//attention, r�initialise le message.
					msgToSend.setAddress("/zLookat");
					msgToSend.addFloatArg(0);
					msgToSend.addFloatArg(0);
					oscFeedBack.sendMessage(msgToSend);
					camMove=false;
					msgToSend = ofxOscMessage();//attention, r�initialise le message.
					msgToSend.setAddress("/camMove");
					msgToSend.addFloatArg(0);
					oscFeedBack.sendMessage(msgToSend);
				}
				else
				{
					cout<<"cam disabled"<<endl;
				}
			}
        }
		else if(OscReceivedMessage.getAddress()=="/brightness/1")//intensit� des balles
		{
			ballBrightness=ofMap(OscReceivedMessage.getArgAsFloat(0),0,1,0,255);
			for(vector< ofPtr<Ball> >::iterator it = theBalls.begin(); it != theBalls.end(); ++it)
			{
				(*it)->setBrightness(ballBrightness);
			}
			cout<<"ball brightness : "<<ballBrightness<<endl;
		}
		else if(OscReceivedMessage.getAddress()=="/brightness/2")//intensit� des liens
		{
			linkColorBrightness=ofMap(OscReceivedMessage.getArgAsFloat(0),0,1,0,255);
			linkColor.setBrightness(linkColorBrightness);
			cout<<"link brightness : "<<linkColorBrightness<<endl;
		}
		else if(OscReceivedMessage.getAddress()=="/brightness/3")//intensit� des cercles
		{
			circleBrightness=ofMap(OscReceivedMessage.getArgAsFloat(0),0,1,0,255);
			for(vector< ofPtr<Ball> >::iterator it = theBalls.begin(); it != theBalls.end(); ++it)
			{
				(*it)->setCircleBrightness(circleBrightness);
			}
			cout<<"circle brightness : "<<circleBrightness<<endl;
		}
		else if(OscReceivedMessage.getAddress()=="/xyPos")
		{
			camPos.x=OscReceivedMessage.getArgAsFloat(0);
			camPos.y=OscReceivedMessage.getArgAsFloat(1);
		}
		else if(OscReceivedMessage.getAddress()=="/xyLookat")		{
			
			camLookAt.x=OscReceivedMessage.getArgAsFloat(0);
			camLookAt.y=OscReceivedMessage.getArgAsFloat(1);
		}
		else if(OscReceivedMessage.getAddress()=="/zPos")
		{
			camPos.z=OscReceivedMessage.getArgAsFloat(0);
			cout<<"camera zPos : "<<camPos.z<<endl;
		}
		else if(OscReceivedMessage.getAddress()=="/zLookat")
		{
			camLookAt.z=OscReceivedMessage.getArgAsFloat(0);
			cout<<"camera zLookat : "<<camLookAt.z<<endl;
		}
		else if(OscReceivedMessage.getAddress()=="/camSpeed")
		{
			camSpeed=OscReceivedMessage.getArgAsFloat(0);
			cout<<"camSpeed : "<<camSpeed<<endl;
			
		}
		else if (OscReceivedMessage.getAddress()=="/camMove")
		{
			ofxOscMessage msgToSend = ofxOscMessage();
			if (OscReceivedMessage.getArgAsFloat(0)==1)
			{
				if(useCam)// on ne peut bouger que si on a la camera
				{
				cout<<"camera Running"<<endl;
				camMove=true;
				}
				else// on d�senclenche le bouton
				{
					msgToSend = ofxOscMessage();//attention, r�initialise le message.
					msgToSend.setAddress("/camMove");
					msgToSend.addFloatArg(0);
					oscFeedBack.sendMessage(msgToSend);
				}
			}
			else
			{
				camMove=false;
				cout<<"camera Stoped"<<endl;
				msgToSend = ofxOscMessage();//attention, r�initialise le message.
				msgToSend.setAddress("/camSpeed");
				msgToSend.addFloatArg(0);
				oscFeedBack.sendMessage(msgToSend);
			}
		}
        else
		{
			cout<<"I don't know this message :"<<ofToString(OscReceivedMessage.getAddress())<<" : "<<OscReceivedMessage.getArgAsFloat(0)<<endl;
		}
    }
}
//envoie le feedback sur la tablette
void testApp::sendFeedback()
{
	ofxOscMessage msgToSend = ofxOscMessage();

	//concerne les balles
	msgToSend = ofxOscMessage();//attention, r�initialise le message.
	msgToSend.setAddress("/1/multifader1/1");
	msgToSend.addFloatArg(ofMap(lifeSpeed,0.9,1,0,1));
	oscFeedBack.sendMessage(msgToSend);
	msgToSend = ofxOscMessage();//attention, r�initialise le message.
	msgToSend.setAddress("/1/multifader1/2");
	msgToSend.addFloatArg(ofMap(ballNoiseFactor,0,200,0,1));
	oscFeedBack.sendMessage(msgToSend);
	msgToSend = ofxOscMessage();//attention, r�initialise le message.
	msgToSend.setAddress("/1/multifader1/3");
	msgToSend.addFloatArg(ofMap(zPos,0,profZ,0,1));
	oscFeedBack.sendMessage(msgToSend);
	
	//concerne les cercles
	msgToSend = ofxOscMessage();
	msgToSend.setAddress("/1/ringFader/1");
	msgToSend.addFloatArg(ofMap(nbCircles,0,maxCircles,0,1));
	oscFeedBack.sendMessage(msgToSend);
	msgToSend = ofxOscMessage();
	msgToSend.setAddress("/1/ringFader/2");
	msgToSend.addFloatArg(ofMap(circleWidth,2,maxCircleWidth,0,1));
	oscFeedBack.sendMessage(msgToSend);
	msgToSend = ofxOscMessage();
	msgToSend.setAddress("/1/ringFader/3");
	msgToSend.addFloatArg(ofMap(circleIncrease,0,maxCircleIncrease,0,1));
	oscFeedBack.sendMessage(msgToSend);
	
	//concerne les liens
	msgToSend = ofxOscMessage();
	msgToSend.setAddress("/link/1");
	msgToSend.addFloatArg(ofMap(curveFactor,0,0.5,0,1));
	oscFeedBack.sendMessage(msgToSend);
	msgToSend = ofxOscMessage();
	msgToSend.setAddress("/link/2");
	msgToSend.addFloatArg(ofMap(curveAmplitude,0,0.5,0,1));
	oscFeedBack.sendMessage(msgToSend);
	
//TODO BRIGHTNESS
	msgToSend = ofxOscMessage();
	msgToSend.setAddress("/brightness/1");
	msgToSend.addFloatArg(ofMap(ballBrightness,0,255,0,1));
	oscFeedBack.sendMessage(msgToSend);
	msgToSend = ofxOscMessage();
	msgToSend.setAddress("/brightness/2");
	msgToSend.addFloatArg(ofMap(linkColorBrightness,0,255,0,1));
	oscFeedBack.sendMessage(msgToSend);
	msgToSend = ofxOscMessage();
	msgToSend.setAddress("/brightness/3");
	msgToSend.addFloatArg(ofMap(circleBrightness,0,255,0,1));
	oscFeedBack.sendMessage(msgToSend);
	
	
	//valeurs texte
	msgToSend = ofxOscMessage();
	msgToSend.setAddress("/infos");
	msgToSend.addStringArg(ofToString(theBalls.size())+" balls @ "+ofToString(ofGetFrameRate())+" FPS");
	oscFeedBack.sendMessage(msgToSend);
	msgToSend = ofxOscMessage();
	msgToSend.setAddress("/linkCurve");
	msgToSend.addFloatArg(curveFactor);
	oscFeedBack.sendMessage(msgToSend);
	msgToSend = ofxOscMessage();
	msgToSend.setAddress("/linkAmpl");
	msgToSend.addFloatArg(curveAmplitude);
	oscFeedBack.sendMessage(msgToSend);
	msgToSend = ofxOscMessage();
	msgToSend.setAddress("/nbCircles");
	msgToSend.addIntArg(nbCircles);
	oscFeedBack.sendMessage(msgToSend);
	msgToSend = ofxOscMessage();
	msgToSend.setAddress("/circleWidth");
	msgToSend.addFloatArg(circleWidth);
	oscFeedBack.sendMessage(msgToSend);
	msgToSend = ofxOscMessage();
	msgToSend.setAddress("/circleSpacing");
	msgToSend.addFloatArg(circleIncrease);
	oscFeedBack.sendMessage(msgToSend);
	msgToSend = ofxOscMessage();
	msgToSend.setAddress("/lifeS");
	msgToSend.addFloatArg(lifeSpeed);
	oscFeedBack.sendMessage(msgToSend);
	msgToSend = ofxOscMessage();
	msgToSend.setAddress("/speed");
	msgToSend.addFloatArg(ballNoiseFactor);
	oscFeedBack.sendMessage(msgToSend);
	msgToSend = ofxOscMessage();
	msgToSend.setAddress("/Zpos");
	msgToSend.addFloatArg(zPos);
	oscFeedBack.sendMessage(msgToSend);

	//cout<<"feedback sended"<<endl;
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
	msgToSend.addFloatArg(0.0);
	msgToSend.addFloatArg(0.0);
	}
	oscSender.sendMessage(msgToSend);
	ofxOscMessage msgToSend4 = ofxOscMessage();
	address="/LinkGeneral";
    msgToSend4.setAddress(address);
	msgToSend4.addFloatArg(lifeSpeed);
	msgToSend4.addFloatArg(ballNoiseFactor);
	msgToSend4.addFloatArg(zPos);
	oscSender.sendMessage(msgToSend4);
	ofxOscMessage msgToSend2 = ofxOscMessage();//! pour l'osc il faut cr�er un nouveau message � chaque fois.
	address="/CirclesGeneral";
    msgToSend2.setAddress(address);
	if(nbCircles!=0)
	{
		msgToSend2.addFloatArg((float)nbCircles);
		msgToSend2.addFloatArg((float)circleWidth);
		msgToSend2.addFloatArg((float)circleIncrease);
	}
	else
	{
		msgToSend2.addFloatArg(0.0);
		msgToSend2.addFloatArg(0.0);
		msgToSend2.addFloatArg(0.0);
	}

	oscSender.sendMessage(msgToSend2);
	ofxOscMessage msgToSend3 = ofxOscMessage();
	address="/BallGeneral";
    msgToSend3.setAddress(address);
	msgToSend3.addFloatArg((float)theBalls.size());
	msgToSend3.addFloatArg(lifeSpeed);
	msgToSend3.addFloatArg(ballNoiseFactor);
	msgToSend3.addFloatArg(zPos);
	oscSender.sendMessage(msgToSend3);


}
//fonction qui va envoyer par osc tout les param�tres de la balle.
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
    //ajouter ici les autre param�tres pour envoyer toutes les infos dans une seule trame ...apr�s tout l'osc c'est fait pour ca.
    //oscSender.sendMessage(msgToSend);
	//cout<<"Sended : "<<value<<" @"<< address<<endl;
}
int testApp::attributeSynth()
{
    //!!! on ne pourra pas cr�er plus de balles que de synth�s !
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
	oscControlerIP=configFile.getValue("controlerIp");
    cout<<"oscControlerIP :"<<oscControlerIP<<endl;
	oscControlerPort=configFile.getIntValue("controlerPort");
    cout<<"oscControlerPort :"<<oscControlerPort<<endl;
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
