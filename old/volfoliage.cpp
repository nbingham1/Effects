void volfoliagehdl::grass(double length, double rigidity)
{
	int numpoints = int(length + 1.0)*3 + 1;
	double numrows = (trunc(length) + 2.0);
	double *verts = new double[numpoints*3];
	double *norms = new double[numpoints*3];

	for (int x = 0; x < int(length + 1.0); x++)
	{
		for (int y = 0; y < 3; y++)
		{
			verts[(x*3 + y)*3 + 0] = (-1.0*(double(x) - numrows/3.0)*(double(x) - numrows/3.0)/(numrows*numrows/4.0)*double(y - 1) + 2.0*double(y - 1))/(40.0/length);
			verts[(x*3 + y)*3 + 1] = sin(3.14159/2.0 - rigidity*double(x)/trunc(length + 1.0))*double(x)*trunc(length)/(length+1.0);
			verts[(x*3 + y)*3 + 2] = cos(3.14159/2.0 - rigidity*double(x)/trunc(length + 1.0))*double(x)*trunc(length)/(length+1.0) - abs(double(y-1))/2.0;
		}
	}

	verts[(numpoints-1)*3 + 0] = 0.0;
	verts[(numpoints-1)*3 + 1] = length*sin(3.14159/2.0 - rigidity);
	verts[(numpoints-1)*3 + 2] = length*cos(3.14159/2.0 - rigidity);

	unsigned int *indices = new unsigned int[6*int(length + 1.0) + 4];

	int index = 0;
	for (int x = 1; x < int(length + 1.0); x++)
	{
		for (int y = 0; y < 3; y++)
		{
			indices[index++] = x*3 + y;
			indices[index++] = (x-1)*3 + y;
		}
	}

	indices[index + 0] = numpoints - 1;
	indices[index + 1] = numpoints - 4;
	indices[index + 2] = numpoints - 3;
	indices[index + 3] = numpoints - 2;

	vec n;
	double count;
	for (int x = 0; x < int(length + 1.0); x++)
		for (int y = 0; y < 3; y++)
		{
			count = 0.0;
			n = vec();
			if (x-1 >= 0 && y-1 >= 0)
			{
				n += normvec(vec(&verts[((x-1)*3 + y)*3]), vec(&verts[(x*3 + y)*3]), vec(&verts[(x*3 + (y-1))*3]));
				n += normvec(vec(&verts[((x-1)*3 + y)*3]), vec(&verts[(x*3 + y)*3]), vec(&verts[(x*3 + (y-1))*3]));
				count+=2.0;
			}
			if (y-1 >= 0 && x+1 <= int(length + 1.0))
			{
				if (x+1 == int(length + 1.0))
				{
					n += normvec(vec(&verts[(x*3 + (y-1))*3]), vec(&verts[(x*3 + y)*3]), vec(&verts[(numpoints-1)*3]));
					count+=1.0;
				}
				else
				{
					n += normvec(vec(&verts[(x*3 + (y-1))*3]), vec(&verts[(x*3 + y)*3]), vec(&verts[((x+1)*3 + (y-1))*3]));
					n += normvec(vec(&verts[((x+1)*3 + (y-1))*3]), vec(&verts[(x*3 + y)*3]), vec(&verts[((x+1)*3 + y)*3]));
					count+=2.0;
				}
			}
			if (x+1 <= int(length + 1.0) && y+1 < 3)
			{
				if (x+1 == int(length + 1.0))
				{
					n += normvec(vec(&verts[(numpoints-1)*3]), vec(&verts[(x*3 + y)*3]), vec(&verts[(x*3 + (y+1))*3]));
					count+=1.0;
				}
				else
				{
					n += normvec(vec(&verts[((x+1)*3 + y)*3]), vec(&verts[(x*3 + y)*3]), vec(&verts[(x*3 + (y+1))*3]));
					count+=1.0;
				}
			}
			if (y+1 < 3 && x-1 >= 0)
			{
				n += normvec(vec(&verts[(x*3 + (y+1))*3]), vec(&verts[(x*3 + y)*3]), vec(&verts[((x-1)*3 + y)*3]));
				count+=1.0;
			}
			n /= count;
			norms[(x*3 + y)*3 + 0] = n.x;
			norms[(x*3 + y)*3 + 1] = n.y;
			norms[(x*3 + y)*3 + 2] = n.z;
		}

	n = vec();
	n += normvec(vec(&verts[(numpoints-1)*3]), vec(&verts[(numpoints-3)*3]), vec(&verts[(numpoints-4)*3]));
	n += normvec(vec(&verts[(numpoints-1)*3]), vec(&verts[(numpoints-2)*3]), vec(&verts[(numpoints-3)*3]));
	n /= 2.0;

	norms[(numpoints-1)*3 + 0] = n.x;
	norms[(numpoints-1)*3 + 1] = n.y;
	norms[(numpoints-1)*3 + 2] = n.z;

	near = glGenLists(1);
	glNewList(near, GL_COMPILE);

	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_DOUBLE, 0, verts);
	//glTexCoordPointer(2, GL_DOUBLE, 0, texts);
	glNormalPointer(GL_DOUBLE, 0, norms);
	double r;
	for (int x = 0; x < 10; x++)
	{
		glPushMatrix();
		glTranslatef(double(rand()%100)/10.0, 0.0, double(rand()%100)/10.0);
		glRotatef(double(rand()%360), 0.0, 1.0, 0.0);
		r = double(rand()%100)/200.0 + .5;
		glScalef(r, r, r);
		for (int x = 0; x < int(length); x++)
			glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, &indices[x*6]);
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, &indices[index]);

		glPopMatrix();
	}

	glDisableClientState(GL_NORMAL_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glEndList();

	delete [] verts;
	delete [] norms;
	delete [] indices;
}


