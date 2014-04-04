#pragma once
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>

#include <GL/freeglut.h>

#include <glm.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat4;
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <GraphicsGL.h>
//#include "Cam.h"
//#include "Spaceship.h"
//#include "Geometry.h"
#include "Globals.h"

#include <iostream>
#include <fstream>
#include <string>

#define PI 3.141592f


using namespace std;

//handles to the two windows
int window_handles[2]; //window 0 is FPS window


//Spaceship: only spheres right now
Spaceship ship;

Globals globals;

int window1;
int window2;



//This actually opens the file and loads the image.
GLuint LoadBMP(const char* imagepath) {
	unsigned char* header = new unsigned char [54]; 
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;

	unsigned char* data;

	std::ifstream imageFile(imagepath, std::ifstream::binary);

	if(imageFile == NULL) {
		printf("Image could not be opened\n");
		return 0;
	}

	imageFile.read((char*)header, 54);
		

	if( header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return 0;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	if(imageSize == 0) {
		imageSize = width*height*3; //24-bit BMP, so one byte for RGB
	}
	if(dataPos == 0) {
		dataPos = 54;
	}

	data = new unsigned char[imageSize];

	imageFile.read((char*)data, imageSize);

	imageFile.close();

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	free(data);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );


	return texture;
}


/*Loads a .bmp image file and configures it as the only texture that is used for the floor*/
void LoadTexture() {
	glGenTextures(1, &globals.texture);

	globals.texture = LoadBMP("D:/School/CS 559/Project 1/CS 559_Project 1/floor.bmp");
	//set this as active texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, globals.texture); //bind my only texture handle to texture0

	glUseProgram(globals.programT);
	glUniform1i( glGetUniformLocation(globals.programT, "floorSampler"), 0); //sets sampler in shader to use texture0, which I bound my only image to
	glUseProgram(0);
}



/*This will setup Window 1 (perspective window). It loads and creates my shader programs, gets uniform handles,
 initializes all my vertex buffer objects and all my vertex array objects that will be used by the shaders. */
void init() {
	//set window dimensions
	//globals.windowWidth = 512;
	//globals.windowHeight = 512;

	//initialize spaceship
	ship.init();

	//Initialize Programs
	GLuint vertShader, fragShader;

	vertShader = LoadShader(GL_VERTEX_SHADER, "D:/School/CS 559/Project 1/CS 559_Project 1/spaceship.vert");
	fragShader = LoadShader(GL_FRAGMENT_SHADER, "D:/School/CS 559/Project 1/CS 559_Project 1/spaceship.frag");

	globals.program1 = CreateProgram(vertShader, fragShader);

	//get uniforms
	globals.worldUniform1 = glGetUniformLocation(globals.program1, "worldMatrix");
	globals.viewUniform1 = glGetUniformLocation(globals.program1, "viewMatrix");
	globals.projUniform1 = glGetUniformLocation(globals.program1, "projMatrix");
	globals.fragColorUniform1 = glGetUniformLocation(globals.program1, "Color");

	//set uniforms
	glUseProgram(globals.program1);
	glUniformMatrix4fv(globals.projUniform1, 1, GL_FALSE, glm::value_ptr(globals.perspProj));
	glUseProgram(0);



	//Floor program with textures
	vertShader = LoadShader(GL_VERTEX_SHADER, "D:/School/CS 559/Project 1/CS 559_Project 1/floor.vert");
	fragShader = LoadShader(GL_FRAGMENT_SHADER, "D:/School/CS 559/Project 1/CS 559_Project 1/floor.frag");

	globals.programT = CreateProgram(vertShader, fragShader);

	//get uniforms
	globals.worldTUniform = glGetUniformLocation(globals.programT, "worldMatrix");
	globals.viewTUniform = glGetUniformLocation(globals.programT, "viewMatrix");
	globals.projTUniform = glGetUniformLocation(globals.programT, "projMatrix");
	globals.textureUniform = glGetUniformLocation(globals.programT, "floorSampler");

	//set uniforms
	glUseProgram(globals.programT);
	glUniformMatrix4fv(globals.projTUniform, 1, GL_FALSE, glm::value_ptr(globals.perspProj));
	glUseProgram(0);





	//Initialize Vertex Buffers

	//GeoSphere
	//position
	glGenBuffers(1, &globals.positionBufferSphere1);
	glBindBuffer(GL_ARRAY_BUFFER, globals.positionBufferSphere1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * ship.positionDataSphere.size(), (float*)ship.positionDataSphere.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//indices
	glGenBuffers(1, &globals.elementBufferSphere1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, globals.elementBufferSphere1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort) * ship.indicesSphere.size(), (GLshort*)ship.indicesSphere.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	//Cylinder
	//positoin
	glGenBuffers(1, &globals.positionBufferCylinder1);
	glBindBuffer(GL_ARRAY_BUFFER, globals.positionBufferCylinder1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * ship.positionDataCylinder.size(), (float*)ship.positionDataCylinder.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//indices
	glGenBuffers(1, &globals.elementBufferCylinder1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, globals.elementBufferCylinder1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort) * ship.indicesCylinder.size(), (GLshort*)ship.indicesCylinder.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	//VAO
	glGenVertexArrays(1, &globals.vaoSphere1);
	glBindVertexArray(globals.vaoSphere1);
	glBindBuffer(GL_ARRAY_BUFFER, globals.positionBufferSphere1);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, globals.elementBufferSphere1);
	glBindVertexArray(0);

	glGenVertexArrays(1, &globals.vaoCylinder1);
	glBindVertexArray(globals.vaoCylinder1);
	glBindBuffer(GL_ARRAY_BUFFER, globals.positionBufferCylinder1);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, globals.elementBufferCylinder1);
	glBindVertexArray(0);





	//Floor
	//position
	glGenBuffers(1, &globals.positionFloorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, globals.positionFloorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * 4, (float*)globals.floor.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//texture
	glGenBuffers(1, &globals.textureFloorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, globals.textureFloorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * 4, (float*)globals.floorTex.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//indices
	glGenBuffers(1, &globals.elementFloorBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, globals.elementFloorBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort) * 6, (GLshort*)globals.floorIndex, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	LoadTexture();

	//vao
	glGenVertexArrays(1, &globals.vaoT);
	glBindVertexArray(globals.vaoT);

	glBindBuffer(GL_ARRAY_BUFFER, globals.positionFloorBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, globals.textureFloorBuffer);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, globals.elementFloorBuffer);
	glBindVertexArray(0);





	
	//Shapes are CCW for front facing triangles, either change this, or change the triangles/indices!
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW); //functions used to build shapes use counter clockwise as front facing!

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
}



