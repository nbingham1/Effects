#include "map.h"

void liquid::new_drip(vec p, vec v, float s)
{
	drip *d = new drip;
	d->position = p;
	d->velocity = v;
	d->size = s;
	d->next = NULL;
	d->prev = NULL;
	if (first == NULL)
	{
		first = d;
		last = d;
	}
	else
	{
		last->next = d;
		d->prev = last;
		last = last->next;
	}
}

void liquid::del_drip(drip *d)
{
	if (first == d)
		first = d->next;
	if (last == d)
		last = d->prev;
	drip *p = d->prev;
	if (p)
		p->next = d->next;
	if (d->next)
		d->next->prev = p;

	delete d;
	d = NULL;
}

void liquid::init()
{
	first = NULL;
	last = NULL;
}

void liquid::render()
{
	drip *curr = first;
	glPointSize(curr->size*10);
	glBegin(GL_POINTS);
	while (curr != NULL)
	{
		curr->position += curr->velocity;
		glVertex3f(curr->position.x, curr->position.y, curr->position.z);
		curr = curr->next;
	}
	glEnd();
}

void liquid::release()
{
	drip *curr = first, *prev;
	while (curr != NULL)
	{
		prev = curr;
		curr = curr->next;
		delete prev;
		prev = NULL;
	}
}

void volfoliagehdl::load(char *filename)
{
}

