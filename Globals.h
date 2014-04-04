#include <GL/glew.h>

#include "Spaceship.h"
#include "Geometry.h"
#include "Cam.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Globals {
public:
	Globals();
	~Globals();


	// Window 1 program 
	GLuint program1; //program
	GLuint vaoSphere1, vaoCylinder1, vaoSphere3; //VAO for sphere and cylinder
	GLuint worldUniform1; //uniforms
	GLuint viewUniform1; 
	GLuint projUniform1; 
	GLuint fragColorUniform1;
	//need to delete these in a close function before exiting main loop
	GLuint positionBufferSphere1;
	GLuint elementBufferSphere1; 
	GLuint positionBufferCylinder1;
	GLuint elementBufferCylinder1;



	// Window 2 program
	GLuint program2; //used for window 2, axis view
	GLuint vaoSphere2, vaoCylinder2; 
	GLuint worldUniform2; 
	GLuint viewUniform2; 
	GLuint projUniform2; 
	GLuint fragColorUniform2;
	
	//delete these too
	GLuint positionBufferSphere2;
	GLuint elementBufferSphere2; 
	GLuint positionBufferCylinder2;
	GLuint elementBufferCylinder2;

	//Sphere data for 
	GLuint positionBufferSphere3;
	GLuint elementBufferSphere3;

	std::vector<glm::vec3> positionSphere3;
	std::vector<GLshort> elementSphere3;



	//Frustum 
	GLuint program3; //program used to draw frustum
	GLuint vaoFrustum;
	GLuint worldUniform;
	GLuint viewUniform;
	GLuint projUniform;
	GLuint inverseViewUniform;
	GLuint inverseProjUniform;

	vector<glm::vec3> frustum;
	GLshort frustumIndex[48]; //is this bad form to actually define in the header, because this is fixed and defined for the frustum

	GLuint frustumBuffer;
	GLuint frustumIndexBuffer;




	//floor 
	GLuint programT;
	GLuint vaoT;

	//uniforms
	GLuint worldTUniform;
	GLuint viewTUniform;
	GLuint projTUniform;

	GLuint textureUniform;

	//buffers for data
	GLuint positionFloorBuffer;
	GLuint textureFloorBuffer;
	GLuint elementFloorBuffer;

	GLuint texture; //used to store texture image

	vector<glm::vec3> floor;
	vector<glm::vec2> floorTex;
	GLshort  floorIndex[6];









	//Cameras
	Camera camera;
	Camera camX; //looking down X-axis
	Camera camZ; //looking down Z-axis
	glm::mat4 viewX;
	glm::mat4 viewZ;



	//Spaceship ship;

	//Geometry shapes;
	Geometry geo;

	//Projections
	glm::mat4 orthoProj;
	glm::mat4 perspProj;
	float frustumScale;
	float nearZ;
	float farZ;
	float FOV;



	//options
	//anti-aliasing
	bool msaaEnable;
	//wireframe mode
	bool wireframeEnable;
	//texture mode
	bool textureOn;


	//first window dimenstion
	int windowWidth1;
	int windowHeight1;
	//used for second window for 2 viewports in one window
	int windowWidth2;
	int windowHeight2;
	int windowHalfWidth2;


	//could have data members to hold data for shapes/geometry??

};