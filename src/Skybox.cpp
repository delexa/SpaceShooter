// Local file
#include "Skybox.h"

// GLM files
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Skybox::Skybox(std::shared_ptr<Camera> camera)
{
	cam_ = camera;
}
Skybox::~Skybox()
{

}
void Skybox::privateInit()
{
	loadCubemap();

	shader_.initShaders("../Shaders/skybox");
	shader_.enable();
}

GLuint Skybox::getTextures()
{
	return cubeTextures_;
}

void Skybox::loadCubemap()
{
	cubeTextures_ = SOIL_load_OGL_cubemap
	(

		"../textures/skybox_east.bmp",			//east
		"../textures/skybox_west.bmp",			// west
		"../textures/skybox_up.bmp",				// up
		"../textures/skybox_down.bmp",			// down
		"../textures/skybox_north.bmp",			// north
		"../textures/skybox_south.bmp",			// south
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS

	);


}

void Skybox::drawCube()
{
	float size = 1.0f;

	float A[3] = { size,  size,  size };
	float B[3] = { size,  size, -size };
	float C[3] = { size, -size, -size };
	float D[3] = { size, -size,  size };
	float E[3] = { -size,  size,  size };
	float F[3] = { -size,  size, -size };
	float G[3] = { -size, -size, -size };
	float H[3] = { -size, -size,  size };

	float I[3] = { 1.0f,  0.0f,  0.0f };
	float K[3] = { -1.0f,  0.0f,  0.0f };
	float L[3] = { 0.0f,  0.0f, -1.0f };
	float M[3] = { 0.0f,  0.0f,  1.0f };
	float N[3] = { 0.0f,  1.0f,  0.0f };
	float O[3] = { 0.0f, -1.0f,  0.0f };


	glBegin(GL_QUADS);

	glNormal3fv(I);

	glTexCoord2f(1, 1);
	glVertex3fv(D);
	glTexCoord2f(0, 1);
	glVertex3fv(C);
	glTexCoord2f(0, 0);
	glVertex3fv(B);
	glTexCoord2f(1, 0);
	glVertex3fv(A);

	glNormal3fv(K);

	glTexCoord2f(1, 1);
	glVertex3fv(G);
	glTexCoord2f(0, 1);
	glVertex3fv(H);
	glTexCoord2f(0, 0);
	glVertex3fv(E);
	glTexCoord2f(1, 0);
	glVertex3fv(F);

	glNormal3fv(L);

	glTexCoord2f(1, 1);
	glVertex3fv(C);
	glTexCoord2f(0, 1);
	glVertex3fv(G);
	glTexCoord2f(0, 0);
	glVertex3fv(F);
	glTexCoord2f(1, 0);
	glVertex3fv(B);

	glNormal3fv(M);

	glTexCoord2f(1, 1);
	glVertex3fv(H);
	glTexCoord2f(0, 1);
	glVertex3fv(D);
	glTexCoord2f(0, 0);
	glVertex3fv(A);
	glTexCoord2f(1, 0);
	glVertex3fv(E);

	glNormal3fv(N);

	glTexCoord2f(1, 1);
	glVertex3fv(E);
	glTexCoord2f(0, 1);
	glVertex3fv(A);
	glTexCoord2f(0, 0);
	glVertex3fv(B);
	glTexCoord2f(1, 0);
	glVertex3fv(F);

	glNormal3fv(O);

	glTexCoord2f(1, 1);
	glVertex3fv(G);
	glTexCoord2f(0, 1);
	glVertex3fv(C);
	glTexCoord2f(0, 0);
	glVertex3fv(D);
	glTexCoord2f(1, 0);
	glVertex3fv(H);

	glEnd();
}

void Skybox::privateRender()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	shader_.enable();
	glFrontFace(GL_CW);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTextures_);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	float v[8][3] =
	{
		{ -1.0f,-1.0f,1.0f },
		{ 1.0f,-1.0f,1.0f },
		{ 1.0f,1.0f,1.0f },
		{ -1.0f,1.0f,1.0f },
		{ -1.0f,-1.0f,-1.0f },
		{ 1.0f,-1.0f,-1.0f },
		{ 1.0f,1.0f,-1.0f },
		{ -1.0f,1.0f,-1.0f } };

	unsigned int i[24] =
	{
		0,1,2,3,
		1,5,6,2,
		5,4,7,6,
		4,0,3,7,
		0,4,5,1,
		2,6,7,3

	};

	glBegin(GL_QUADS);
	for (int k = 0; k < 24; ++k)
	{
		glTexCoord3f(v[i[k]][0], v[i[k]][1], v[i[k]][2]);
		glVertex3f(v[i[k]][0], v[i[k]][1], v[i[k]][2]);
	}
	glEnd();

	glDisable(GL_TEXTURE_CUBE_MAP);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	shader_.disable();

	//drawCube();

}
void Skybox::privateUpdate()
{
	matrix_[3][0] = -cam_->getMatrix()[3][0];
	matrix_[3][1] = -cam_->getMatrix()[3][1];
	matrix_[3][2] = -cam_->getMatrix()[3][2];
}