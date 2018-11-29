#include "MyVector2.h"
#include <math.h>
#include "Game.h"
/*
author Jack Malone
C00236428
estimated time taken: 3 hours
each of the functions perform different vector maths calculations
each of the functions are tested in the mainline using the debugger and test cases I worked out on paper beforehand
the program calculates the vector length, the vector length squared, the vector cross product, the vector dot product,
the angle between 2 vectors, a vector being rotated through an angle, vector projection for 2 vectors, scalar rejection,
vector scalar projection and the unit vector for a given vector
known bugs: none
*/

/// <summary>
/// get length of vector using sqrt of the sum of the squares
/// </summary>
/// <param name="t_vector">input vector</param>
/// <returns>length</returns>
float vectorLength(sf::Vector2f t_vector)
{
	float sumOfSquares = (t_vector.x * t_vector.x ) + (t_vector.y * t_vector.y);
	const float length = sqrt(sumOfSquares);
	return length;
}
//get the length squared by gettting the sum of the squares
//gets a vector and returns the length squared
float vectorLengthSquared(sf::Vector2f t_vector)
{
	float lengthSquared = (t_vector.x * t_vector.x) + (t_vector.y * t_vector.y);
	return lengthSquared;
}
//gets the vector cross product of 2 vectors using the formula (t_vectorA.x * t_VectorB.y) - (t_vectorA.y * t_VectorB.x)
//takes 2 vectors and returns the vector cross product
float vectorCorssProduct(sf::Vector2f t_vectorA, sf::Vector2f t_VectorB)
{
	float vectorCrossProduct = (t_vectorA.x * t_VectorB.y) - (t_vectorA.y * t_VectorB.x);
	return vectorCrossProduct;
}
//get the dot product by multiplying the two x values and adding the result to the two y values multiplied together
//takes two vectors as input
float vectorDotProduct(sf::Vector2f t_vectorA, sf::Vector2f t_VectorB)
{
	float dotProduct = (t_vectorA.x * t_VectorB.x) + (t_vectorA.y * t_VectorB.y);
	return dotProduct;
}
//finds the angle between two vectors by getting the cos inverse of the dot product divided by the product of the magnitudes
//takes two vectors as input and returns an angle in radians
float vectorAngleBetween(sf::Vector2f t_vectorA, sf::Vector2f t_VectorB)
{
	float cos = vectorDotProduct(vectorUnitVector(t_vectorA), vectorUnitVector(t_VectorB));
	if (cos > 1.0f) {
		cos = 1.0f;
	}
	if (cos < -1.0f) {
		cos = -1.0f;
	}
	const float ANGLE_RADIANS = std::acos(cos);
	const float ANGLE_DEGREES = ANGLE_RADIANS * 180.0f / m_PI;
	return ANGLE_DEGREES;
}
//rotates a vector through a certain amount of degrees and gives the resulting vector.
//finds new x value with the formula (cosOfAngle * t_vector.x) - (sinOfAngle * t_vector.y)
//finds the new y value with the formula (sinOfAngle * t_vector.x) + (cosOfAngle * t_vector.y)
//takes a vector and an angle in radians and returns a vector
sf::Vector2f vectorRotateBy(sf::Vector2f t_vector, float t_angleRadians)
{
	const float COS_OF_ANGLE = std::cos(t_angleRadians);
	const float SIN_OF_ANGLE = std::cos(t_angleRadians);
	const float X_COMPONENT = (COS_OF_ANGLE * t_vector.x) - (SIN_OF_ANGLE * t_vector.y);
	const float Y_COMPONENT = (SIN_OF_ANGLE * t_vector.x) + (COS_OF_ANGLE * t_vector.y);
	const sf::Vector2f VECTOR_ROTATED(X_COMPONENT, Y_COMPONENT);
	return VECTOR_ROTATED;
}

//gets the vector projection of two vectors by multiplying the dot product divided by the magnitude by the first vector divided
//by the magnitude of the other vector
//takes two vectors as input and returns the projected vector
sf::Vector2f vectorProjection(sf::Vector2f t_vector, sf::Vector2f t_onto)
{
	float dotProduct = vectorDotProduct(t_vector, t_onto);
	float length = vectorLengthSquared(t_onto);
	float scale = dotProduct / length;
	sf::Vector2f projectedVector(scale * t_onto);
	return projectedVector;
}
//vector that is perpendicular to the first vector
//resultant of this vector and the other vector is the vector projection
//found using the formula t_vector - projectedVector
//takes two vectors as inputs and returns the rejected vector
sf::Vector2f vectorRejection(sf::Vector2f t_vector, sf::Vector2f t_onto)
{
	sf::Vector2f projectedVector(vectorProjection(t_vector, t_onto));
	sf::Vector2f rejectedVector(t_vector - projectedVector);
	
	return rejectedVector;
}
//gets the scalar projection by dividing the dot product by the magnitude squared of one of the vectors
//takes two vectors as input and returns a scalar
float vectorScalarProjection(sf::Vector2f t_vector, sf::Vector2f t_onto)
{
	float dotProduct = vectorDotProduct(t_vector, t_onto);
	float length = vectorLength(t_onto);
	float scalarProjection = dotProduct / length;
	return scalarProjection;
}
//finds the unit vector for a given vector by dividing the vector by its magnitude
//takes a vector as an input and returns a vector
sf::Vector2f vectorUnitVector(sf::Vector2f t_vector)
{
	sf::Vector2f unitVector(0.0f, 0.0f);
	const float LENGTH = vectorLength(t_vector);
	if (LENGTH != 0.0f) {
		unitVector = sf::Vector2f{t_vector.x / LENGTH, t_vector.y / LENGTH};
	}
	return unitVector;
}