void volfoliagehdl::grass(double length, double width, double rigidity, int size_h)
{
/****************************************************
 * 													*
 * 													*
 * 						NEAR						*
 * 													*
 * 													*
 ****************************************************/
	int numpoints = size_h*2;
	int size;
	int numblades = 400;

	float *verts = new float[numblades*numpoints*3];
	float *texts = new float[numblades*numpoints*2];
	float *norms = new float[numblades*numpoints*3];
	unsigned char *data;
	unsigned int *indices = new unsigned int[2*size_h*numblades];
	float rigid;
	float rwidth;
	float rlength;

	for (int a = 0; a < numblades; a++)
	{
		rigid = ((float(rand()%101)/400.0 - .25) + rigidity)*m_pi/2.0;
		rwidth = ((float(rand()%101)/400.0 - .25) + width);
		rlength = ((float(rand()%101)/400.0 - .25) + length);

		float s;
		float l;

		for (int x = 0; x < 2; x++)
		{
			s = x == 0 ? -1.0 : 1.0;
			for (int y = 0; y < size_h; y++)
			{
				l = float(y)/float(size_h-1);
				verts[a*numpoints*3 + (x*size_h + y)*3 + 0] = rwidth*s*.2;
				verts[a*numpoints*3 + (x*size_h + y)*3 + 1] = sin(m_pi/2.0 - rigid*l)*rlength*l;
				verts[a*numpoints*3 + (x*size_h + y)*3 + 2] = cos(m_pi/2.0 - rigid*l)*rlength*l;

				texts[a*numpoints*2 + (x*size_h + y)*2 + 0] = l;
				texts[a*numpoints*2 + (x*size_h + y)*2 + 1] = float(x);
			}
		}

		verts[a*numpoints*3 + 1] -= 4.0;
		verts[a*numpoints*3 + size_h*3 + 1] -= 4.0;

		int index = 0;
		for (int x = 1; x < 2; x++)
		{
			for (int y = 0; y < size_h; y++)
			{
				indices[2*size_h*a + index++] = x*size_h + y;
				indices[2*size_h*a + index++] = (x-1)*size_h + y;
			}
		}

		vec n;
		double count;
		for (int x = 0; x < 2; x++)
			for (int y = 0; y < size_h; y++)
			{
				count = 0.0;
				n = vec();
				if (y-1 >= 0 && x-1 >= 0)
				{
					n += normvec(vec(&verts[a*numpoints*3 + (x*size_h + (y-1))*3]), vec(&verts[a*numpoints*3 + (x*size_h + y)*3]), vec(&verts[a*numpoints*3 + ((x-1)*size_h + y)*3]));
					n += normvec(vec(&verts[a*numpoints*3 + (x*size_h + (y-1))*3]), vec(&verts[a*numpoints*3 + (x*size_h + y)*3]), vec(&verts[a*numpoints*3 + ((x-1)*size_h + y)*3]));
					count+=2.0;
				}
				if (x-1 >= 0 && y+1 <= size_h)
				{
					n += normvec(vec(&verts[a*numpoints*3 + ((x-1)*size_h + y)*3]), vec(&verts[a*numpoints*3 + (x*size_h + y)*3]), vec(&verts[a*numpoints*3 + ((x-1)*size_h + (y+1))*3]));
					n += normvec(vec(&verts[a*numpoints*3 + ((x-1)*size_h + (y+1))*3]), vec(&verts[a*numpoints*3 + (x*size_h + y)*3]), vec(&verts[a*numpoints*3 + (x*size_h + (y+1))*3]));
					count+=2.0;
				}
				if (y+1 <= int(length + 1.0) && x+1 < 2)
				{
					n += normvec(vec(&verts[a*numpoints*3 + (x*size_h + (y+1))*3]), vec(&verts[a*numpoints*3 + (x*size_h + y)*3]), vec(&verts[a*numpoints*3 + ((x+1)*size_h + y)*3]));
					count+=1.0;
				}
				if (x+1 < 2 && y-1 >= 0)
				{
					n += normvec(vec(&verts[a*numpoints*3 + ((x+1)*size_h + y)*3]), vec(&verts[a*numpoints*3 + (x*size_h + y)*3]), vec(&verts[a*numpoints*3 + (x*size_h + (y-1))*3]));
					count+=1.0;
				}
				n /= count;
				norms[a*numpoints*3 + (x*size_h + y)*3 + 0] = -n.x;
				norms[a*numpoints*3 + (x*size_h + y)*3 + 1] = -n.y;
				norms[a*numpoints*3 + (x*size_h + y)*3 + 2] = -n.z;
			}
	}

	near_l = glGenLists(1);
	glNewList(near_l, GL_COMPILE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, verts);
	glTexCoordPointer(2, GL_FLOAT, 0, texts);
	glNormalPointer(GL_FLOAT, 0, norms);
	vec col;
	for (int x = 0; x < numblades; x++)
	{
		glPushMatrix();
		glTranslatef(double(rand()%101)/10.0 - 5, 0.0, double(rand()%101)/10.0 - 5);
		glRotatef(double(rand()%40) - 20.0, 1.0, 0.0, 1.0);
		glRotatef(double(rand()%360), 0.0, 1.0, 0.0);
		//r = double(rand()%100)/200.0 + .5;
		//glScalef(r, r, r);
		glDrawElements(GL_QUAD_STRIP, size_h*2, GL_UNSIGNED_INT, &indices[x*size_h*2]);
		glPopMatrix();
	}

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glEndList();

	data = rgba_tga((char*)"blade.tga", (char*)"blade_m.tga", &size, &size);

	near_m.init((char*)"apptexl.vx", (char*)"apptexl.ft");
	near_m.addtex(data, size, size, 1, GL_RGBA, true);
	delete [] data;

	delete [] verts;
	delete [] norms;
	delete [] texts;
	delete [] indices;

/****************************************************
 * 													*
 * 													*
 * 						RIGHT						*
 * 													*
 * 													*
 ****************************************************/
	int dens = 3;
	verts = new float[4*dens*3];
	texts = new float[4*dens*2];
	norms = new float[4*dens*3];

	for (int x = 0; x < dens; x++)
	{
		verts[(x*4 + 0)*3 + 0] = -5.0;
		verts[(x*4 + 0)*3 + 1] = -1.0;
		verts[(x*4 + 0)*3 + 2] = 10.0*float(x)/float(dens-1) - 5.0;

		verts[(x*4 + 1)*3 + 0] = 5.0;
		verts[(x*4 + 1)*3 + 1] = -1.0;
		verts[(x*4 + 1)*3 + 2] = 10.0*float(x)/float(dens-1) - 5.0;

		verts[(x*4 + 2)*3 + 0] = 5.0;
		verts[(x*4 + 2)*3 + 1] = length;
		verts[(x*4 + 2)*3 + 2] = 10.0*float(x)/float(dens-1) - 5.0;

		verts[(x*4 + 3)*3 + 0] = -5.0;
		verts[(x*4 + 3)*3 + 1] = length;
		verts[(x*4 + 3)*3 + 2] = 10.0*float(x)/float(dens-1) - 5.0;

		texts[(x*4 + 0)*2 + 0] = 0.0;
		texts[(x*4 + 0)*2 + 1] = 0.0;

		texts[(x*4 + 1)*2 + 0] = 1.0;
		texts[(x*4 + 1)*2 + 1] = 0.0;

		texts[(x*4 + 2)*2 + 0] = 1.0;
		texts[(x*4 + 2)*2 + 1] = 1.0;

		texts[(x*4 + 3)*2 + 0] = 0.0;
		texts[(x*4 + 3)*2 + 1] = 1.0;

		norms[(x*4 + 0)*3 + 0] = 0.0;
		norms[(x*4 + 0)*3 + 1] = 1.0;
		norms[(x*4 + 0)*3 + 2] = 0.0;

		norms[(x*4 + 1)*3 + 0] = 0.0;
		norms[(x*4 + 1)*3 + 1] = 1.0;
		norms[(x*4 + 1)*3 + 2] = 0.0;

		norms[(x*4 + 2)*3 + 0] = 0.0;
		norms[(x*4 + 2)*3 + 1] = 1.0;
		norms[(x*4 + 2)*3 + 2] = 0.0;

		norms[(x*4 + 3)*3 + 0] = 0.0;
		norms[(x*4 + 3)*3 + 1] = 1.0;
		norms[(x*4 + 3)*3 + 2] = 0.0;
	}

	right_l = glGenLists(1);
	glNewList(right_l, GL_COMPILE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, verts);
	glTexCoordPointer(2, GL_FLOAT, 0, texts);
	glNormalPointer(GL_FLOAT, 0, norms);
	glDrawArrays(GL_QUADS, 0, 4*dens);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glEndList();

	right_m.init((char*)"apptex.vx", (char*)"apptex.ft");
	right_m.blank(size, size, 1, false);

	glDisable(GL_CULL_FACE);
	right_m.srend(right_m.first, 0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0/10.0, 1.0/10.0, -1.0/40.0, 1.0/40.0, .2, 999999999);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0.0, 0.0, -15.0);

	GLfloat light_position[] = { 0.0, 1000000.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	near_m.bind();
	glCallList(near_l);

	right_m.erend(right_m.first, GL_RGBA, 0);

	delete [] verts;
	delete [] texts;
/****************************************************
 * 													*
 * 													*
 * 						LEFT						*
 * 													*
 * 													*
 ****************************************************/
	verts = new float[4*dens*3];
	texts = new float[4*dens*2];

	for (int x = 0; x < dens; x++)
	{
		verts[(x*4 + 0)*3 + 0] = 10.0*float(x)/float(dens-1) - 5.0;
		verts[(x*4 + 0)*3 + 1] = -1.0;
		verts[(x*4 + 0)*3 + 2] = -5.0;

		verts[(x*4 + 1)*3 + 0] = 10.0*float(x)/float(dens-1) - 5.0;
		verts[(x*4 + 1)*3 + 1] = -1.0;
		verts[(x*4 + 1)*3 + 2] = 5.0;

		verts[(x*4 + 2)*3 + 0] = 10.0*float(x)/float(dens-1) - 5.0;
		verts[(x*4 + 2)*3 + 1] = length;
		verts[(x*4 + 2)*3 + 2] = 5.0;

		verts[(x*4 + 3)*3 + 0] = 10.0*float(x)/float(dens-1) - 5.0;
		verts[(x*4 + 3)*3 + 1] = length;
		verts[(x*4 + 3)*3 + 2] = -5.0;

		texts[(x*4 + 0)*2 + 0] = 0.0;
		texts[(x*4 + 0)*2 + 1] = 0.0;

		texts[(x*4 + 1)*2 + 0] = 1.0;
		texts[(x*4 + 1)*2 + 1] = 0.0;

		texts[(x*4 + 2)*2 + 0] = 1.0;
		texts[(x*4 + 2)*2 + 1] = 1.0;

		texts[(x*4 + 3)*2 + 0] = 0.0;
		texts[(x*4 + 3)*2 + 1] = 1.0;
	}

	left_l = glGenLists(1);
	glNewList(left_l, GL_COMPILE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, verts);
	glTexCoordPointer(2, GL_FLOAT, 0, texts);
	glNormalPointer(GL_FLOAT, 0, norms);
	glDrawArrays(GL_QUADS, 0, 4*dens);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glEndList();

	left_m.init((char*)"apptex.vx", (char*)"apptex.ft");
	left_m.blank(size, size, 1, false);
	left_m.srend(left_m.first, 0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0/10.0, 1.0/10.0, -1.0/40.0, 1.0/40.0, .2, 999999999);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0.0, 0.0, -15.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	near_m.bind();
	glCallList(near_l);

	left_m.erend(left_m.first, GL_RGBA, 0);

	delete [] norms;
	delete [] verts;
	delete [] texts;

/****************************************************
 * 													*
 * 													*
 * 						TOP							*
 * 													*
 * 													*
 ****************************************************/
	dens = 1;
	verts = new float[4*dens*3];
	texts = new float[4*dens*2];

	double scale = .6;

	for (int x = 0; x < dens; x++)
	{
		verts[(x*4 + 0)*3 + 0] = -5.3;
		verts[(x*4 + 0)*3 + 1] = length*scale*double(x+1)/double(dens);//0.5*length*(1.0 - sin(m_pi/2.0 - rigid)*float(x)/float(dens));// + length*sin(m_pi/2.0 - rigid)/float(dens + 1);
		verts[(x*4 + 0)*3 + 2] = -5.3;

		verts[(x*4 + 1)*3 + 0] = -5.3;
		verts[(x*4 + 1)*3 + 1] = length*scale*double(x+1)/double(dens);//0.5*length*(1.0 - sin(m_pi/2.0 - rigid)*float(x)/float(dens));// + length*sin(m_pi/2.0 - rigid)/float(dens + 1);
		verts[(x*4 + 1)*3 + 2] = 5.3;

		verts[(x*4 + 2)*3 + 0] = 5.3;
		verts[(x*4 + 2)*3 + 1] = length*scale*double(x+1)/double(dens);//0.5*length*(1.0 - sin(m_pi/2.0 - rigid)*float(x)/float(dens));// + length*sin(m_pi/2.0 - rigid)/float(dens + 1);
		verts[(x*4 + 2)*3 + 2] = 5.3;

		verts[(x*4 + 3)*3 + 0] = 5.3;
		verts[(x*4 + 3)*3 + 1] = length*scale*double(x+1)/double(dens);//0.5*length*(1.0 - sin(m_pi/2.0 - rigid)*float(x)/float(dens));// + length*sin(m_pi/2.0 - rigid)/float(dens + 1);
		verts[(x*4 + 3)*3 + 2] = -5.3;

		texts[(x*4 + 0)*2 + 0] = 0.0;
		texts[(x*4 + 0)*2 + 1] = 0.0;

		texts[(x*4 + 1)*2 + 0] = 0.0;
		texts[(x*4 + 1)*2 + 1] = 1.0;

		texts[(x*4 + 2)*2 + 0] = 1.0;
		texts[(x*4 + 2)*2 + 1] = 1.0;

		texts[(x*4 + 3)*2 + 0] = 1.0;
		texts[(x*4 + 3)*2 + 1] = 0.0;
	}

	top_l = glGenLists(1);
	glNewList(top_l, GL_COMPILE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, verts);
	glTexCoordPointer(2, GL_FLOAT, 0, texts);
	glDrawArrays(GL_QUADS, 0, 4*dens);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glEndList();

	top_m.init((char*)"apptexl.vx", (char*)"apptexl.ft");
	top_m.blank(size, size, 1, false);

	top_m.srend(top_m.first, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0/10.0, 1.0/10.0, -1.0/10.0, 1.0/10.0, .2, 999999999);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -10);

	glRotatef(90.0, 1.0, 0.0, 0.0);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	near_m.bind();
	for (int x = 0; x < 2; x++)
	{
		glPushMatrix();
		glRotatef(double(rand()%360), 0.0, 1.0, 0.0);
		glScalef(rand()%2 == 0 ? -1.0 : 1.0, 1.0, rand()%2 == 0 ? -1.0 : 1.0);
		glCallList(near_l);
		glPopMatrix();
	}

	top_m.erend(top_m.first, GL_RGBA, 0);

	delete [] verts;
	delete [] texts;

	glEnable(GL_CULL_FACE);
}