/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/
/***********************************************************************************************************************************/


void volfoliagehdl::load(char *filename)
{
}

void volfoliagehdl::grass(double length, double width, double rigidity, int size_h)
{
	int numpoints = size_h*2;
	double numrows = double(size_h);
	double *verts = new double[numpoints*3];
	double *norms = new double[numpoints*3];
	double *texts = new double[numpoints*2];
	double rigid = rigidity*m_pi/2.0;

	double s;
	for (int x = 0; x < 2; x++)
	{
		s = x == 0 ? -1.0 : 1.0;
		for (int y = 0; y < size_h; y++)
		{
			verts[(x*size_h + y)*3 + 0] = width*(2.0*s)/10.0;
			verts[(x*size_h + y)*3 + 1] = sin(m_pi/2.0 - rigid*double(y)/double(size_h-1))*length*double(y)/double(size_h-1);
			verts[(x*size_h + y)*3 + 2] = cos(m_pi/2.0 - rigid*double(y)/double(size_h-1))*length*double(y)/double(size_h-1);

			texts[(x*size_h + y)*2 + 0] = double(y)/double(size_h-1);
			texts[(x*size_h + y)*2 + 1] = s;
		}
	}

	unsigned int *indices = new unsigned int[2*size_h];

	int index = 0;
	for (int x = 1; x < 2; x++)
	{
		for (int y = 0; y < size_h; y++)
		{
			indices[index++] = x*size_h + y;
			indices[index++] = (x-1)*size_h + y;
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
				n += normvec(vec(&verts[(x*size_h + (y-1))*3]), vec(&verts[(x*size_h + y)*3]), vec(&verts[((x-1)*size_h + y)*3]));
				n += normvec(vec(&verts[(x*size_h + (y-1))*3]), vec(&verts[(x*size_h + y)*3]), vec(&verts[((x-1)*size_h + y)*3]));
				count+=2.0;
			}
			if (x-1 >= 0 && y+1 <= size_h)
			{
				n += normvec(vec(&verts[((x-1)*size_h + y)*3]), vec(&verts[(x*size_h + y)*3]), vec(&verts[((x-1)*size_h + (y+1))*3]));
				n += normvec(vec(&verts[((x-1)*size_h + (y+1))*3]), vec(&verts[(x*size_h + y)*3]), vec(&verts[(x*size_h + (y+1))*3]));
				count+=2.0;
			}
			if (y+1 <= int(length + 1.0) && x+1 < 2)
			{
				n += normvec(vec(&verts[(x*size_h + (y+1))*3]), vec(&verts[(x*size_h + y)*3]), vec(&verts[((x+1)*size_h + y)*3]));
				count+=1.0;
			}
			if (x+1 < 2 && y-1 >= 0)
			{
				n += normvec(vec(&verts[((x+1)*size_h + y)*3]), vec(&verts[(x*size_h + y)*3]), vec(&verts[(x*size_h + (y-1))*3]));
				count+=1.0;
			}
			n /= count;
			norms[(x*size_h + y)*3 + 0] = -n.x;
			norms[(x*size_h + y)*3 + 1] = -n.y;
			norms[(x*size_h + y)*3 + 2] = -n.z;
		}

	int size;
	unsigned char *data = rgba_tga("/home/nedbingham/Desktop/grass2.tga", "/home/nedbingham/Desktop/grass3.tga", &size, &size);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &near_t);

	glBindTexture(GL_TEXTURE_2D, near_t);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, size, size, GL_RGBA, GL_UNSIGNED_BYTE, data);

	delete [] data;

	near = glGenLists(1);
	glNewList(near, GL_COMPILE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_DOUBLE, 0, verts);
	glTexCoordPointer(2, GL_DOUBLE, 0, texts);
	glNormalPointer(GL_DOUBLE, 0, norms);
	double r;
	vec col;
	for (int x = 0; x < 200; x++)
	{
		glPushMatrix();
		glTranslatef(double(rand()%121)/10.0 - 6, 0.0, double(rand()%151)/10.0 - 6);
		glRotatef(double(rand()%360), 0.0, 1.0, 0.0);
		r = double(rand()%100)/200.0 + .5;
		glScalef(r, r, r);
		col.x = 0.1*double(rand()%11)/10.0 + 0.35;
		col.y = 0.1*double(rand()%11)/10.0 + 0.55;
		col.z = 0.1*double(rand()%11)/10.0 + 0.02;
		GLfloat diff[] = {col.x, col.y, col.z, 1.0};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
		glDrawElements(GL_TRIANGLE_STRIP, size_h*2, GL_UNSIGNED_INT, indices);
		glColor3f(1.0, 1.0, 1.0);

		glPopMatrix();
	}

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glEndList();

	delete [] verts;
	delete [] texts;
	delete [] norms;
	delete [] indices;

	int dens = 4;
	verts = new double[4*2*dens*3];
	texts = new double[4*2*dens*2];
	norms = new double[4*2*dens*3];

	for (int x = 0; x < dens; x++)
	{
		verts[(x*4 + 0)*3 + 0] = 0.0;
		verts[(x*4 + 0)*3 + 1] = 0.0;
		verts[(x*4 + 0)*3 + 2] = double(x)*10.0/double(dens);

		verts[(x*4 + 1)*3 + 0] = 0.0;
		verts[(x*4 + 1)*3 + 1] = length;
		verts[(x*4 + 1)*3 + 2] = (double(x) + rigidity)*10.0/double(dens);

		verts[(x*4 + 2)*3 + 0] = 10.0;
		verts[(x*4 + 2)*3 + 1] = length;
		verts[(x*4 + 2)*3 + 2] = (double(x) + rigidity)*10.0/double(dens);

		verts[(x*4 + 3)*3 + 0] = 10.0;
		verts[(x*4 + 3)*3 + 1] = 0.0;
		verts[(x*4 + 3)*3 + 2] = double(x)*10.0/double(dens);

		n = normvec(vec(&verts[(x*4 + 1)*3]), vec(&verts[(x*4 + 0)*3]), vec(&verts[(x*4 + 3)*3]));

		for (int z = 0; z < 4; z++)
		{
			norms[(x*4 + z)*3 + 0] = n.x;
			norms[(x*4 + z)*3 + 1] = n.y;
			norms[(x*4 + z)*3 + 2] = n.z;
		}

		verts[((x+dens)*4 + 0)*3 + 0] = double(x)*10.0/double(dens);
		verts[((x+dens)*4 + 0)*3 + 1] = 0.0;
		verts[((x+dens)*4 + 0)*3 + 2] = 0.0;

		verts[((x+dens)*4 + 1)*3 + 0] = (double(x) + rigidity)*10.0/double(dens);
		verts[((x+dens)*4 + 1)*3 + 1] = length;
		verts[((x+dens)*4 + 1)*3 + 2] = 0.0;

		verts[((x+dens)*4 + 2)*3 + 0] = (double(x) + rigidity)*10.0/double(dens);
		verts[((x+dens)*4 + 2)*3 + 1] = length;
		verts[((x+dens)*4 + 2)*3 + 2] = 10.0;

		verts[((x+dens)*4 + 3)*3 + 0] = double(x)*10.0/double(dens);
		verts[((x+dens)*4 + 3)*3 + 1] = 0.0;
		verts[((x+dens)*4 + 3)*3 + 2] = 10.0;

		n = normvec(vec(&verts[((x+dens)*4 + 1)*3]), vec(&verts[((x+dens)*4 + 0)*3]), vec(&verts[((x+dens)*4 + 3)*3]));

		for (int z = 0; z < 4; z++)
		{
			norms[((x+dens)*4 + z)*3 + 0] = n.x;
			norms[((x+dens)*4 + z)*3 + 1] = n.y;
			norms[((x+dens)*4 + z)*3 + 2] = n.z;
		}

		texts[(x*4 + 0)*2 + 0] = 0.0;
		texts[(x*4 + 0)*2 + 1] = 0.0;

		texts[(x*4 + 1)*2 + 0] = 0.0;
		texts[(x*4 + 1)*2 + 1] = 1.0;

		texts[(x*4 + 2)*2 + 0] = 2.0;
		texts[(x*4 + 2)*2 + 1] = 1.0;

		texts[(x*4 + 3)*2 + 0] = 2.0;
		texts[(x*4 + 3)*2 + 1] = 0.0;

		texts[((x+dens)*4 + 0)*2 + 0] = 0.0;
		texts[((x+dens)*4 + 0)*2 + 1] = 0.0;

		texts[((x+dens)*4 + 1)*2 + 0] = 0.0;
		texts[((x+dens)*4 + 1)*2 + 1] = 1.0;

		texts[((x+dens)*4 + 2)*2 + 0] = 2.0;
		texts[((x+dens)*4 + 2)*2 + 1] = 1.0;

		texts[((x+dens)*4 + 3)*2 + 0] = 2.0;
		texts[((x+dens)*4 + 3)*2 + 1] = 0.0;
	}

	size = 128;
	unsigned char *tex_data = new unsigned char[size*size*4];
	for (int x = 0; x < size*size; x++)
	{
		tex_data[x*4 + 0] = 128;
		tex_data[x*4 + 1] = 128;
		tex_data[x*4 + 2] = 64;
		tex_data[x*4 + 3] = 0;
	}

	int tex_pos;
	int w;
	for (int x = 0; x < dens; x++)
	{
		tex_pos = (rand()%size);
		col.x = 255.0*0.6*double(rand()%11)/10.0;
		col.y = 255.0*0.8*double(rand()%11)/10.0;
		col.z = 255.0*0.3*double(rand()%11)/10.0;
		for (int y = 0; y < size; y++)
		{
			for (int z = tex_pos + (y-size/3)*(y-size/3)*int(width*7.0)/(size*size/4) - int(width*7.0); z < tex_pos - (y-size/3)*(y-size/3)*int(width*7.0)/(size*size/4) + int(width*7.0); z++)
			{
				w = z - int(double(size*2/3)*sin(m_pi/2.0 - rigid*double(y)/double(size)));
				if (w < size && w >= 0)
				{
					tex_data[(y*size + w)*4 + 0] = int(col.x);
					tex_data[(y*size + w)*4 + 1] = int(col.y);
					tex_data[(y*size + w)*4 + 2] = int(col.z);
					tex_data[(y*size + w)*4 + 3] = 255;
				}
			}
		}
	}

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &med_t);

	glBindTexture(GL_TEXTURE_2D, med_t);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, size, size, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);

	delete tex_data;

	med = glGenLists(1);
	glNewList(med, GL_COMPILE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_DOUBLE, 0, norms);
	glVertexPointer(3, GL_DOUBLE, 0, verts);
	glTexCoordPointer(2, GL_DOUBLE, 0, texts);
	glDrawArrays(GL_QUADS, 0, 4*2*dens);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glEndList();

	delete [] verts;
	delete [] texts;
	
	
	
	
	
	size = 256;
	tex_data = new unsigned char[size*size*4];
	for (int x = 0; x < size*size; x++)
	{
		tex_data[x*4 + 0] = 128;
		tex_data[x*4 + 1] = 128;
		tex_data[x*4 + 2] = 64;
		tex_data[x*4 + 3] = 255;
	}

	double dx, dy;
	double sx, sy;
	int a, b;
	for (int x = 0; x < 3000*dens; x++)
	{
		tex_pos = (rand()%16)*size/16;
		col.x = 255.0*0.5*double(rand()%11)/10.0;
		col.y = 255.0*(double(rand()%11)/20.0 + .5);
		col.z = 255.0*0.3*double(rand()%11)/10.0;
		dy = double(rand()%360)*m_pi/180.0;
		dx = cos(dy);
		dy = sin(dy);
		sx = double(rand()%(size+200) - 100);
		sy = double(rand()%(size+200) - 100);

		for (int y = 0; y < int(length*7.0); y++)
		{
			for (int z = tex_pos + (y-size/3)*(y-size/3)*int(width*7.0)/(size*size/4) - int(width*7.0); z < tex_pos - (y-size/3)*(y-size/3)*int(width*7.0)/(size*size/4) + int(width*7.0); z++)
			{
				w = z - int(double(size*2/3)*sin(m_pi/2.0 - rigid*double(y)/double(size)));
				a = int(double(y)*dx + double(w)*dy + sx);
				b = int(double(y)*dy + double(w)*dx + sy);
				if (a < size && a >= 0 && b < size && b >= 0)
				{
					tex_data[(a*size + b)*4 + 0] = int((col.x*1.0 + 200.0*1.0)/2.0);
					tex_data[(a*size + b)*4 + 1] = int((col.y*1.0 + 150.0*1.0)/2.0);
					tex_data[(a*size + b)*4 + 2] = int((col.z*1.0 + 64.0*1.0)/2.0);
					tex_data[(a*size + b)*4 + 3] = 255;
				}
			}
		}
	}

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &far_t);

	glBindTexture(GL_TEXTURE_2D, far_t);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, size, size, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);

	delete tex_data;
}