/*This will setup Window 1 (perspective window). It loads and creates my shader programs, gets uniform handles,
 initializes all my vertex buffer objects and all my vertex array objects that will be used by the shaders. */
void init2() {

	//Initialize Programs
	GLuint vertShader, fragShader;

	vertShader = LoadShader(GL_VERTEX_SHADER, "D:/School/CS 559/Project 1/CS 559_Project 1/spaceship.vert");
	fragShader = LoadShader(GL_FRAGMENT_SHADER, "D:/School/CS 559/Project 1/CS 559_Project 1/spaceship.frag");

	globals.program2 = CreateProgram(vertShader, fragShader);

	//get uniforms
	globals.worldUniform2 = glGetUniformLocation(globals.program2, "worldMatrix");
	globals.viewUniform2 = glGetUniformLocation(globals.program2, "viewMatrix");
	globals.projUniform2 = glGetUniformLocation(globals.program2, "projMatrix");
	globals.fragColorUniform2 = glGetUniformLocation(globals.program2, "Color");


	//frustum program
	vertShader = LoadShader(GL_VERTEX_SHADER, "D:/School/CS 559/Project 1/CS 559_Project 1/frustum.vert");
	fragShader = LoadShader(GL_FRAGMENT_SHADER, "D:/School/CS 559/Project 1/CS 559_Project 1/frustum.frag");

	globals.program3 = CreateProgram(vertShader, fragShader);

	//get uniforms
	globals.worldUniform = glGetUniformLocation(globals.program3, "worldMatrix");
	globals.viewUniform = glGetUniformLocation(globals.program3, "viewMatrix");
	globals.projUniform = glGetUniformLocation(globals.program3, "projMatrix");
	globals.inverseProjUniform = glGetUniformLocation(globals.program3, "inverseProj");
	globals.inverseViewUniform = glGetUniformLocation(globals.program3, "inverseView");
	


	//set uniforms
	glUseProgram(globals.program2);
	glUniformMatrix4fv(globals.projUniform2, 1, GL_FALSE, glm::value_ptr(globals.orthoProj));
	glUseProgram(0);

	



	//Initialize Vertex Buffers

	//GeoSphere
	//position
	glGenBuffers(1, &globals.positionBufferSphere2);
	glBindBuffer(GL_ARRAY_BUFFER, globals.positionBufferSphere2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * ship.positionDataSphere.size(), (float*)ship.positionDataSphere.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//indices
	glGenBuffers(1, &globals.elementBufferSphere2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, globals.elementBufferSphere2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort) * ship.indicesSphere.size(), (GLshort*)ship.indicesSphere.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	//Cylinder
	//positoin
	glGenBuffers(1, &globals.positionBufferCylinder2);
	glBindBuffer(GL_ARRAY_BUFFER, globals.positionBufferCylinder2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * ship.positionDataCylinder.size(), (float*)ship.positionDataCylinder.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//indices
	glGenBuffers(1, &globals.elementBufferCylinder2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, globals.elementBufferCylinder2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort) * ship.indicesCylinder.size(), (GLshort*)ship.indicesCylinder.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	//Sphere
	//position
	glGenBuffers(1, &globals.positionBufferSphere3);
	glBindBuffer(GL_ARRAY_BUFFER, globals.positionBufferSphere3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * globals.positionSphere3.size(), (float*)globals.positionSphere3.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//indices
	glGenBuffers(1, &globals.elementBufferSphere3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, globals.elementBufferSphere3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort) * globals.elementSphere3.size(), (GLshort*)globals.elementSphere3.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



	//VAO
	glGenVertexArrays(1, &globals.vaoSphere2);
	glBindVertexArray(globals.vaoSphere2);
	glBindBuffer(GL_ARRAY_BUFFER, globals.positionBufferSphere2);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, globals.elementBufferSphere2);
	glBindVertexArray(0);

	glGenVertexArrays(1, &globals.vaoCylinder2);
	glBindVertexArray(globals.vaoCylinder2);
	glBindBuffer(GL_ARRAY_BUFFER, globals.positionBufferCylinder2);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, globals.elementBufferCylinder2);
	glBindVertexArray(0);

	glGenVertexArrays(1, &globals.vaoSphere3);
	glBindVertexArray(globals.vaoSphere3);
	glBindBuffer(GL_ARRAY_BUFFER, globals.positionBufferSphere3);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, globals.elementBufferSphere3);
	glBindVertexArray(0);


	//Frustum
	//position
	glGenBuffers(1, &globals.frustumBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, globals.frustumBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * 8, (float*)globals.frustum.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//indices
	glGenBuffers(1, &globals.frustumIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, globals.frustumIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLshort) * (8*6), (GLshort*)globals.frustumIndex, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//vao
	glGenVertexArrays(1, &globals.vaoFrustum);
	glBindVertexArray(globals.vaoFrustum);
	glBindBuffer(GL_ARRAY_BUFFER, globals.frustumBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, globals.frustumIndexBuffer);
	glBindVertexArray(0);




	//Shapes are CCW for front facing triangles, either change this, or change the triangles/indices!
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW); //functions used to build shapes use counter clockwise as front facing!

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
}