void volfoliagehdl::render(int density, vec position, vec normal, vec camera)
{
	double dist = dot(position - camera, position - camera);
	int d = rand();
	int r = d;

	double ax, az;

	ax = 90.0 - acos(normal.z)*180.0/m_pi;
	az = acos(normal.x)*180.0/m_pi - 90.0;

	if (dist < m_sqr(5))
	{
		r = d;
		near_m.bind();
		for (int x = 0; x < density; x++)
		{
			r *= 2411;
			glPushMatrix();
			glTranslatef(position.x, position.y + 0.50, position.z);
			glTranslatef((r%100)/100.0, 0.0, ((r*251)%100)/100.0);
			glRotatef(ax, 1.0, 0.0, 0.0);
			glRotatef(az, 0.0, 0.0, 1.0);
			glRotatef(float((1 - r%2) + (1.0 - (r*251)%2))*90.0, 0.0, 1.0, 0.0);
			glScalef(r%2 == 0 ? -1.0 : 1.0, 1.0, (r*251)%2 == 0 ? -1.0 : 1.0);
			glScalef(1.0/cos(ax*m_pi/180.0), 1.0, 1.0/cos(az*m_pi/180.0));
			glCallList(near_l);
			glPopMatrix();
		}
	}
	else if (dist > m_sqr(5) && dist < m_sqr(20))
	{
		int d1 = int(float(density)*(dist - m_sqr(5))/(m_sqr(20) - m_sqr(5)));

		r = d;

		top_m.bind();
		for (int x = 0; x < d1; x++)
		{
			r *= 2411;
			glPushMatrix();
			glTranslatef(position.x, position.y + 0.50, position.z);
			glRotatef(ax, 1.0, 0.0, 0.0);
			glRotatef(az, 0.0, 0.0, 1.0);
			glRotatef(double(r%4)*90.0, 0.0, 1.0, 0.0);
			glScalef(r%2 == 0 ? -1.0 : 1.0, 1.0, (r*251)%2 == 0 ? -1.0 : 1.0);
			glScalef(1.0/cos(ax*m_pi/180.0), 1.0, 1.0/cos(az*m_pi/180.0));
			glCallList(top_l);
			glPopMatrix();
		}

		near_m.bind();
		for (int x = 0; x < density - d1; x++)
		{
			r *= 2411;
			glPushMatrix();
			glTranslatef(position.x, position.y + 0.50, position.z);
			glTranslatef((r%100)/100.0, 0.0, ((r*251)%100)/100.0);
			glRotatef(ax, 1.0, 0.0, 0.0);
			glRotatef(az, 0.0, 0.0, 1.0);
			glRotatef(float((1 - r%2) + (1.0 - (r*251)%2))*90.0, 0.0, 1.0, 0.0);
			glScalef(r%2 == 0 ? -1.0 : 1.0, 1.0, (r*251)%2 == 0 ? -1.0 : 1.0);
			glScalef(1.0/cos(ax*m_pi/180.0), 1.0, 1.0/cos(az*m_pi/180.0));
			glCallList(near_l);
			glPopMatrix();
		}

		r = d;

		right_m.bind();
		for (int x = 0; x < d1; x++)
		{
			r *= 2411;
			glPushMatrix();
			glTranslatef(position.x, position.y + 0.50, position.z);
			glRotatef(ax, 1.0, 0.0, 0.0);
			glRotatef(az, 0.0, 0.0, 1.0);
			glRotatef(float(r%360), 0.0, 1.0, 0.0);
			glScalef(r%2 == 0 ? -1.0 : 1.0, 1.0, (r*251)%2 == 0 ? -1.0 : 1.0);
			glScalef(1.0/cos(ax*m_pi/180.0), 1.0, 1.0/cos(az*m_pi/180.0));
			glCallList(right_l);
			glPopMatrix();
		}

		r = d;

		left_m.bind();
		for (int x = 0; x < d1; x++)
		{
			r *= 2411;
			glPushMatrix();
			glTranslatef(position.x, position.y + 0.50, position.z);
			glRotatef(ax, 1.0, 0.0, 0.0);
			glRotatef(az, 0.0, 0.0, 1.0);
			glRotatef(float(r%360), 0.0, 1.0, 0.0);
			glScalef(r%2 == 0 ? -1.0 : 1.0, 1.0, (r*251)%2 == 0 ? -1.0 : 1.0);
			glScalef(1.0/cos(ax*m_pi/180.0), 1.0, 1.0/cos(az*m_pi/180.0));
			glCallList(left_l);
			glPopMatrix();
		}
	}
	else if (dist > m_sqr(20) && dist < m_sqr(50))
	{
		r = d;
		float ndensity = float(density)*(1.0 - (dist - m_sqr(10))/(m_sqr(40) - m_sqr(10)));
		float ntdensity = float(density)*(1.0 - (dist - m_sqr(10))/(m_sqr(70) - m_sqr(10)));

		top_m.bind();
		for (int x = 0; x < int(ntdensity); x++)
		{
			r *= 2411;
			glPushMatrix();
			glTranslatef(position.x, position.y + 0.50, position.z);
			//glTranslatef(double(r%200)/50.0, 0.0, double((r*253)%200)/50.0);
			glRotatef(ax, 1.0, 0.0, 0.0);
			glRotatef(az, 0.0, 0.0, 1.0);
			glRotatef(double(r%4)*90.0, 0.0, 1.0, 0.0);
			glScalef(r%2 == 0 ? -1.0 : 1.0, 1.0, (r*251)%2 == 0 ? -1.0 : 1.0);
			glScalef(1.0/cos(ax*m_pi/180.0), 1.0, 1.0/cos(az*m_pi/180.0));
			glCallList(top_l);
			glPopMatrix();
		}

		r = d;

		right_m.bind();
		for (int x = 0; x < int(ndensity); x++)
		{
			r *= 2411;
			glPushMatrix();
			glTranslatef(position.x, position.y + 0.50, position.z);
			glRotatef(ax, 1.0, 0.0, 0.0);
			glRotatef(az, 0.0, 0.0, 1.0);
			glRotatef(float(r%360), 0.0, 1.0, 0.0);
			glScalef(r%2 == 0 ? -1.0 : 1.0, 1.0, (r*251)%2 == 0 ? -1.0 : 1.0);
			glScalef(1.0/cos(ax*m_pi/180.0), 1.0, 1.0/cos(az*m_pi/180.0));
			glCallList(right_l);
			glPopMatrix();
		}

		r = d;

		left_m.bind();
		for (int x = 0; x < int(ndensity); x++)
		{
			r *= 2411;
			glPushMatrix();
			glTranslatef(position.x, position.y + 0.50, position.z);
			glRotatef(ax, 1.0, 0.0, 0.0);
			glRotatef(az, 0.0, 0.0, 1.0);
			glRotatef(float(r%360), 0.0, 1.0, 0.0);
			glScalef(r%2 == 0 ? -1.0 : 1.0, 1.0, (r*251)%2 == 0 ? -1.0 : 1.0);
			glScalef(1.0/cos(ax*m_pi/180.0), 1.0, 1.0/cos(az*m_pi/180.0));
			glCallList(left_l);
			glPopMatrix();
		}
	}
	else if (dist < m_sqr(80.0))
	{
		r = d;

		top_m.bind();
		float ndensity = float(density)*(1.0 - (dist - m_sqr(10))/(m_sqr(100) - m_sqr(10)));

		for (int x = 0; x < int(ndensity); x++)
		{
			r *= 2411;
			glPushMatrix();
			glTranslatef(position.x, position.y + 0.50, position.z);
			glRotatef(ax, 1.0, 0.0, 0.0);
			glRotatef(az, 0.0, 0.0, 1.0);
			glRotatef(double(r%4)*90.0, 0.0, 1.0, 0.0);
			glScalef(r%2 == 0 ? -1.0 : 1.0, 1.0, (r*251)%2 == 0 ? -1.0 : 1.0);
			glScalef(1.0/cos(ax*m_pi/180.0), 1.0, 1.0/cos(az*m_pi/180.0));
			glCallList(top_l);
			glPopMatrix();
		}
	}
}

