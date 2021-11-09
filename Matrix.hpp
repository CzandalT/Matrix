#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <cmath>
#include <exception>

namespace LinearAlgebra
{
	template <typename T>
	class Matrix
	{
		std::vector<std::vector<T>> context;
		size_t rows, columns;

	public:
		//default constructor
		Matrix();
		//default destructor
		~Matrix();
		//constructor
		Matrix(const size_t& M, const size_t& N);

		//constructor assigning values generated by the function
		Matrix(const size_t& M, const size_t& N,std::function<T()>W);

		//copying constructor
		Matrix(const Matrix<T>& Q);

		//accesses row of given index without boundary checks
		std::vector<T>& operator[](const size_t& index) { return context[index]; }

		constexpr const std::vector<T>& operator[](const size_t& index) const { return context[index]; }

		//accesses field of context without bondary checks
		T& operator()(const size_t& Row, const size_t& Col) {return context[Row][Col];}

		constexpr const T& operator()(const size_t& Row, const size_t& Col) const {return context[Row][Col]; }

		//accesses field of context with boundary checks
		constexpr T& at (const size_t& Row, const size_t& Col) noexcept(false) {return (Row<rows&&Col<columns)?context[Row][Col]:(throw std::out_of_range("Field of given row and col doesn't exist"));}

		constexpr const T& at(const size_t& Row, const size_t& Col) const noexcept(false) { return ((Row<rows&&Col<columns)?context[Row][Col]:throw std::out_of_range("Field of given row and col doesn't exist")); }

		//returns true iff two objects have are equal
		constexpr bool operator==(const Matrix<T>& other)const { return (context == other.context&&rows==other.rows&&other.columns); }

		//copies object
		Matrix<T> operator=(const Matrix<T>& index) noexcept;

		//addition of matrices
		Matrix<T> operator+(const Matrix<T>& W) const noexcept(false);

		//subtraction of matrices
		Matrix<T> operator-(const Matrix<T>& W) const noexcept(false);

		//scalar multiplication
		Matrix<T> operator*(const T& C) const noexcept;

		//scalar multiplication (by the inverse of arg)
		Matrix<T> operator/(const T& C) const noexcept(false);

		//Matrix<T> multiplication
		Matrix<T> operator*(const Matrix<T>& B) const noexcept(false);

		//matrix addition
		Matrix<T> operator+=(const Matrix<T>& W) noexcept(false);

		//matrix subtraction
		Matrix<T> operator-=(const Matrix<T>& W) noexcept(false);

		//scalar multiplication
		Matrix<T> operator*=(const T& C) noexcept;

		//matrix multiplication
		Matrix<T> operator*=(const Matrix<T>& W) noexcept(false);

		//scalar multiplication
		Matrix<T> operator/=(const T& C) noexcept(false);

		//return Matrix<T>'s transposition
		Matrix<T> transposed() const noexcept;

		//return the dot product of two matrices
		const T dot(const Matrix<T>& B) const noexcept(false);

		//print to std IO-stream
		void print(std::ostream&out=std::cout) const noexcept;

		//add another column to matrix
		void expandColumn(const std::vector<T>& newCol) noexcept(false);

		//add another row to matrix
		void expandRow(const std::vector<T>& newRow) noexcept(false);

		//returns row of given index (from 0 to N-1)
		std::vector<T> extractRow(size_t index) const noexcept;

		// returns row of given index (from 0 to N-1)
		std::vector<T> extractColumn(size_t index) const noexcept;

		//change values in a given row
		void changeRow(const std::vector<T>& row, const size_t& index) noexcept(false);

		//change values in a given column
		void changeColumn(const std::vector<T>& column, const size_t& index) noexcept(false);

		void copyFrom(const Matrix<T>& D) noexcept;

		void free() noexcept;

		//returns the sum of all the elements of the matrix
		const T sum() const noexcept;

		//return the supremum of set consisting of all the fields in matrix
		const T max() const noexcept;

		//element wise multiplication of two matrices
		Matrix<T> hadamardProduct(const Matrix<T>& B) const noexcept(false);
		
		Matrix<T> applyOperation(const Matrix<T>& other, std::function<T(const T&, const T&)>f) const noexcept(false);

		Matrix<T> applyOperation(std::function<T(const T&)>f) const noexcept;

		Matrix<T> modify(std::function<void(T&)>f) noexcept;

