
#include "Globals.h"
#include <GraphicsGL.h>

#define PI 3.141592f

Globals::Globals() {
	//initialize ship
	//this->ship = Spaceship();


	//initialize geometry

	
	//Initialize cameras
	float radius;
	float theta;
	float phi;

	radius = 100.0f;
	theta = phi = PI / 2; //set at (0, 0, 5)
	this->camera = Camera(radius, theta, phi);

	//camera looking down Z-axis
	this->camZ = Camera(radius, theta, phi);

	//camera looking down X-axis
	phi = 0.0f;
	this->camX = Camera(radius, theta, phi);
	
	this->viewX = CalcViewMatrix(this->camX.camPosition, this->camX.camTarget, this->camX.upDirection);
	this->viewZ = CalcViewMatrix(this->camZ.camPosition, this->camZ.camTarget, this->camZ.upDirection);

	this->geo.createSphere(100.0f, 20, 20, this->positionSphere3, this->elementSphere3);



	//initialize perspective projection matrix
	//initialize proj matrix
	this->nearZ = 1.0f;
	this->farZ = 100.0f;
	this->FOV = 45.0f;
	this->frustumScale = CalcFrustumScale(FOV);

	
	this->perspProj[0].x = frustumScale;
	this->perspProj[1].y = frustumScale;
	this->perspProj[2].z = (farZ + nearZ) / (nearZ - farZ);
	this->perspProj[2].w = -1.0f;
	this->perspProj[3].z = (2 * farZ * nearZ) / (nearZ - farZ);
	
	

	//initialize and set orthographic projection matrix
	this->orthoProj = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, -100.0f, 200.0f);


	//anti-aliasing
	this->msaaEnable = true;
	//wireframe mode
	this->wireframeEnable = false;
	this->textureOn = false;


	//setup frustum data
	this->frustum.push_back(vec3(-1.0f, -1.0f, 1.0f));
	this->frustum.push_back(vec3(1.0f, -1.0f, 1.0f));
	this->frustum.push_back(vec3(1.0f, 1.0f, 1.0f));
	this->frustum.push_back(vec3(-1.0f, 1.0f, 1.0f));
	this->frustum.push_back(vec3(-1.0f, -1.0f, -1.0f));
	this->frustum.push_back(vec3(1.0f, -1.0f, -1.0f));
	this->frustum.push_back(vec3(1.0f, 1.0f, -1.0f));
	this->frustum.push_back(vec3(-1.0f, 1.0f, -1.0f));
	//indices for frustum
	GLshort temp[] = {
		//front
		0, 1,
		1, 2,
		2, 3,
		3, 0,
		//back
		4, 5,
		5, 6,
		6, 7,
		7, 4,
		//left
		4, 0,
		0, 3,
		3, 7,
		7, 4,
		//right
		1, 5,
		5, 6, 
		6, 2,
		2, 1,
		//top
		3, 2,
		2, 6, 
		6, 7,
		7, 3,
		//bottom
		0, 1, 
		1, 5,
		5, 4,
		4, 0,
	};
	for(int i = 0; i < 48; i++) {
		this->frustumIndex[i] = temp[i];
	}




	//floor position
	this->floor.push_back(vec3(-1.0f, 0.0f, -1.0f));
	this->floor.push_back(vec3(1.0f, 0.0f, -1.0f));
	this->floor.push_back(vec3(1.0f, 0.0f, 1.0f));
	this->floor.push_back(vec3(-1.0f, 0.0f, 1.0f));
	//floor texture coords
	this->floorTex.push_back(vec2(1.0f, 1.0f));
	this->floorTex.push_back(vec2(0.0f, 1.0f));
	this->floorTex.push_back(vec2(0.0f, 0.0f));
	this->floorTex.push_back(vec2(1.0f, 0.0f));
	//floor index 
	this->floorIndex[0] = 0;
	this->floorIndex[1] = 2;
	this->floorIndex[2] = 1;
	this->floorIndex[3] = 0;
	this->floorIndex[4] = 3;
	this->floorIndex[5] = 2;



}

Globals::~Globals() {
	//delete texture
	glDeleteTextures(1, &this->texture);

	//delete buffers
	glDeleteBuffers(1, &this->positionBufferCylinder1);
	glDeleteBuffers(1, &this->positionBufferCylinder2);
	glDeleteBuffers(1, &this->positionBufferSphere1);
	glDeleteBuffers(1, &this->positionBufferSphere2);
	glDeleteBuffers(1, &this->positionBufferSphere3);
	glDeleteBuffers(1, &this->positionFloorBuffer);

	glDeleteBuffers(1, &this->elementBufferCylinder1);
	glDeleteBuffers(1, &this->elementBufferCylinder2);
	glDeleteBuffers(1, &this->elementBufferSphere1);
	glDeleteBuffers(1, &this->elementBufferSphere2);
	glDeleteBuffers(1, &this->elementBufferSphere3);
	glDeleteBuffers(1, &this->elementFloorBuffer);
	glDeleteBuffers(1, &this->frustumBuffer);
	glDeleteBuffers(1, &this->frustumIndexBuffer);
	glDeleteBuffers(1, &this->textureFloorBuffer);



	//delete buffers? and what else??
}