void volfoliagehdl::release()
{
	glDeleteLists(near_l, 1);
	near_m.release();
	glDeleteLists(right_l, 1);
	right_m.release();
	glDeleteLists(left_l, 1);
	left_m.release();
	glDeleteLists(top_l, 1);
	top_m.release();
}

void maphdl::sphere_collide(vec *position, vec *velocity, float size)
{
	double dx = (position->x)/10.0 + double(width/2);
	double dy = (position->z)/10.0 + double(height/2);

	int x = int(dx);
	int y = int(dy);

	vec n;

	if (x+1 < width && y+1 < height && x >= 0 && y >= 0)
	{
		double h00 = verts[(x*width + y)*3 + 1];
		double h01 = verts[((x+1)*width + y)*3 + 1];
		double h10 = verts[(x*width + (y+1))*3 + 1];
		double h11 = verts[((x+1)*width + (y+1))*3 + 1];

		n = (vec(&norms[(x*width + y)*3]) +
		     vec(&norms[((x+1)*width + y)*3]) +
		     vec(&norms[(x*width + (y+1))*3]) +
		     vec(&norms[((x+1)*width + (y+1))*3]))/4.0;

		double tx = dx - double(x);
		double ty = dy - double(y);

		double txty = tx * ty;

		double height = h00 * (1.0 - ty - tx + txty)
				+ h01 * (tx - txty)
				+ h11 * txty
				+ h10 * (ty - txty) + size;

		if (position->y <= height + 0.0001)
		{
			*position += n*(height + 0.0002 - position->y)/dot(vec(0.0, 1.0, 0.0), n);
			*velocity += -2.0*dot(*velocity, n)*n;
			*velocity *= 0.90;
		}
	}
}

