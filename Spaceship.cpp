#include <GL/glew.h>
#include "Spaceship.h"
#include "Geometry.h"
#include <glm.hpp>
using glm::vec4;
using glm::vec3;
using glm::vec2;
#include <gtc/type_ptr.hpp>
#include <vector>
#pragma once
#include <GraphicsGL.h>

#include <iostream>

#define PI 3.141592;
int frames = 0;

//constructor
Spaceship::Spaceship() {
	
}
Spaceship::~Spaceship() {
	
}

//
void Spaceship::init() {

	//create geosphere and store data for geometry in Spaceship class variables, then can alter them or build on ship
	Geometry::createGeosphere(2.0f, 4, positionDataSphere, indicesSphere);

	Geometry::createCylinder(2.0f, 2.0f, 10.0f, 100, 5, positionDataCylinder, indicesCylinder);
	
	
}

/* This takes a base postion to draw ship at and from that position I compute the postion offset for each part of the ship. 
 Also takes everything I need to do the draw call in the class, so I need the vao's and uniforms. */
void Spaceship::render(glm::vec3 pos, GLuint& vaoSphere, GLuint& vaoCylinder, GLuint& worldUniform, GLuint& fragColorUniform) {
	
	//(x,y,z) pos
	float x = pos.x;
	float y = pos.y;
	float z = pos.z;

	glm::mat4 shipPart(1.0f); //used to translate, scale and rotate the various parts of the spaceship
	glm::vec4 shipPartColor;  //used to color the various parts of the ship

	

	//Draw Spheres
	glBindVertexArray(vaoSphere);

	//draw main ship base
	shipPart[0].x = shipPart[2].z = 2.0f; //scale width and lenght just a little
	shipPart[1].y = 8.0f; //scale sphere by 8ft in the y-axis
	shipPart[3] = vec4(x, y, z, 1.0f); //make sure it is translated to the correct place in the world
	glUniformMatrix4fv(worldUniform, 1, GL_FALSE, glm::value_ptr(shipPart));

	shipPartColor = vec4(0.0f, 0.0f, 1.0f, 1.0f); //color is blue
	glUniform4fv(fragColorUniform, 1, glm::value_ptr(shipPartColor));
	



	glDrawElements(GL_TRIANGLES, indicesSphere.size(), GL_UNSIGNED_SHORT, 0);


	shipPart = glm::mat4(1.0f); //reset to Id matrix to get rid of previous scaling 
	//draw top parts to 4 rockets
	shipPartColor = vec4(0.7f, 0.0f, 0.2f, 1.0f);
	glUniform4fv(fragColorUniform, 1, glm::value_ptr(shipPartColor));


	shipPart[3] = vec4(x - 8.0f, y - 5.0f, z, 1.0f);
	glUniformMatrix4fv(worldUniform, 1, GL_FALSE, glm::value_ptr(shipPart));
	glDrawElements(GL_TRIANGLES, indicesSphere.size(), GL_UNSIGNED_SHORT, 0);

	
	shipPart[3] = vec4(x + 8.0f, y - 5.0f, z, 1.0f);
	glUniformMatrix4fv(worldUniform, 1, GL_FALSE, glm::value_ptr(shipPart));
	glDrawElements(GL_TRIANGLES, indicesSphere.size(), GL_UNSIGNED_SHORT, 0);

	
	shipPart[3] = vec4(x, y - 5.0f, z + 8.0f, 1.0f);
	glUniformMatrix4fv(worldUniform, 1, GL_FALSE, glm::value_ptr(shipPart));
	glDrawElements(GL_TRIANGLES, indicesSphere.size(), GL_UNSIGNED_SHORT, 0);

	shipPart[3] = vec4(x, y - 5.0f, z - 8.0f, 1.0f);
	glUniformMatrix4fv(worldUniform, 1, GL_FALSE, glm::value_ptr(shipPart));
	glDrawElements(GL_TRIANGLES, indicesSphere.size(), GL_UNSIGNED_SHORT, 0);



	//draw bottom parts to rockets, scaled a bit
	shipPartColor = vec4(0.0f, 0.7f, 0.3f, 1.0f);
	glUniform4fv(fragColorUniform, 1, glm::value_ptr(shipPartColor));

	shipPart[1].y = 2.0f; //double height of sphere

	shipPart[3] = vec4(x - 8.0f, y - 15.0f, z, 1.0f);
	glUniformMatrix4fv(worldUniform, 1, GL_FALSE, glm::value_ptr(shipPart));
	glDrawElements(GL_TRIANGLES, indicesSphere.size(), GL_UNSIGNED_SHORT, 0);

	shipPart[3] = vec4(x + 8.0f, y - 15.0f, z, 1.0f);
	glUniformMatrix4fv(worldUniform, 1, GL_FALSE, glm::value_ptr(shipPart));
	glDrawElements(GL_TRIANGLES, indicesSphere.size(), GL_UNSIGNED_SHORT, 0);

	shipPart[3] = vec4(x, y - 15.0f, z + 8.0f, 1.0f);
	glUniformMatrix4fv(worldUniform, 1, GL_FALSE, glm::value_ptr(shipPart));
	glDrawElements(GL_TRIANGLES, indicesSphere.size(), GL_UNSIGNED_SHORT, 0);

	shipPart[3] = vec4(x, y - 15.0f, z - 8.0f, 1.0f);
	glUniformMatrix4fv(worldUniform, 1, GL_FALSE, glm::value_ptr(shipPart));
	glDrawElements(GL_TRIANGLES, indicesSphere.size(), GL_UNSIGNED_SHORT, 0);


	//Draw connecting supports from main ship part to rockets using scaled sphere
	shipPartColor = vec4(0.2f, 0.8f, 0.1f, 1.0f);
	glUniform4fv(fragColorUniform, 1, glm::value_ptr(shipPartColor));

	shipPart[3] = vec4(x, y - 10.0f, z, 1.0f);
	shipPart[0].x = 4.0f;
	shipPart[1].y = 2.0f;
	shipPart[2].z = 0.5f;
	glUniformMatrix4fv(worldUniform, 1, GL_FALSE, glm::value_ptr(shipPart));
	glDrawElements(GL_TRIANGLES, indicesSphere.size(), GL_UNSIGNED_SHORT, 0);

	shipPart[3] = vec4(x, y - 10.0f, z, 1.0f);
	shipPart[0].x = 0.5f;
	shipPart[1].y = 2.0f;
	shipPart[2].z = 4.0f;
	glUniformMatrix4fv(worldUniform, 1, GL_FALSE, glm::value_ptr(shipPart));
	glDrawElements(GL_TRIANGLES, indicesSphere.size(), GL_UNSIGNED_SHORT, 0);


	glBindVertexArray(0);

	//Draw Cylinders
	glBindVertexArray(vaoCylinder);

	shipPart = glm::mat4(1.0f); //reset to Id matrix 
	//draw 4 rocket parts
	shipPartColor = vec4(0.0f, 0.5f, 0.5f, 1.0f);
	glUniform4fv(fragColorUniform, 1, glm::value_ptr(shipPartColor));

	shipPart[3] = vec4(x - 8.0f, y - 10.0f, z, 1.0f);
	glUniformMatrix4fv(worldUniform, 1, GL_FALSE, glm::value_ptr(shipPart));
	glDrawElements(GL_TRIANGLES, indicesCylinder.size(), GL_UNSIGNED_SHORT, 0);

	
	shipPart[3] = vec4(x + 8.0f, y - 10.0f, z, 1.0f);
	glUniformMatrix4fv(worldUniform, 1, GL_FALSE, glm::value_ptr(shipPart));
	glDrawElements(GL_TRIANGLES, indicesCylinder.size(), GL_UNSIGNED_SHORT, 0);

	
	shipPart[3] = vec4(x, y - 10.0f, z + 8.0f, 1.0f);
	glUniformMatrix4fv(worldUniform, 1, GL_FALSE, glm::value_ptr(shipPart));
	glDrawElements(GL_TRIANGLES, indicesCylinder.size(), GL_UNSIGNED_SHORT, 0);

	shipPart[3] = vec4(x, y - 10.0f, z - 8.0f, 1.0f);
	glUniformMatrix4fv(worldUniform, 1, GL_FALSE, glm::value_ptr(shipPart));
	glDrawElements(GL_TRIANGLES, indicesCylinder.size(), GL_UNSIGNED_SHORT, 0);
	

	glBindVertexArray(0);
	
}