		Matrix<T> modify(std::function<T(const T&)>f) noexcept;
		
		size_t getCountRows() const noexcept { return rows; }
		size_t getCountColumns() const noexcept { return columns; }

		constexpr bool empty() const noexcept;

		constexpr T det() const noexcept(false);

		constexpr T cofactor(const size_t& i, const size_t& j) const noexcept(false);

		Matrix<T> adjoint() const noexcept(false);

		Matrix<T> inverse() const noexcept(false);
	};

	template<typename T>
	Matrix<T>::Matrix() :context(), rows(0), columns(0)
	{

	}

	template<typename T>
	Matrix<T>::~Matrix()
	{
		if (!context.empty())
		{
			context.erase(context.begin(), context.end());
		}
		rows = 0;
		columns = 0;
	}

	template<typename T>
	Matrix<T>::Matrix(const size_t& M, const size_t& N) :context(), rows(M), columns(N)
	{
		std::vector<T> E;
		for (size_t i = 0; i < N; i++)
		{
			E.push_back(0);
		}
		for (size_t i = 0; i < M; i++)
		{
			context.push_back(E);
		}
	}

	template<typename T>
	Matrix<T>::Matrix(const size_t& M, const size_t& N, std::function<T()> W) :context(), rows(M), columns(N)
	{
		for (size_t i = 0; i < M; i++)
		{
			std::vector<T> E;
			for (size_t i = 0; i < N; i++)
			{
				E.push_back(W());
			}
			context.push_back(E);
		}
	}

