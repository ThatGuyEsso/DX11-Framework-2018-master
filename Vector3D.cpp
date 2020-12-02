#include "Vector3D.h"

Vector3D::Vector3D()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector3D::Vector3D(float x1, float y1, float z1)
{
	x = x1;
	y = y1;
	z = z1;

}

Vector3D::Vector3D(const Vector3D& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;

}

Vector3D Vector3D::operator+(const Vector3D& vec)
{
	//Returns a new vector summing the values for each component with the 
	  //corresponding component in the added vector

	return Vector3D(x + vec.x, y + vec.y, z + vec.z);
}

Vector3D& Vector3D::operator+=(const Vector3D& vec)
{

	//Returns ‘this’ pointer (i.e. self-reference summing the values for 
	//each component with the corresponding component in the added vector


	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;

}

Vector3D Vector3D::operator-(const Vector3D& vec)
{
	//Returns a new vector subtracting the values for each component with the 
	//corresponding component in the subtracted vector


	return Vector3D(x - vec.x, y - vec.y, z - vec.z);
}

Vector3D& Vector3D::operator-=(const Vector3D& vec)
{
	//Subtracts value of corrosponding vector from left hand 
	
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	//Returns new current vector
	return *this;
}

Vector3D Vector3D::operator*(float value)
{
	return Vector3D(x*value, y * value, z * value);
}

Vector3D& Vector3D::operator*=(float value)
{
	//Mutiplies each vector componenet by scalar

	x *= value;
	y *= value;
	z *= value;
	//Returns new value of current vector
	return *this;
}

Vector3D Vector3D::operator/(float value)
{
	assert(value != 0); //prevent divide by 0
	return Vector3D(x / value, y/ value, z / value);
}

Vector3D& Vector3D::operator/=(float value)
{
	assert(value != 0); //prevent divide by 0
	x /= value;
	y /= value;
	z /= value;
	//Returns new value of current vector
	return *this;
}

Vector3D& Vector3D::operator=(const Vector3D& vec)
{

	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;

}

float Vector3D::dot_product(const Vector3D& vec)
{
	//returns (x1*x2 + y1*y2 + x1*z2) where these are the terms from
	// each vector 

	return ((x * vec.x) + (y * vec.y) + (z * vec.z));

}

Vector3D Vector3D::cross_product(const Vector3D& vec)
{
	float ni = y * vec.z - z * vec.y;;
	float nj = z * vec.x - x * vec.z;
	float nk= x*vec.y - y*vec.x;
	return Vector3D(ni,nj,nk);
}

Vector3D Vector3D::normalization()
{
	
	return  *this /= magnitude();;
}

float Vector3D::square()
{
	return x * x + y * y + z * z;
}

float Vector3D::distance(const Vector3D& vec)
{
	Vector3D vectorToPoint =  *this - vec;
	return vectorToPoint.magnitude();
}

float Vector3D::magnitude()
{
	float squaredValue = square();
	return sqrt(squaredValue);
}

float Vector3D::show_X()
{
	return x;
}

float Vector3D::show_Y()
{
	return y;
}

float Vector3D::show_Z()
{
	return z;
}

void Vector3D::disp()
{
	std::cout << x << " " << y << " " << z << std::endl;
}

