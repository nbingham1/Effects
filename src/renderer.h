#include "display.h"
#include "camera.h"

#include "map.h"

#ifndef renderer_h
#define renderer_h

struct renderhdl
{
	displayhdl display;

	camerahdl  camera;

	maphdl map;

	void init();
	void release();

	void displayf();
};

#endif
