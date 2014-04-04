#pragma once
#include "Geometry.h"

#include <fstream>

#define PI 3.141592f

Geometry::Geometry() {
	
}

Geometry::~Geometry() {

}

void Geometry::createSphere(float radius, int sliceCount, int stackCount, vector<glm::vec3>& position, vector<GLshort>& index) {

	glm::vec3 top(0.0f, +radius, 0.0f);
	glm::vec3 bottom(0.0f, -radius, 0.0f);

	position.push_back(top);

	float phiStep = PI/stackCount;
	float thetaStep = 2.0f*PI/sliceCount;

	for(int i = 0; i <= stackCount-1; i++) {
		float phi = i * phiStep;

		for(int j = 0; j <= sliceCount; j++) {
			float theta = j * thetaStep;

			glm::vec3 v;


			v.x = radius * sinf(phi) * cosf(theta);
			v.y = radius * cosf(phi);
			v.z = radius * sinf(phi) * sinf(theta);

			position.push_back(v);
		}
	}

	position.push_back(bottom);

	//compute indices

	for(int i = 0; i <= sliceCount; i++) {
		index.push_back(0);
		index.push_back(i+1);
		index.push_back(i);
	}

	int baseIndex = 1;
	int ringVertexCount = sliceCount + 1;

	for(int i = 0; i < stackCount-2; i++) {
		for(int j = 0; j < sliceCount; j++) {
			index.push_back(baseIndex + i * ringVertexCount + j);
			index.push_back(baseIndex + i * ringVertexCount + j + 1);
			index.push_back(baseIndex + (i+1) * ringVertexCount + j);

			index.push_back(baseIndex + (i+1) * ringVertexCount + j);
			index.push_back(baseIndex + i * ringVertexCount + j + 1);
			index.push_back(baseIndex + (i+1) * ringVertexCount + j + 1);
		}
	}

	int southPoleIndex = (int)(position.size()-1);

	baseIndex = southPoleIndex - ringVertexCount;

	for(int i = 0; i < sliceCount; i++) {
		index.push_back(southPoleIndex);
		index.push_back(baseIndex + i);
		index.push_back(baseIndex + i + 1);
	}

}

void Geometry::createGeosphere(float radius, int numSubDivisions, vector<glm::vec3>& position, vector<GLshort>& index){
	
	//test: build icosahedron
	const float x = 0.525731f;
	const float z = 0.850651f;

	vec3 pos[12] = {
		vec3(-x, 0.0f, z),  vec3(x, 0.0f, z),
		vec3(-x, 0.0f, -z), vec3(x, 0.0f, -z),
		vec3(0.0f, z, x),   vec3(0.0f, z, -x),
		vec3(0.0f, -z, x),  vec3(0.0f, -z, -x),
		vec3(z, x, 0.0f),   vec3(-z, x, 0.0f),
		vec3(z, -x, 0.0f),  vec3(-z, -x, 0.0f)
	};


	const GLshort indices[60] = {
		1,4,0,  4,9,0,  4,5,9,  8,5,4,   1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,   3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11,  6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,   11,2,7
	};

	position.resize(12);
	index.resize(60);
	for(unsigned int i = 0; i < 12; i++) {
		position.at(i) = pos[i];
	}
	for(unsigned int i = 0; i < 60; i++) {
		index.at(i) = indices[i];
	}

	for(int i = 0; i < numSubDivisions; i++) {
		subDivide(position, index);
	}
	
	//project vertices onto sphere and scale
	//why am i getting warning here for '<' mismatch?? type mismatch? fixed it, unsigned int instead of int
	for(unsigned int i = 0; i < position.size(); i++) {
		//set positon data
		vec3 temp = position.at(i);
		//printf("\n (%.2f, %.2f, %.2f)", temp.x, temp.y, temp.z);
		vec3 n = glm::normalize(position.at(i));
		vec3 p = radius * n;
		position.at(i) = p;

		//set color data

		//set texture data

		//set normal data
	}
	
}