void maphdl::load(int w, int h)
{
	double scale = 10.0;

	width = w;
	height = h;

	indices = new unsigned int[(w-1)*(h)*(2)];
	verts = new double[w*h*3];
	texts = new double[w*h*2];
	norms = new double[w*h*3];

	int index = 0;
	for (int x = 1; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			indices[index++] = x*w + y;
			indices[index++] = (x-1)*w + y;
		}
	}

	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
		{
			verts[(x*w + y)*3 + 0] = double(x - w/2);
			verts[(x*w + y)*3 + 1] = double(rand()%20 - 40)/4.0;
			verts[(x*w + y)*3 + 2] = double(y - h/2);

			texts[(x*w + y)*2 + 0] = scale*10.0*double(x)/double(w);
			texts[(x*w + y)*2 + 1] = scale*10.0*double(y)/double(h);
		}

	double r;
	for (int x = 0; x < w; x+=w/8)
		for (int y = 0; y < h; y+=h/8)
		{
			r = (rand()%10 - 20)/2;
			for (int z = 0; z < w/8; z++)
				for (int u = 0; u < h/8; u++)
					verts[((x+z)*w + y + u)*3 + 1] += r;
		}

	for (int x = 1; x < w-1; x++)
		for (int y = 1; y < h-1; y++)
			verts[(x*w + y)*3 + 1] = (verts[(x*w + y)*3 + 1] + verts[((x+1)*w + y)*3 + 1] + verts[((x-1)*w + y)*3 + 1] + verts[(x*w + (y+1))*3 + 1] + verts[(x*w + (y-1))*3 + 1])/5;

	for (int x = 1; x < w-1; x++)
		for (int y = 1; y < h-1; y++)
			verts[(x*w + y)*3 + 1] = (verts[(x*w + y)*3 + 1] + verts[((x+1)*w + y)*3 + 1] + verts[((x-1)*w + y)*3 + 1] + verts[(x*w + (y+1))*3 + 1] + verts[(x*w + (y-1))*3 + 1])/5;

	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
			for (int z = 0; z < 3; z++)
				verts[(x*w + y)*3 + z] *= scale;

	vec n;
	for (int x = 1; x < w-1; x++)
		for (int y = 1; y < h-1; y++)
		{
			n = vec();
			n += normvec(vec(&verts[((x-1)*w + y)*3]), vec(&verts[(x*w + y)*3]), vec(&verts[(x*w + (y-1))*3]));
			n += normvec(vec(&verts[(x*w + (y-1))*3]), vec(&verts[(x*w + y)*3]), vec(&verts[((x+1)*w + (y-1))*3]));
			n += normvec(vec(&verts[((x+1)*w + (y-1))*3]), vec(&verts[(x*w + y)*3]), vec(&verts[((x+1)*w + y)*3]));
			n += normvec(vec(&verts[((x+1)*w + y)*3]), vec(&verts[(x*w + y)*3]), vec(&verts[(x*w + (y+1))*3]));
			n += normvec(vec(&verts[((x-1)*w + y)*3]), vec(&verts[(x*w + y)*3]), vec(&verts[(x*w + (y-1))*3]));
			n += normvec(vec(&verts[(x*w + (y+1))*3]), vec(&verts[(x*w + y)*3]), vec(&verts[((x-1)*w + y)*3]));
			n /= 6.0;
			norms[(x*w + y)*3 + 0] = n.x;
			norms[(x*w + y)*3 + 1] = n.y;
			norms[(x*w + y)*3 + 2] = n.z;
		}

	//cover.grass(2.0, 1.0, 0.25, 2);
}

