#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){


	#ifdef _USE_LIVE_VIDEO
        vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(320,240);
		width = vidGrabber.width;
		height = vidGrabber.height;
	
	#else
        vidPlayer.loadMovie("silhouettes-sorenson.mov");
        vidPlayer.play();
		width = vidPlayer.width;
		height = vidPlayer.height;
	
	#endif

    colorImg.allocate(width,height);
	grayImage.allocate(width,height);
	grayBg.allocate(width,height);
	grayDiff.allocate(width,height);
	
	grayAdv.allocate(width, height);
	
	CS = new centroidSkel(6,width, height);
	CS->loadFromXML("centroidSkelPamars.xml");
	
	bLearnBakground = true;
	threshold = 80;
	
	bPaused = false;
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(100,100,100);

    bool bNewFrame = false;

	#ifdef _USE_LIVE_VIDEO
       vidGrabber.grabFrame();
	   bNewFrame = vidGrabber.isFrameNew();
    #else
        vidPlayer.idleMovie();
        bNewFrame = vidPlayer.isFrameNew();
	#endif

	if (bNewFrame){

		#ifdef _USE_LIVE_VIDEO
            colorImg.setFromPixels(vidGrabber.getPixels(), width,height);
	    #else
            colorImg.setFromPixels(vidPlayer.getPixels(), width,height);
        #endif

        grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff = grayImage;
		grayDiff.invert();
		//grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 2, (340*height)/3, 10, false);	// find holes
		
		if (contourFinder.nBlobs > 0){
			grayAdv.set(0);
			grayAdv.drawBlobIntoMe(contourFinder.blobs[0], 255);
			
			CS->computeCentroid(grayAdv.getPixels());
		}
	}

	
}

//--------------------------------------------------------------
void testApp::draw(){

	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetColor(0xffffff);
	
	
	grayDiff.draw(0,0);
	
	ofEnableAlphaBlending();
	ofSetColor(255,0,0,100);
	grayAdv.draw(0,0);
	// then draw the contours:

	CS->draw();
	
	// we could draw the whole contour finder
	//contourFinder.draw(360,540);

	// or, instead we can draw each blob individually,
	// this is how to get access to them:
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(0,0);
    }

	// finally, a report:

	ofSetColor(0xffffff);
	char reportStr[1024];
	sprintf(reportStr, "bg subtraction and blob detection\npress ' ' to capture bg\nthreshold %i (press: +/-)\nnum blobs found %i", threshold, contourFinder.nBlobs);
	ofDrawBitmapString(reportStr, 20, 600);

}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){

	switch (key){
		case ' ':
			bLearnBakground = true;
			break;
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
	}
	
	/*
	switch (key){
			
		case 'A':
			bPaused = !bPaused;
			if (bPaused == true){
				fPausedSpeed = (vidPlayer).getSpeed();
				(vidPlayer).setSpeed(0);
			} else {
				(vidPlayer).setSpeed(fPausedSpeed);
			}
			break;
		case 'S':
			CS->bDrawInOrder = !CS->bDrawInOrder;
			break;
		case 'D':
			CS->bDrawingLines = !CS->bDrawingLines;
			break;
		case 'F':
			CS->bLinesVisible = !CS->bLinesVisible;
			break;
		case 'G':
			CS->bPointsVisible = !CS->bPointsVisible;
			break;
		case 'O':
			CS->saveToXML("centroidSkelPamars.xml");
			break;
		case 'I':
			CS->loadFromXML("centroidSkelPamars.xml");
			break;
		case 'X':
			CS->clearAllLines();
			break;
			
			
	}
	 */
}


//---------------------------------------------------------------
void testApp::mouseMoved( int x, int y ){
	
	/*if (bPaused != true){
		(vidPlayer).setSpeed( ((x - 512) / 1024.0f) * 3.0f);
	}*/
}

//---------------------------------------------------------------
void testApp::mouseDragged( int x, int y, int button){
	
}

//---------------------------------------------------------------
void testApp::mousePressed( int x, int y, int button ){
	//bDown = true;
	//CS->handleMouseDown(x,y);
}

//---------------------------------------------------------------
void testApp::mouseReleased  ( int x, int y, int button ){
	//bDown = false;
	//CS->handleMouseUp(x,y);
}





//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