void Geometry::subDivide(std::vector<vec3>& pos, std::vector<GLshort>& indices) {
	std::vector<vec3> copyVertex = pos;
	std::vector<GLshort> copyIndex = indices;

	pos.resize(0);
	indices.resize(0);

	unsigned int numTriangles = copyIndex.size()/3;
	for(unsigned int i = 0; i < numTriangles; i++) {
		//get current triangle
		vec3 v0 = copyVertex.at( copyIndex.at(i*3+0) );
		vec3 v1 = copyVertex.at( copyIndex.at(i*3+1) );
		vec3 v2 = copyVertex.at( copyIndex.at(i*3+2) );
		//generate midpoints
		vec3 m0, m1, m2;

		m0 = vec3(
			0.5f*(v0.x + v1.x),
			0.5f*(v0.y + v1.y),
			0.5f*(v0.z + v1.z));
		m1 = vec3( 
			0.5f*(v1.x + v2.x),
			0.5f*(v1.y + v2.y),
			0.5f*(v1.z + v2.z));
		m2 = vec3(
			0.5f*(v0.x + v2.x),
			0.5f*(v0.y + v2.y),
			0.5f*(v0.z + v2.z));

		//add the newly create geometry, new vertices and indices back to arrays
		pos.push_back(v0);
		pos.push_back(v1);
		pos.push_back(v2);
		pos.push_back(m0);
		pos.push_back(m1);
		pos.push_back(m2);

		indices.push_back(i*6+0);
		indices.push_back(i*6+3);
		indices.push_back(i*6+5);

		indices.push_back(i*6+3);
		indices.push_back(i*6+4);
		indices.push_back(i*6+5);

		indices.push_back(i*6+5);
		indices.push_back(i*6+4);
		indices.push_back(i*6+2);

		indices.push_back(i*6+3);
		indices.push_back(i*6+1);
		indices.push_back(i*6+4);
	}

}


void Geometry::createCylinder(float topRadius, float bottomRadius, float height, unsigned int sliceCount, unsigned int stackCount, vector<vec3>& position, vector<GLshort>& index) {

	//build stacks
	float stackHeight = height / stackCount;
	//change in radius as we move up each stack level, from bottom to top
	float radiusDelta = (topRadius - bottomRadius) / stackCount;

	unsigned int ringCount = stackCount + 1;

	//compute vertices for each stack ring starting at bottom and moving up
	for(unsigned int i = 0; i < ringCount; i++) {
		float y = -0.5f*height + i*stackHeight;
		float r = bottomRadius + i*radiusDelta;

		//vertices of ring
		float dTheta = (float)(2.0f * 3.141592 / sliceCount);
		for(unsigned int j = 0; j <= sliceCount; j++) {
			vec3 vertex;
			float c = cosf(j*dTheta);
			float s = sinf(j*dTheta);
			vertex = vec3(r*c, y, r*s);
			//texture coordinates computed here
			//normal coputed here as well

			position.push_back(vertex);
		}
	}

	//add one because we duplicate the first and last vertex per ring
	//since the texture coords are different
	unsigned int ringVertexCount = sliceCount + 1;

	for(unsigned int i = 0; i < stackCount; i++) {
		for(unsigned int j = 0; j < sliceCount; j++) {
			index.push_back(i*ringVertexCount + j);
			index.push_back((i+1)*ringVertexCount + j);
			index.push_back((i+1)*ringVertexCount + j+1);

			index.push_back(i*ringVertexCount + j);
			index.push_back((i+1)*ringVertexCount + j+1);
			index.push_back(i*ringVertexCount + j+1);
		}
	}
	//functions needed for top and bottom
	buildCylinderTop(bottomRadius, topRadius, height, sliceCount, stackCount, position, index);
	buildCylinderBottom(bottomRadius, topRadius, height, sliceCount, stackCount, position, index);
}

void Geometry::buildCylinderTop(float bottomRadius, float topRadius, float height, unsigned int sliceCount, unsigned int stackCount, vector<vec3>& position, vector<GLshort>& index) {
	unsigned int baseIndex = (unsigned int)position.size();

	float y = 0.5f*height;
	float dTheta = (float)(2.0f * 3.14159f / sliceCount);
	//duplicate cap ring vertices because the texture coordinates and normals differ
	for(unsigned int i = 0; i <= sliceCount; i++) {
		float x = topRadius * cosf(i*dTheta);
		float z = topRadius * sinf(i*dTheta);

		position.push_back(vec3(x, y, z));
	}

	position.push_back(vec3(0.0f, y, 0.0f));

	//indices 
	unsigned int centerIndex = (unsigned int)(position.size() - 1);

	for(unsigned int i = 0; i < sliceCount; i++) {
		index.push_back(centerIndex);
		index.push_back(baseIndex + i+1);
		index.push_back(baseIndex + i);
	}
}

void Geometry::buildCylinderBottom(float bottomRadius, float topRadius, float height, unsigned int sliceCount, unsigned int stackCount, vector<vec3>& position, vector<GLshort>& index) {
	unsigned int baseIndex = (unsigned int)position.size();
	float y = -0.5f * height;

	//vertices of ring
	float dTheta = (float)(2.0f * 3.14159f / sliceCount);
	for(unsigned int i = 0; i <= sliceCount; i++) {
		float x = bottomRadius * cosf(i*dTheta);
		float z = bottomRadius * sinf(i*dTheta);

		position.push_back(vec3(x, y, z));
	}
	//cap center vertex
	position.push_back(vec3(0.0f, y, 0.0f));

	unsigned int centerIndex = (unsigned int)(position.size() - 1);

	for(unsigned int i = 0; i < sliceCount; i++) {
		index.push_back(centerIndex);
		index.push_back(baseIndex + i);
		index.push_back(baseIndex + i+1);
	}
}