#pragma once
#include <GL/freeglut.h>
#include <glm.hpp>
using glm::vec3;
using glm::vec2;
#include <vector>

class Spaceship {
public:

	//Sphere data
	std::vector<vec3> positionDataSphere;
	std::vector<GLshort> indicesSphere;


	//Cylinder data
	std::vector<vec3> positionDataCylinder;
	std::vector<GLshort> indicesCylinder;


	//Ship data
	std::vector<vec3> position;
	std::vector<GLint> index;


public:
	Spaceship(); //initialize vertex data for objects and construct a spaceship
	~Spaceship();

	void init();


	void render(glm::vec3 pos, GLuint& vaoSphere, GLuint& vaoCylinder, GLuint& worldUniform, GLuint& fragColorUniform);

};