void maphdl::load(char *filename)
{
	unsigned char *data = rgb_tga(filename, &width, &height);
	int w = width;
	int h = height;
	int bpp = 3;

	double scale = 10.0;

	indices = new unsigned int[(w-1)*(h)*(2)];
	verts = new double[w*h*3];
	texts = new double[w*h*2];
	norms = new double[w*h*3];

	int index = 0;
	for (int x = 1; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			indices[index++] = x*w + y;
			indices[index++] = (x-1)*w + y;
		}
	}

	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
		{
			verts[(x*w + y)*3 + 0] = double(x - w/2);
			verts[(x*w + y)*3 + 1] = 20.0*double(data[(x*w + y)*bpp + 1])/256.0;
			verts[(x*w + y)*3 + 2] = double(y - h/2);

			texts[(x*w + y)*2 + 0] = scale*10.0*double(x)/double(w);
			texts[(x*w + y)*2 + 1] = scale*10.0*double(y)/double(h);
		}

	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
			for (int z = 0; z < 3; z++)
				verts[(x*w + y)*3 + z] *= scale;

	vec n;
	for (int x = 1; x < w-1; x++)
		for (int y = 1; y < h-1; y++)
		{
			n = vec();
			n += normvec(vec(&verts[((x-1)*w + y)*3]), vec(&verts[(x*w + y)*3]), vec(&verts[(x*w + (y-1))*3]));
			n += normvec(vec(&verts[(x*w + (y-1))*3]), vec(&verts[(x*w + y)*3]), vec(&verts[((x+1)*w + (y-1))*3]));
			n += normvec(vec(&verts[((x+1)*w + (y-1))*3]), vec(&verts[(x*w + y)*3]), vec(&verts[((x+1)*w + y)*3]));
			n += normvec(vec(&verts[((x+1)*w + y)*3]), vec(&verts[(x*w + y)*3]), vec(&verts[(x*w + (y+1))*3]));
			n += normvec(vec(&verts[((x-1)*w + y)*3]), vec(&verts[(x*w + y)*3]), vec(&verts[(x*w + (y-1))*3]));
			n += normvec(vec(&verts[(x*w + (y+1))*3]), vec(&verts[(x*w + y)*3]), vec(&verts[((x-1)*w + y)*3]));
			n /= 6.0;
			norms[(x*w + y)*3 + 0] = n.x;
			norms[(x*w + y)*3 + 1] = n.y;
			norms[(x*w + y)*3 + 2] = n.z;
		}

	delete data;


	int size;
	data = rgba_tga((char*)"dirt.tga", (char*)"opaque.tga", &size, &size);


	base.init((char*)"map.vx", (char*)"map.ft");
	base.addtex(data, size, size, 1, GL_RGBA, true);

	delete [] data;

	glAlphaFunc(GL_GEQUAL, -1.0);
	cover.grass(2.0, 0.5, 0.6, 4);
	glAlphaFunc(GL_GEQUAL, 0.8);

	data = rgba_tga((char*)"grass.tga", (char*)"opaque.tga", &size, &size);

	base.addtex(data, size, size, 1, GL_RGBA, true);

	delete [] data;

	data = rgb_tga((char*)"grassdensity.tga", &size, &size);

	unsigned char *ret = new unsigned char[size*size*4];

	for (int x = 0; x < size; x++)
		for (int y = 0; y < size; y++)
		{
			ret[(x*size + y)*4 + 0] = data[(y*size + x)*3 + 0];
			ret[(x*size + y)*4 + 1] = data[(y*size + x)*3 + 1];
			ret[(x*size + y)*4 + 2] = data[(y*size + x)*3 + 2];
			ret[(x*size + y)*4 + 3] = 255;
		}

	base.addtex(ret, size, size, 1, GL_RGBA, false);

	density = new int[size*size];

	for (int x = 0; x < size; x++)
		for (int y = 0; y < size; y++)
			density[x*size + y] = (int(data[(x*size + y)*3 + 1])*6)/255;

	delete [] data;
	delete [] ret;

	data = rgba_tga((char*)"rock.tga", (char*)"opaque.tga", &size, &size);
	base.addtex(data, size, size, 1, GL_RGBA, true);

	delete [] data;

	data = rgb_tga((char*)"rockdensity.tga", &size, &size);

	ret = new unsigned char[size*size*4];

	for (int x = 0; x < size; x++)
		for (int y = 0; y < size; y++)
		{
			ret[(x*size + y)*4 + 0] = data[(y*size + x)*3 + 0];
			ret[(x*size + y)*4 + 1] = data[(y*size + x)*3 + 1];
			ret[(x*size + y)*4 + 2] = data[(y*size + x)*3 + 2];
			ret[(x*size + y)*4 + 3] = 255;
		}

	base.addtex(ret, size, size, 1, GL_RGBA, false);

	delete [] data;
	delete [] ret;

	water.init();

	for (float x = -51.2; x < 51.2; x+=3.0)
		for (float y = -51.2; y < 51.2; y+=3.0)
		{
			water.new_drip(vec(x*10.0, 200.0, y*10.0), vec(0.0, 0.0, 0.0), 5.0);
		}
}