//Simple function to draw text, I disable depth testing so the text always shows on top and re-enables it after done.
//Takes as parameters the string to draw and the place/offset to draw it to. 
void drawText(const unsigned char* string, vec3 translate) {


	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)(globals.windowWidth2/globals.windowHeight2), 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glTranslatef(translate.x, translate.y, translate.z);

	glScalef(0.035f, 0.035f, 0.035f);

	glColor4f(0.0f, 1.0f, 1.0f, 1.0f);


	glutStrokeString(GLUT_STROKE_ROMAN, string);



	glEnable(GL_DEPTH_TEST);
}


//This is my perspective window render fuction 
void renderFPS() {
	//enable options
	//msaa
	if(globals.msaaEnable) {
		glEnable(GL_MULTISAMPLE_ARB);
	} else {
		glDisable(GL_MULTISAMPLE_ARB);
	}
	//wireframe mode
	if(globals.wireframeEnable) {
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
	}

	 
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//get current view matrix from camera class
	mat4 view = CalcViewMatrix(globals.camera.camPosition, globals.camera.camTarget, globals.camera.upDirection);

	glUseProgram(globals.program1);
	
	//get view matrix from camera
	glUniformMatrix4fv(globals.viewUniform1, 1, GL_FALSE, glm::value_ptr(view));
	


	//for this project, each ship is going to be the same, could either add method or parameter to pass in a "world" matrix
	//or a scale/rotation matrix to alter each ship individually

	//draw ships passing in the postion it should be placed
	float coord[] = { -60.0f, -20.0f, 20.0f, 60.0f }; //used to place ships 
	vec3 position;
	for(int i = 0; i < 4; i++) {
		position = vec3( coord[i], 0.0f, -60.0f);
		ship.render(position, globals.vaoSphere1, globals.vaoCylinder1, globals.worldUniform1, globals.fragColorUniform1);
		
		position = vec3( coord[i], 0.0f, -20.0f);
		ship.render(position, globals.vaoSphere1, globals.vaoCylinder1, globals.worldUniform1, globals.fragColorUniform1);

		position = vec3( coord[i], 0.0f, 20.0f);
		ship.render(position, globals.vaoSphere1, globals.vaoCylinder1, globals.worldUniform1, globals.fragColorUniform1);

		position = vec3( coord[i], 0.0f, 60.0f);
		ship.render(position, globals.vaoSphere1, globals.vaoCylinder1, globals.worldUniform1, globals.fragColorUniform1);
	}
	

	glUseProgram(0);


	//I have this as an option since for this project we wanted to demonstrate the frustum and so I can't see all of the textured floor so I can
	//turn it on and off.
	if(globals.textureOn) {
		//draw textured floor
		glUseProgram(globals.programT);
		glBindVertexArray(globals.vaoT);

		glUniformMatrix4fv(globals.viewTUniform, 1, GL_FALSE, glm::value_ptr(view)); //view

		mat4 world(1.0f);
		world[3] = vec4(0.0f, -19.0f, 0.0f, 1.0f); //translate
		world[0].x = world[2].z = 100.0f; //scale
		glUniformMatrix4fv(globals.worldTUniform, 1, GL_FALSE, glm::value_ptr(world));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, globals.texture);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (GLubyte*)0);


		glBindVertexArray(0);
		glUseProgram(0);

	}

	//draw text
	glViewport(0, 0, 400, 400); //do this so that text gets rendered to same area in window and doesn't change due to resizing
	const unsigned char string1[] = "Options: w - wireframe mode, m - antialiasing, t - textures";
	vec3 temp(-80.0f, -30.0f, 0.0f);
	drawText(string1, temp);
	const unsigned char string2[] = "PERSPECTIVE VIEW";
	temp = vec3(-80.0f, -40.0f, 0.0f);
	drawText(string2, temp);
	glViewport(0, 0, globals.windowWidth1, globals.windowHeight1); //reset viewport back to entire screen


	glutSwapBuffers();
	glutPostRedisplay();
}


