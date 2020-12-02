#ifndef __MATRIX_CPP
#define __MATRIX_CPP
# define M_PI           3.14159265358979323846 
#include "Matrix.h"

// Parameter Constructor                                                                                                                                                      
template<typename T>
Matrix<T>::Matrix(unsigned _rows, unsigned _cols, const T& _initial) {
	mat.resize(_rows);

	//…
	// Complete code to initialise member elements
	for (int i = 0; i < _rows; i++) {
		mat[i].resize(_cols, _initial);
	}
	//…

	rows = _rows;
	cols = _cols;
}

// Copy Constructor                                                                                                                                                           
template<typename T>
Matrix<T>::Matrix(const Matrix<T>& rhs) {
	mat = rhs.mat;
	rows = rhs.get_rows();
	cols = rhs.get_cols();

}


template<typename T>
Matrix<float> Matrix<T>::CreateRotationMatrixY(float angle)
{
	Matrix yRot(3, 3, 0.0);

	yRot(0, 0) = cos(angle* M_PI / 180);
	yRot(0, 1) = 0;
	yRot(0, 2) = sin(angle* M_PI / 180);

	yRot(1, 0) = 0;
	yRot(1, 1) = 1;
	yRot(1, 2) = 0;

	yRot(2, 0) = -sin(angle *M_PI / 180);
	yRot(2, 1) = 0;
	yRot(2, 2) = cos(angle * M_PI / 180);
	return yRot;
}

// (Virtual) Destructor                                                                                                                                                       
template<typename T>
Matrix<T>::~Matrix() {}
// Assignment Operator                                                                                                                                                        
template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& rhs) {
	if (&rhs == this)
		return *this;

	for (unsigned i = 0; i < rhs.get_rows(); i++) {
		for (unsigned j = 0; j < rhs.get_cols(); j++) {
			this->mat[i][j] = rhs(i, j);
		}
	}

	return *this;
}

// Addition of two matrices                                                                                                                                                   
template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& rhs) {
	Matrix result(rows, cols, 0.0);

	for (unsigned i = 0; i < rows; i++) {
		for (unsigned j = 0; j < cols; j++) {
			result(i, j) = this->mat[i][j] + rhs(i, j);

		}
	}

	return result;
}

// Cumulative addition of this matrix and another                                                                                                                             
template<typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& rhs) {

	for (unsigned i = 0; i < rows; i++) {
		for (unsigned j = 0; j < cols; j++) {

			this->mat[i][j] += rhs(i, j);
		}
	}


	return *this;
}

// Subtraction of this matrix and another                                                                                                                                     
template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& rhs) {

	Matrix result(rows, cols, 0.0);
	for (unsigned i = 0; i < rows; i++) {
		for (unsigned j = 0; j < cols; j++) {
			result(i, j) = this->mat[i][j] - rhs(i, j);
		}
	}

	return result;
}

// Cumulative subtraction of this matrix and another                                                                                                                          
template<typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& rhs) {

	for (unsigned i = 0; i < rows; i++) {
		for (unsigned j = 0; j < cols; j++) {
			this->mat[i][j] -= rhs(i, j);
		}
	}

	return *this;
}

// Left multiplication of this matrix and another                                                                                                                              
template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& rhs) {
	Matrix result(rows, cols, 0.0);
	//Rows of other must equal columns of this
	if (rhs.get_cols() != rows) {//Bug the rows does n
		return *this;
	}
	for (unsigned i = 0; i < rows; i++) {
		for (unsigned j = 0; j < rows; j++) {
			for (unsigned k = 0; k < cols; k++) {

				result(i, k) = this->mat[i][k] * rhs(i, k);

			}
		}
	}
	return result;
}

// Cumulative left multiplication of this matrix and another                                                                                                                  
template<typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& rhs) {

	Matrix result(rows, cols, 0.0);
	if (rhs.get_rows() != cols) {
		return *this;
	}
	for (unsigned i = 0; i < rows; i++) {
		for (unsigned j = 0; i < cols; i++) {

			for (unsigned k = 0; k < cols; k++) {

				this->mat[i][k] *= rhs(i, k);

			}
		}
	}
	return *this;
}

// Calculate a transpose of this matrix                                                                                                                                       
template<typename T>
Matrix<T> Matrix<T>::transpose() {

	// Swap the column size for the row size in the result

	Matrix result(cols, rows, 0.0);
	//result.mat.resize(rows);

	//set colum dimensions

	//for eveery row
	for (unsigned i = 0; i < cols; i++) {

		//for every column
		for (unsigned j = 0; j < rows; j++) {


			result(i, j) = this->mat[j][i];
		}
	}


	return result;
}

// Matrix/scalar addition                                                                                                                                                     
template<typename T>
Matrix<T> Matrix<T>::operator+(const T& rhs) {

	Matrix result(rows, cols, 0.0);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			result(i, j) = this[i][j] + rhs;
		}
	}

	return result;
}

// Matrix/scalar subtraction                                                                                                                                                  
template<typename T>
Matrix<T> Matrix<T>::operator-(const T& rhs) {

	Matrix result(rows, cols, 0.0);

	for (int i = 0; i < result.get_rows(); i++) {
		for (int j = 0; j < result.get_cols(); j++) {
			result(i, j) = this->mat[i][j] - rhs;
		}
	}

	return result;
}

// Matrix/scalar multiplication                                                                                                                                               
template<typename T>
Matrix<T> Matrix<T>::operator*(const T& rhs) {

	Matrix result(rows, cols, 0.0);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {

			result(i, j) = this->mat[i][j] * rhs;
		}
	}
	return result;
}

// Matrix/scalar division                                                                                                                                                     
template<typename T>
Matrix<T> Matrix<T>::operator/(const T& rhs) {

	Matrix result(rows, cols, 0.0);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			result(i, j) = this->mat[i][j] / rhs;
		}
	}
	return result;
}

// Multiply a matrix with a vector                                                                                                                                            
template<typename T>
std::vector<T> Matrix<T>::operator*(const std::vector<T>& rhs) {
	std::vector<T> result(rhs.size(), 0.0);

	if (rhs.size() != cols) {
		return rhs;
	}
	for (int i = 0; i < rhs.size(); i++) {
		for (int j = 0; j < cols; j++) {
			result[i] = this->mat[i][j] * rhs[i];
		}
	}

	return result;
}

// Obtain a vector of the diagonal elements                                                                                                                                   
template<typename T>
std::vector<T> Matrix<T>::diag_vec() {
	std::vector<T> result(cols, 0.0);

	for (int i = 0; i < cols; i++) {
		result[i] = this->mat[i][0 + i];

	}
	return result;
}

// Access the individual elements                                                                                                                                             
template<typename T>
T& Matrix<T>::operator()(const unsigned& row, const unsigned& col) {
	return this->mat[row][col];
}

// Access the individual elements (const)                                                                                                                                     
template<typename T>
const T& Matrix<T>::operator()(const unsigned& row, const unsigned& col) const {
	return this->mat[row][col];
}

// Get the number of rows of the matrix                                                                                                                                       
template<typename T>
unsigned Matrix<T>::get_rows() const {
	return this->rows;
}

// Get the number of columns of the matrix                                                                                                                                    
template<typename T>
unsigned Matrix<T>::get_cols() const {
	return this->cols;
}

#endif
