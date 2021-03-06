#include "SpaceShip.hpp"
#include "SOIL.h"
#include <iostream>

SpaceShip::SpaceShip()
{
	this->currentWeaponType = "MachineGun";
	mashineGunAmountBullets = 100;
	laserAmountBullets = 500;
	this->life_ = 100;
	this->armor_ = 100;
	maxX = 15;
	minX = 0;
	maxY = 15;
	minY = 0;
	maxZ = 15;
	minZ = 0;
}

SpaceShip::~SpaceShip()
{
}

void SpaceShip::moveRight()
{
	if (matrix_[3][0] <= 50.f)
		matrix_ = glm::translate(matrix_, glm::vec3(5.0f, 0.0f, 0.0f));
}

void SpaceShip::moveLeft()
{
	if (matrix_[3][0] >= -50.f)
		matrix_ = glm::translate(matrix_, glm::vec3(-5.0f, 0.0f, 0.0f));
}


void SpaceShip::moveUp()
{
	matrix_ = glm::translate(matrix_, glm::vec3(0.0f, 5.0f, 0.0f));
}

void SpaceShip::moveDown()
{
	matrix_ = glm::translate(matrix_, glm::vec3(0.0f, -5.0f, 0.0f));
}

void SpaceShip::moveBackward()
{
	matrix_ = glm::translate(matrix_, glm::vec3(0.0f, 0.0f, 5.0f));
}

void SpaceShip::moveForward()
{
	matrix_ = glm::translate(matrix_, glm::vec3(0.0f, 0.0f, -5.0f));
}


void SpaceShip::setWeapon(std::string weaponName)
{
	this->currentWeaponType = weaponName;
}

std::string SpaceShip::getCurrentWeapon()
{
	return this->currentWeaponType;
}

int SpaceShip::getLaserAmountBullets()
{
	return this->laserAmountBullets;
}

int SpaceShip::getMashineGunAmountBullets()
{
	return this->mashineGunAmountBullets;
}

bool SpaceShip::shoot()
{
	if (currentWeaponType == "MachineGun" && mashineGunAmountBullets > 0)
	{
		mashineGunAmountBullets -= 1;
		return true;
	}

	if (currentWeaponType == "Laser"  && laserAmountBullets > 0)
	{
		laserAmountBullets -= 1;
		return true;
	}

	return false;
}

void SpaceShip::reload()
{
	if (currentWeaponType == "MachineGun")
		mashineGunAmountBullets = 100;

	if (currentWeaponType == "Laser")
		laserAmountBullets = 500;
}

void SpaceShip::privateInit()
{
	//list_id = glGenLists(1);
	//glNewList(list_id, GL_COMPILE);	//begin new list

	//glColor3f(0.0f, 0.0f, 1.0f);
	//float size = 5.0f;

	//glBegin(GL_QUADS);
	//glNormal3f(0.0f, 0.0f, 1.0f);
	//glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, size); vertexArray_.push_back(std::vector<float>{ -size, -size, size});
	//glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, size); vertexArray_.push_back(std::vector<float>{size, -size, size});
	//glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, size); vertexArray_.push_back(std::vector<float>{ size, size, size});
	//glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, size); vertexArray_.push_back(std::vector<float>{-size, size, size});
	//glEnd();

	//glEndList();//end new list


	//texture initiation
	int width, height;
	unsigned char* img = SOIL_load_image("../textures/particle.png", &width, &height, 0, SOIL_LOAD_RGB);//color

	glGenTextures(1, &particleTexture_);
	glBindTexture(GL_TEXTURE_2D, particleTexture_);

	if (!img)
		std::cout << "Not loaded" << sizeof(img) << std::endl;

	if (particleTexture_ == NULL) {

		printf("Error in download '%s'", SOIL_last_result());
	}
	else {
		std::cout << "Texture loaded: " << "size " << sizeof(img) << " height " << height << " width " << width << std::endl;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

		SOIL_free_image_data(img);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	///*particles_ptr.reset(new ParticlesEngineClass(particleTexture_));
	//this->addSubObject(particles_ptr);*/

	matrix_ = glm::translate(glm::mat4(), glm::vec3(0.0f, 10.0f, -5.0f));


	myModel = std::make_shared<Model>("../Models/starwars-tie-fighter.obj");
	myModel->Import3DFromFile();


}

void SpaceShip::privateRender()
{
	//glCallList(list_id);
	myModel->DrawGLScene();
}

void SpaceShip::privateUpdate()
{
}

void SpaceShip::getSpaceShipPosition()
{
}

// get position
glm::vec3 SpaceShip::getPos() const
{
	return glm::vec3(matrix_[3][0], matrix_[3][1], matrix_[3][2]);
}


float SpaceShip::getRadius() const
{
	auto eps = 1e-5;

	float centX1, centY1, centZ1, radius;

	if ((minX + maxX) / 2 < eps) centX1 = 0;
	else centX1 = (minX + maxX) / 2;

	if ((minY + maxY) / 2 < eps) centY1 = 0;
	else centY1 = (minY + maxY) / 2;

	float distanceX1 = 0.0f;
	float xVarMax1 = std::abs(maxX) - std::abs(centX1);
	float xVarMin1 = std::abs(minX) - std::abs(centX1);

	if (xVarMax1 > xVarMin1) distanceX1 = xVarMax1;
	else distanceX1 = xVarMin1;

	float distanceY1 = 0.0f;
	float yVarMax1 = std::abs(maxY) - std::abs(centY1);
	float yVarMin1 = std::abs(minY) - std::abs(centY1);

	if (yVarMax1 > yVarMin1) distanceY1 = yVarMax1;
	else distanceY1 = yVarMin1;

	if (distanceX1 > distanceY1) radius = distanceX1;
	else radius = distanceY1;

	return radius;

}

float SpaceShip::getLife()
{
	return this->life_;
}

float SpaceShip::getArmor()
{
	return this->armor_;
}

void SpaceShip::setLife(float damage)
{
	this->life_ -= damage;
}

void SpaceShip::setArmor(float damage)
{
	this->armor_ -= damage;
}

