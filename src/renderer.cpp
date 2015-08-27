#include "renderer.h"

void renderhdl::init()
{
	map.load("heightmap.tga");

	display.init();
	camera.init(&display.framespersecond);
	camera.position.y = -200.0;
}

void renderhdl::release()
{
	printf("%f", display.framespersecond);
	map.release();
}

void renderhdl::displayf()
{
	if (display.vsync())
	{
		glClearColor(0.2, 0.2, 1.0, 0.0);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		camera.setproj();

		GLfloat light_position[] = { 0.0, 1000000.0, 0.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		map.render(-1.0*camera.position);

		display.render();

		glutSwapBuffers();
	}
}