void maphdl::render(vec camera)
{
	srand(10);
	vec vert;
	vec norm;
	glDisable(GL_CULL_FACE);
	for (int x = 0; x < width-1; x++)
		for (int y = 0; y < height-1; y++)
		{
			vert = (vec(&verts[(x*width + y)*3]) + vec(&verts[((x+1)*width + y)*3]) + vec(&verts[((x+1)*width + (y+1))*3]) + vec(&verts[(x*width + (y+1))*3]))/4.0;
			norm = (vec(&norms[(x*width + y)*3]) + vec(&norms[((x+1)*width + y)*3]) + vec(&norms[((x+1)*width + (y+1))*3]) + vec(&norms[(x*width + (y+1))*3]))/4.0;
			cover.render(density[x*width + y], vert, norm, camera);
		}
	glEnable(GL_CULL_FACE);

	/*drip *curr = water.first;
	drip *curr2, *prev;
	vec n;
	double dist;
	vec force;
	while (curr != NULL)
	{
		curr->velocity.y -= 9.8/60;

		curr2 = curr->next;
		while (curr2 != NULL)
		{
			n = curr2->position - curr->position;
			dist = mag(n);
			if (dist < curr->size + curr2->size)
			{
				n = n/dist;
				force = (-dot(curr->velocity, n) + dot(curr2->velocity, n))*n;
				curr->velocity += force;
				curr2->velocity -= force;
				force = (curr2->position + curr->position)/2.0;
				curr->position = force - n*curr->size;
				curr2->position = force + n*curr2->size;
				curr->velocity *= 0.99;
				curr2->velocity *= 0.99;
			}
			curr2 = curr2->next;
		}
		sphere_collide(&curr->position, &curr->velocity, curr->size);
		prev = curr;
		curr = curr->next;

		if (prev->position.x < -10.0*float(width/2) || prev->position.z < -10.0*float(height/2) || prev->position.x > 10.0*float(width/2) || prev->position.z > 10.0*float(height/2))
			water.del_drip(prev);
	}*/

	base.bind();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_DOUBLE, 0, verts);
	glTexCoordPointer(2, GL_DOUBLE, 0, texts);
	glNormalPointer(GL_DOUBLE, 0, norms);
	for (int x = 0; x < height-1; x++)
		glDrawElements(GL_QUAD_STRIP, 2*width, GL_UNSIGNED_INT, &indices[x*2*width]);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	//water.render();
}

void maphdl::release()
{
	delete verts;
	delete texts;
	delete norms;
	delete indices;
	delete density;

	cover.release();
	water.release();
}