void volfoliagehdl::render(int density, vec position, vec normal, vec camera)
{
	double dist = dot(position - camera, position - camera);
	int r = rand();

	double ax, az;

	if (dist < 50000.0)
	{
		ax = 90.0 - acos(normal.z)*180.0/m_pi;
		az = acos(normal.x)*180.0/m_pi - 90.0;

		if (dist < 1000.0)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, near_t);
			for (int x = 0; x < density; x++)
			{
				r *= 2411;
				glPushMatrix();
				glTranslatef(position.x + 5.0, position.y, position.z + 5.0);
				glTranslatef(double(r%100)/25, 0.0, double((r*253)%100)/25);
				glRotatef(ax, 1.0, 0.0, 0.0);
				glRotatef(az, 0.0, 0.0, 1.0);
				glRotatef(double(r%360), 0.0, 1.0, 0.0);
				glScalef(r%2 == 0 ? -1.0 : 1.0, 1.0, (r*251)%2 == 0 ? -1.0 : 1.0);
				glCallList(near);
				glPopMatrix();
			}
		}
		/*if (dist > 900.0)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, med_t);
			glColor3f(1.0, 1.0, 1.0);
			for (int x = 0; x < density/5.0; x++)
			{
				r *= 2411;
				glPushMatrix();
				glTranslatef(position.x, position.y, position.z);
				glTranslatef(double(r%100)/25, 0.0, double((r*253)%100)/25);
				glRotatef(ax, 1.0, 0.0, 0.0);
				glRotatef(az, 0.0, 0.0, 1.0);
				//glRotatef(double(r%360), 0.0, 1.0, 0.0);
				glScalef(r%2 == 0 ? -1.0 : 1.0, 1.0, (r*251)%2 == 0 ? -1.0 : 1.0);
				glScalef(1.0, m_clamp(1.0 - dist/50000.0, 0.0, 1.0), 1.0);
				glCallList(med);
				glPopMatrix();
			}
		}*/
	}
}

void volfoliagehdl::release()
{
	glDeleteLists(near, 1);
	glDeleteLists(med, 1);
	//glDeleteLists(far, 1);
}