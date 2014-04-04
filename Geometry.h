#pragma once
#include <glm.hpp>
using glm::vec3;
#include <GL/freeglut.h>
#include <vector>

using namespace std;

/* This class uses functions taken from 3D Graphics Programming by Frank Luna. Taken from examples used in book and source code, common folder, GeometryHelper. */

class Geometry {
public:
	Geometry();
	~Geometry();


	void static createSphere(float radius, int sliceCount, int stackCount, vector<glm::vec3>& position, vector<GLshort>& index);
	void static createGeosphere(float radius, int numSubDivisions, vector<glm::vec3>& position, vector<GLshort>& index);
	void static createCylinder(float topRadius, float bottomRadius, float height, unsigned int sliceCount, unsigned int stackCount, vector<vec3>& position, vector<GLshort>& index);


private:

	

	void static subDivide(std::vector<vec3>& pos, std::vector<GLshort>& indices);
	void static buildCylinderTop(float bottomRadius, float topRadius, float height, unsigned int sliceCount, unsigned int stackCount, vector<vec3>& position, vector<GLshort>& index);
	void static buildCylinderBottom(float bottomRadius, float topRadius, float height, unsigned int sliceCount, unsigned int stackCount, vector<vec3>& position, vector<GLshort>& index);

};