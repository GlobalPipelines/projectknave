#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "cvManager.h"
#include "sceneManager.h"
#include "FerryBuilding.h"

enum {
	
	DRAW_SCENE, DRAW_CV
	
};




class testApp : public ofBaseApp{
	
public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void resized(int w, int h);
	
	cvManager				CVM;
	sceneManager			SM;
	FerryBuilding			FB;
	int						drawMode;
	
	
	
	
};

#endif