//This is my orthographic projection window with two viewports, one looking down the x and one down the z axis.
void renderAxis() {	
	
	vec3 position; 

	glClearColor(0.4f, 0.4f, 0.4f, 0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(globals.program2);


	//left side X-axis
	glViewport( (int)(.1*globals.windowHalfWidth2), (int)(.1*globals.windowHeight2), (int)(.8*globals.windowHalfWidth2), (int)(.8*globals.windowHeight2) ); //Question

	//set view matrix with camera looking down x-axis
	glUniformMatrix4fv(globals.viewUniform2, 1, GL_FALSE, glm::value_ptr(globals.viewX));

	//draw ships, save time and only draw one row, since in ortho proj we don't see anything behind since it's a parallel proj
	float coord[] = { -60.0f, -20.0f, 20.0f, 60.0f };
	for(int i = 0; i < 4; i++) {
		position = vec3( 20.0f, 0.0f, coord[i]);
		ship.render(position, globals.vaoSphere2, globals.vaoCylinder2, globals.worldUniform2, globals.fragColorUniform2);
	}



	//draw sphere
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);

	//update uniforms 
	mat4 temp(1.0f);
	temp[3] = vec4(-80.0f, 0.0f, 0.0f, 1.0f);
	glUniformMatrix4fv(globals.worldUniform2, 1, GL_FALSE, glm::value_ptr(temp));
	
	vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
	glUniform4fv(globals.fragColorUniform2, 1, glm::value_ptr(color));

	glBindVertexArray(globals.vaoSphere3);

	glDrawElements(GL_TRIANGLES, globals.elementSphere3.size(), GL_UNSIGNED_SHORT, 0);

	//reset this so other objects get filled in
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);

	glBindVertexArray(0);







	//right side Z-axis
	glViewport( (int)(.1*globals.windowHalfWidth2 + globals.windowHalfWidth2), (int)(.1*globals.windowHeight2), (int)(.8*globals.windowHalfWidth2), (int)(.8*globals.windowHeight2));

	glUniformMatrix4fv(globals.viewUniform2, 1, GL_FALSE, glm::value_ptr(globals.viewZ));

	for(int i = 0; i < 4; i++) {
		position = vec3( coord[i], 0.0f, 20.0f);
		ship.render(position, globals.vaoSphere1, globals.vaoCylinder1, globals.worldUniform1, globals.fragColorUniform1);
	}
	


	//draw sphere
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);

	//update uniforms
	temp[3] = vec4(0.0f, 0.0f, -80.0f, 1.0f);
	glUniformMatrix4fv(globals.worldUniform2, 1, GL_FALSE, glm::value_ptr(temp));
	
	color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glUniform4fv(globals.fragColorUniform2, 1, glm::value_ptr(color));

	glBindVertexArray(globals.vaoSphere3);

	glDrawElements(GL_TRIANGLES, globals.elementSphere3.size(), GL_UNSIGNED_SHORT, 0);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);

	glBindVertexArray(0);


	glUseProgram(0);



	//draw frustum 
	glUseProgram(globals.program3);
	glBindVertexArray(globals.vaoFrustum);

	//set uniforms that will be used by the frustum shader program
	mat4 world(1.0f);
	
	glUniformMatrix4fv(globals.worldUniform, 1, GL_FALSE, glm::value_ptr(world)); //world
	
	glUniformMatrix4fv(globals.viewUniform, 1, GL_FALSE, glm::value_ptr(globals.viewX)); //view

	glUniformMatrix4fv(globals.projUniform, 1, GL_FALSE, glm::value_ptr(globals.orthoProj)); //proj

	mat4 inverse = CalcViewMatrix(globals.camera.camPosition, globals.camera.camTarget, globals.camera.upDirection);
	inverse = glm::inverse(inverse);
	glUniformMatrix4fv(globals.inverseViewUniform, 1, GL_FALSE, glm::value_ptr(inverse)); // view inverse

	inverse = glm::inverse(globals.perspProj);
	glUniformMatrix4fv(globals.inverseProjUniform, 1, GL_FALSE, glm::value_ptr(inverse)); //proj inverse


	//use same two viewports for drawing the frustum. I do this separately because I have a particular shader program and didn't want to switch back and forth.
	//left side X-axis
	glViewport( (int)(.1*globals.windowHalfWidth2), (int)(.1*globals.windowHeight2), (int)(.8*globals.windowHalfWidth2), (int)(.8*globals.windowHeight2) );

	glPointSize(10.0f);
	glDrawElements(GL_LINES, 48, GL_UNSIGNED_SHORT, (GLvoid*)0);


	//right side Z-axis
	glUniformMatrix4fv(globals.viewUniform, 1, GL_FALSE, glm::value_ptr(globals.viewZ)); //view

	glViewport( (int)(.1*globals.windowHalfWidth2 + globals.windowHalfWidth2), (int)(.1*globals.windowHeight2), (int)(.8*globals.windowHalfWidth2), (int)(.8*globals.windowHeight2));

	glDrawElements(GL_LINES, 48, GL_UNSIGNED_SHORT, (GLvoid*)0);


	glBindVertexArray(0);
	glUseProgram(0);



	//draw text
	glViewport(0, 0, 400, 400);
	const unsigned char string1[] = "Options: w - wireframe mode, m - antialiasing, t - textures";
	vec3 temp2(-80.0f, -30.0f, 0.0f);
	drawText(string1, temp2);
	const unsigned char string2[] = "X - A X I S";
	temp2 = vec3(-80.0f, -40.0f, 0.0f);
	drawText(string2, temp2);
	
	glViewport(700, 0, 800, 400);
	const unsigned char string3[] = "Z - A X I S";
	temp2 = vec3(-80.0f, -40.0f, 0.0f);
	drawText(string3, temp2);



	glutSwapBuffers();
	glutPostRedisplay();
}




