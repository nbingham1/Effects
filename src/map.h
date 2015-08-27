#include "vector.h"
#include "standard.h"
#include "graphics.h"
#include "material.h"
#include "mathdef.h"

#ifndef map_h
#define map_h

struct drip
{
	vec position;
	vec velocity;
	float size;

	drip *next, *prev;
};

struct liquid
{
	drip *first, *last;

	void new_drip(vec p, vec v, float s);
	void del_drip(drip *d);

	void init();
	void render();
	void release();
};

struct volfoliagehdl
{
	GLuint near_l;
	materialhdl near_m;

	GLuint top_l;
	materialhdl top_m;

	GLuint right_l;
	materialhdl right_m;

	GLuint left_l;
	materialhdl left_m;

	void load(char *filename);
	void grass(double length, double width, double rigidity, int size_h);
	void render(int density, vec position, vec normal, vec camera);
	void release();
};

struct selfoliagehdl
{
	GLuint near_l;
	materialhdl near_m;

	GLuint far_l;
	materialhdl far_m;

};

struct maphdl
{
	double *verts;
	double *texts;
	double *norms;
	unsigned int *indices;

	int width;
	int height;

	int *density;
	materialhdl base;
	volfoliagehdl cover;
	liquid water;

	void sphere_collide(vec *position, vec *velocity, float size);
	void load(int w, int h);
	void load(char *filename);
	void render(vec camera);
	void release();
};

#endif