	template<typename T>
	Matrix<T>::Matrix(const Matrix<T>& Q) :columns(Q.columns), rows(Q.rows), context(Q.context)
	{
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator=(const Matrix<T>& index) noexcept
	{
		if (this == &index)
		{
			return *this;
		}
		this->context.erase(context.begin(), context.end());
		this->columns = index.columns;
		this->rows = index.rows;
		this->context = index.context;
		return *this;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator+(const Matrix<T>& W) const noexcept(false)
	{
		//if dimensions don't match addition is not defined
		if (rows != W.rows || columns != W.columns)
		{
			throw std::invalid_argument("Addition of matrices is undefined!");
		}
		Matrix<T> A(rows, columns);
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < columns; j++)
			{
				A[i][j] = context[i][j] + (W.context[i])[j];
			}
		}
		return A;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator-(const Matrix<T>& W) const noexcept(false)
	{
		//if dimensions don't match subtraction is not defined
		if (rows != W.rows || columns != W.columns)
		{
			throw std::invalid_argument("Subtraction of matrices is undefined!");
		}
		Matrix<T> A(rows, columns);
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < columns; j++)
			{
				A[i][j] = context[i][j] - W.context[i][j];
			}
		}
		return A;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator*(const T& C) const noexcept
	{
		Matrix<T> A(rows, columns);
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < columns; j++)
			{
				A[i][j] = context[i][j] * C;
			}
		}
		return A;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator/(const T& C) const noexcept(false)
	{
		if (!C)
		{
			throw std::invalid_argument("Division by zero is undefined!");
		}
		Matrix<T> A(rows, columns);
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < columns; j++)
			{
				A[i][j] = context[i][j] / C;
			}
		}
		return A;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator+=(const Matrix<T>& W) noexcept(false)
	{
		//if dimensions don't match addition is not defined
		if (rows != W.rows || columns != W.columns)
		{
			throw std::invalid_argument("Addition of matrices is undefined!");
		}
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < columns; j++)
			{
				this->context[i][j] += W.context[i][j];
			}
		}
		return *this;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator-=(const Matrix<T>& W) noexcept(false)
	{
		//if dimensions don't match subtraction is not defined
		if (rows != W.rows || columns != W.columns)
		{
			throw std::invalid_argument("Subtraction of matrices is undefined!");
		}
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < columns; j++)
			{
				this->context[i][j] -= W.context[i][j];
			}
		}
		return *this;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator*=(const T& C) noexcept
	{
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < columns; j++)
			{
				this->context[i][j] *= C;
			}
		}
		return *this;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator*=(const Matrix<T>& W) noexcept(false)
	{
		*this = (*this) * W;
		return *this;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator/=(const T& C) noexcept(false)
	{
		if (!C)
		{
			throw std::invalid_argument("Division by zero is undefined!");
		}
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < columns; j++)
			{
				this->context[i][j] /= C;
			}
		}
		return *this;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator*(const Matrix<T>& B) const noexcept(false)
	{
		if (this->columns != B.rows)
		{
			throw std::invalid_argument("Matrix multiplication undefined!");
		}
		Matrix<T> A(this->rows, B.columns);
		for (size_t i = 0; i < this->rows; i++)
		{
			for (size_t j = 0; j < B.columns; j++)
			{
				//row i
				//column j
				//i-th row of *this multiplied by j-th column of B
				for (size_t k = 0; k < this->columns; k++)
				{
					A[i][j] += this->context[i][k] * B.context[k][j];
				}
			}
		}
		return A;
	}

	template<typename T>
	Matrix<T> Matrix<T>::transposed() const noexcept
	{
		Matrix<T> A(columns, rows);
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < columns; j++)
			{
				A[j][i] = context[i][j];
			}
		}
		return A;
	}

	template<typename T>
	const T Matrix<T>::dot(const Matrix<T>& B) const noexcept(false)
	{
		if (B.rows != rows || B.columns != columns)
		{
			throw std::invalid_argument("Dot product is undefined for matrices of different dimensions!");
		}
		T s(0);
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < columns; j++)
			{
				s += B.context[i][j] * context[i][j];
			}
		}
		return s;
	}

	template<typename T>
	void Matrix<T>::print(std::ostream& out) const noexcept
	{
		for (const std::vector<T>& Q : context)
		{
			out << "|";
			for (const double& W : Q)
			{
				out << W << "|";
			}
			out << "\n";
		}
		out << "\n";
	}

	template<typename T>
	void Matrix<T>::expandColumn(const std::vector<T>& newCol) noexcept(false)
	{
		if (rows == 0)
		{
			for (size_t i = 0; i < newCol.size(); i++)
			{
				std::vector<T> newRow;
				newRow.push_back(newCol[i]);
				context.push_back(newRow);
			}
		}
		else
		{
			if (newCol.size() != rows)
			{
				throw std::invalid_argument("New column has to have as many records as there are rows!");
			}
			for (size_t i = 0; i < rows; i++)
			{
				context[i].push_back(newCol[i]);
			}
		}
		columns++;
	}

	template<typename T>
	void Matrix<T>::expandRow(const std::vector<T>& newRow) noexcept(false)
	{
		if (newRow.size() != columns && columns != 0)
		{
			throw std::invalid_argument("New row has to have as many records as there are columns!");
		}
		context.push_back(newRow);
		rows++;
		if (columns == 0) columns = newRow.size();
	}

	template<typename T>
	std::vector<T> Matrix<T>::extractRow(size_t index) const noexcept
	{
		return context[index];
	}

	template<typename T>
	std::vector<T> Matrix<T>::extractColumn(size_t index) const noexcept
	{
		std::vector<T> A;
		for (size_t i = 0; i < rows; i++)
		{
			A.push_back(context[i][index]);
		}
		return A;
	}

	template<typename T>
	void Matrix<T>::changeRow(const std::vector<T>& row, const size_t& index) noexcept(false)
	{
		if (row.size() != columns)
		{
			throw std::invalid_argument("Dimension of vector provided doesn't match dimensions of the matrix!");
		}
		for (size_t i = 0; i < columns; i++)
		{
			context[index][i]=row[i];
		}
	}

	template<typename T>
	void Matrix<T>::changeColumn(const std::vector<T>& column, const size_t& index)
	{
		if (column.size() != rows)
		{
			throw std::invalid_argument("Dimension of vector provided doesn't match dimensions of the matrix!");
		}
		for (size_t i = 0; i < rows; i++)
		{
			context[i][index] = column[i];
		}
	}

	template<typename T>
	void Matrix<T>::copyFrom(const Matrix<T>& D) noexcept
	{
		this->context = D.context;
		rows = D.rows;
		columns = D.columns;
	}

	template<typename T>
	void Matrix<T>::free() noexcept
	{
		context.erase(context.begin(), context.end());
		columns = 0;
		rows = 0;
	}

	template<typename T>
	const T Matrix<T>::sum() const noexcept
	{
		T S=0.0;
		for (const std::vector<T>& W : context)
		{
			for (const T& Q : W)
			{
				S += Q;
			}
		}
		return S;
	}

	template<typename T>
	const T Matrix<T>::max() const noexcept
	{
		T supremum=0.0;
		for (const std::vector<T>&row : context)
		{
			for (const T& value : row)
			{
				if (value > supremum)
				{
					supremum = value;
				}
			}
		}
		return supremum;
	}

	template<typename T>
	Matrix<T> Matrix<T>::hadamardProduct(const Matrix<T>& B) const noexcept(false)
	{
		if (B.rows != rows || B.columns != columns)
		{
			throw std::invalid_argument("Hadamard product is undefined for matrices of different dimensions!");
		}
		Matrix<T> returned(rows,columns);
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < columns; j++)
			{
				returned(i, j) = context[i][j] * B.context[i][j];
			}
		}
		return returned;
	}

	template<typename T>
	Matrix<T> Matrix<T>::applyOperation(const Matrix<T>& other, std::function < T(const T&, const T&)>f) const noexcept(false)
	{
		if (other.rows != rows || other.columns != columns)
		{
			throw std::invalid_argument("Function applyOperation is undefined for matrices of different dimensions!");
		}
		Matrix<T> result(this->rows, this->columns);
		for (size_t i = 0; i < this->rows; i++)
		{
			for (size_t j = 0; j < this->columns; j++)
			{
				result.at(i, j) = f(this->context[i][j], other.context[i][j]);
			}
		}
		return result;
	}

	template<typename T>
	Matrix<T> Matrix<T>::applyOperation(std::function < T(const T&)>f) const noexcept
	{
		Matrix<T> result(this->rows, this->columns);
		for (size_t i = 0; i < this->rows; i++)
		{
			for (size_t j = 0; j < this->columns; j++)
			{
				result.at(i, j) = f(this->context[i][j]);
			}
		}
		return result;
	}

	template<typename T>
	Matrix<T> Matrix<T>::modify(std::function<void(T&)> f) noexcept
	{
		for (std::vector<T>& row : context)
		{
			for (T& field : row)
			{
				f(field);
			}
		}
		return *this;
	}

	template<typename T>
	Matrix<T> Matrix<T>::modify(std::function<T(const T&)> f) noexcept
	{
		for (std::vector<T>& row : context)
		{
			for (T& field : row)
			{
				field=f(field);
			}
		}
		return *this;
	}

	template<typename T>
	constexpr bool Matrix<T>::empty() const noexcept
	{
		for (const std::vector<T>& row : context)
		{
			if (!row.empty()) { return false; }
		}
		return true;
	}

	template<typename T>
	constexpr T Matrix<T>::det() const noexcept(false)
	{
		if (rows == columns)
		{
			T det = 0;
			switch (rows)
			{
			case 0:
				return 1;
			case 1:
				return context[0][0];
			case 2:
				return context[0][0] * context[1][1] - context[0][1] * context[1][0];
			}
			for (size_t j = 0; j < columns; j++)
			{
				det += context[0][j]*cofactor(0,j);
			}
			return det;
		}
		else
		{
			throw std::domain_error("Determinant is undefined for non square matrix!");
		}
	}

	template<typename T>
	constexpr T Matrix<T>::cofactor(const size_t& i, const size_t& j) const noexcept(false)
	{
		Matrix<T> sub(rows - 1, columns - 1);
		size_t it = 0;
		for (size_t r = 0; r < rows; r++)
		{
			if (r != i)
			{
				for (size_t c = 0; c < columns; c++)
				{
					if (c != j)
					{
						sub(it / (columns-1), it % (columns-1)) = context[r][c];
						it++;
					}
				}
			}
		}
		return sub.det()*(std::pow(-1.l,i+j));
	}

	template<typename T>
	Matrix<T> Matrix<T>::adjoint() const noexcept(false)
	{
		Matrix ad(rows, columns);
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < columns; j++)
			{
				ad(i, j) = cofactor(i, j);
			}
		}
		return ad.transposed();
	}
	template<typename T>
	Matrix<T> Matrix<T>::inverse() const noexcept(false)
	{
		if (rows != columns)
		{
			throw std::domain_error("Inverse of matrix is undefined for non-square matrices!");
		}
		return adjoint()/det();
	}


	template<typename T>
	bool isnan(const LinearAlgebra::Matrix<T>& Mat) noexcept
	{
		for (size_t i = 0; i < Mat.getCountRows(); i++)
		{
			for (size_t j = 0; j < Mat.getCountColumns(); j++)
			{
				T W = Mat(i, j);
				if (std::isnan(W))
				{
					return true;
				}
			}
		}
		return false;
	}
}