void reshape1(int w, int h) {
	if(h > 0) {
		//this allows us to see more or less of the world
		globals.perspProj[0].x = globals.frustumScale * (h/(float)w);
		globals.perspProj[1].y = globals.frustumScale;

		//make sure to update uniforms for shaders that use perspective matrix
		glUseProgram(globals.program1);
		glUniformMatrix4fv(globals.projUniform1, 1, GL_FALSE, glm::value_ptr(globals.perspProj));
		glUseProgram(0);

		glUseProgram(globals.programT);
		glUniformMatrix4fv(globals.projTUniform, 1, GL_FALSE, glm::value_ptr(globals.perspProj));
		glUseProgram(0);
		
		globals.windowWidth1 = w;
		globals.windowHeight1 = h;

		glViewport(0, 0, w, h);
		
	}
}
void reshape2(int w, int h) {
	if(h > 0) {
		//update these becaue we use the width and height in the second window to set up the two viewports correctly
		globals.windowWidth2 = w;
		globals.windowHeight2 = h;
		globals.windowHalfWidth2 = (int)(w/2);
	}
}




void keyboard(unsigned char key, int x, int y) {
	switch(key) {
	
		//option toggles
	case 'm':
		globals.msaaEnable = !globals.msaaEnable;
		break;
	case 'w':
		globals.wireframeEnable = !globals.wireframeEnable;
		break;
	case 't':
		globals.textureOn = !globals.textureOn;
		break;
	case 27:
		glutLeaveMainLoop();
		return;
	}
}

void keyboard_special(int key, int x, int y) {
	switch(key) {
	//these simply move camera around on a sphere, use spherical coords then convert to xyz.
	case GLUT_KEY_UP:
		globals.camera.MoveUp();
		break;
	case GLUT_KEY_DOWN:
		globals.camera.MoveDown();
		break;
	case GLUT_KEY_RIGHT:
		globals.camera.MoveRight();
		break;
	case GLUT_KEY_LEFT:
		globals.camera.MoveLeft();
		break;

	//these two zoom in and out by increasing or decreasing the field of view for the perspective projection
	case GLUT_KEY_PAGE_UP:
		
		if(globals.FOV > 30.0f) {
			globals.FOV -= 3.0f;
			globals.frustumScale = CalcFrustumScale(globals.FOV);
			globals.perspProj[0].x = globals.frustumScale;
			globals.perspProj[1].y = globals.frustumScale;
			glUseProgram(globals.program1);
			glUniformMatrix4fv(globals.projUniform1, 1, GL_FALSE, glm::value_ptr(globals.perspProj));
			glUseProgram(0);
		}else { globals.FOV = 30.0f; }


		break;
	case GLUT_KEY_PAGE_DOWN:
		
		globals.FOV += 3.0f;
		if(globals.FOV <= 80.0f) {
			globals.frustumScale = CalcFrustumScale(globals.FOV);
			globals.perspProj[0].x = globals.frustumScale;
			globals.perspProj[1].y = globals.frustumScale;
			glUseProgram(globals.program1);
			glUniformMatrix4fv(globals.projUniform1, 1, GL_FALSE, glm::value_ptr(globals.perspProj));
			glUseProgram(0);
		}else { globals.FOV = 80.0f; }



		break;

	}
}

void keyboard2(unsigned char key, int x, int y) {
	switch(key) {
	case 27:
		glutLeaveMainLoop();
		break;
	}
}


void close() {
	//release variables in global class
	globals.~Globals();

}


int main(int argc, char* argv[]) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(512, 512);

	
	//window 1
	glutInitWindowPosition(500, 500);
	window1 = glutCreateWindow("Window 1");
	glutReshapeFunc(reshape1);
	glutDisplayFunc(renderFPS);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard_special);
	glutCloseFunc(close);


	//glew initialize for window 1
	GLenum err = glewInit();
	if(GLEW_OK != err) {
		fprintf(stderr, "Error initializing GLEW: %s\n", glewGetErrorString(err));
	}

	
	//initialize window 1
	init();
	

	
	
	//window 2
	glutInitWindowSize(1200, 600); //Question
	glutInitWindowPosition(1200, 500);
	window2 = glutCreateWindow("Window 2");
	glutReshapeFunc(reshape2);
	glutDisplayFunc(renderAxis);
	glutKeyboardFunc(keyboard2);
	glutCloseFunc(close);

	//initialize glew for window 2
	err = glewInit();
	if(GLEW_OK != err) {
		fprintf(stderr, "Error initializing GLEW: %s\n", glewGetErrorString(err));
	}
	

	//initialize window 2
	init2();
	
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	glutMainLoop();


	return 0;
}