#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <cmath>

namespace LinearAlgebra
{
	static const std::string __ExceptionMatrixMultiplicationUndefined("Matrix multiplication is valid only if number of columns of left one is equal to number of rows of the right one!");
	static const std::string __ExceptionMatrixDeterminantUndefined("Matrix determinant is valid only for square matrices!");
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

		//accesses row of given index
		std::vector<T>& operator[](const size_t& index) { return context[index]; }

		constexpr const std::vector<T>& operator[](const size_t& index) const { return context[index]; }

		//accesses field of context
		T& operator()(const size_t& Row, const size_t& Col) {return context[Row][Col];}

		constexpr const T& operator()(const size_t& Row, const size_t& Col) const {return context[Row][Col]; }

		//accesses field of context
		T& at (const size_t& Row, const size_t& Col) {return context[Row][Col];}

		const T& get(const size_t& Row, const size_t& Col) const { return context[Row][Col];}

		//
		constexpr bool operator==(const Matrix<T>& other)const { return context == other.context; }

		//copies object
		Matrix<T> operator=(const Matrix<T>& index);

		//addition of matrices
		Matrix<T> operator+(const Matrix<T>& W) const;

		//subtraction of matrices
		Matrix<T> operator-(const Matrix<T>& W) const;

		//scalar multiplication
		Matrix<T> operator*(const T& C) const;

		//scalar multiplication (by the inverse of arg)
		Matrix<T> operator/(const T& C) const;

		//Matrix<T> multiplication
		Matrix<T> operator*(const Matrix<T>& B) const;

		//matrix addition
		Matrix<T> operator+=(const Matrix<T>& W);

		//matrix subtraction
		Matrix<T> operator-=(const Matrix<T>& W);

		//subtract constant C from each record
		Matrix<T> operator-=(const T& C);

		//scalar multiplication
		Matrix<T> operator*=(const T& C);

		//matrix multiplication
		Matrix<T> operator*=(const Matrix<T>& W);

		//scalar multiplication
		Matrix<T> operator/=(const T& C);		

		//return Matrix<T>'s transposition
		Matrix<T> transposed() const;

		//return the dot product of two matrices
		const T dot(const Matrix<T>& B) const;

		//print to std IO-stream
		void print(std::ostream&out=std::cout) const;

		//add another column to matrix
		void expandColumn(const std::vector<T>& newCol);

		//add another row to matrix
		void expandRow(const std::vector<T>& newRow);

		//returns row of given index (from 0 to N-1)
		std::vector<T> extractRow(size_t index) const;

		// returns row of given index (from 0 to N-1)
		std::vector<T> extractColumn(size_t index) const;

		//change values in a given row
		void changeRow(const std::vector<T>& row, const size_t& index);

		//change values in a given column
		void changeColumn(const std::vector<T>& column, const size_t& index);

		void copyFrom(const Matrix<T>& D);

		void free();

		//returns the sum of all the elements of the matrix
		const T sum() const;

		//return the supremum of set consisting of all the fields in matrix
		const T max() const;

		//element wise multiplication of two matrices
		Matrix<T> hadamardProduct(const Matrix<T>& B) const;
		
		Matrix<T> applyOperation(const Matrix<T>& other, std::function<T(const T&, const T&)>f) const;
		
		size_t getCountRows() const { return rows; }
		size_t getCountColumns() const { return columns; }

		constexpr bool empty() const;

		constexpr T det() const;

		constexpr T cofactor(const size_t& i, const size_t& j) const;

		Matrix<T> adjoint() const;
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
	Matrix<T> Matrix<T>::operator=(const Matrix<T>& index)
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
	Matrix<T> Matrix<T>::operator+(const Matrix<T>& W) const
	{

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
	Matrix<T> Matrix<T>::operator-(const Matrix<T>& W) const
	{

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
	Matrix<T> Matrix<T>::operator*(const T& C) const
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
	Matrix<T> Matrix<T>::operator/(const T& C) const
	{

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
	Matrix<T> Matrix<T>::operator+=(const Matrix<T>& W)
	{

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
	Matrix<T> Matrix<T>::operator-=(const Matrix<T>& W)
	{

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
	Matrix<T> Matrix<T>::operator-=(const T& C)
	{
		for (std::vector<T>& row : context)
		{
			for (T& value : row)
			{
				value -= C;
			}
		}
		return *this;
	}
	template<typename T>
	Matrix<T> Matrix<T>::operator*=(const T& C)
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
	inline Matrix<T> Matrix<T>::operator*=(const Matrix<T>& W)
	{

		Matrix<T> E((*this) * W);
		*this = E;
		return *this;
	}

	template<typename T>
	Matrix<T> Matrix<T>::operator/=(const T& C)
	{
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
	Matrix<T> Matrix<T>::operator*(const Matrix<T>& B) const//prduct of two matrices (remember - not commutative)
	{
		if (this->columns != B.rows)
		{
			throw __ExceptionMatrixMultiplicationUndefined;
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
	Matrix<T> Matrix<T>::transposed() const
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
	const T Matrix<T>::dot(const Matrix<T>& B) const
	{

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
	void Matrix<T>::print(std::ostream& out) const
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
	void Matrix<T>::expandColumn(const std::vector<T>& newCol)
	{

		for (size_t i = 0; i < rows; i++)
		{
			context[i].push_back(newCol[i]);
		}
		columns++;
	}

	template<typename T>
	inline void Matrix<T>::expandRow(const std::vector<T>& newRow)
	{
		context.push_back(newRow);
		rows++;
		if (columns == 0) columns = newRow.size();
	}

	template<typename T>
	inline std::vector<T> Matrix<T>::extractRow(size_t index) const
	{

		return context[index];
	}

	template<typename T>
	inline std::vector<T> Matrix<T>::extractColumn(size_t index) const
	{

		std::vector<T> A;
		for (size_t i = 0; i < rows; i++)
		{
			A.push_back(context[i][index]);
		}
		return A;
	}

	template<typename T>
	inline void Matrix<T>::changeRow(const std::vector<T>& row, const size_t& index)
	{

		for (size_t i = 0; i < columns; i++)
		{
			context[index][i]=row[i];
		}
	}

	template<typename T>
	inline void Matrix<T>::changeColumn(const std::vector<T>& column, const size_t& index)
	{

		for (size_t i = 0; i < rows; i++)
		{
			context[i][index] = column[i];
		}
	}

	template<typename T>
	inline void Matrix<T>::copyFrom(const Matrix<T>& D)
	{
		//assume this->rows=D.rows and this->columns=D.columns and *this!=D
		this->context = D.context;
	}

	template<typename T>
	inline void Matrix<T>::free()
	{
		context.erase(context.begin(), context.end());
		columns = 0;
		rows = 0;
	}

	template<typename T>
	inline const T Matrix<T>::sum() const
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
	inline const T Matrix<T>::max() const
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
	Matrix<T> Matrix<T>::hadamardProduct(const Matrix<T>& B) const
	{

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
	Matrix<T> Matrix<T>::applyOperation(const Matrix<T>& other, std::function < T(const T&, const T&)>f) const
	{

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
	inline constexpr bool Matrix<T>::empty() const
	{
		for (const std::vector<T>& row : context)
		{
			if (!row.empty()) { return false; }
		}
		return true;
	}

	template<typename T>
	constexpr T Matrix<T>::det() const
	{
		if (rows == columns && rows != 0)
		{
			T det = 0;
			switch (rows)
			{
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
			throw __ExceptionMatrixDeterminantUndefined;
		}
	}

	template<typename T>
	constexpr T Matrix<T>::cofactor(const size_t& i, const size_t& j) const
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
	Matrix<T> Matrix<T>::adjoint() const
	{
		Matrix ad(rows, columns);
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < columns; j++)
			{
				ad(i, j) = cofactor(i, j);
			}
		}
		return ad;
	}



	template<typename T>
	void modify(Matrix<T>& M,std::function<void(T&)> f)
	{
		for (size_t i = 0; i < M.getCountRows(); i++)
		{
			for (size_t j = 0; j < M.getCountColumns(); j++)
			{
				f(M[i][j]);
			}
		}
	}

	template<typename T>
	void modify(Matrix<T>& M, void(*f)(T&))
	{

		for (size_t i = 0; i < M.getCountRows(); i++)
		{
			for (size_t j = 0; j < M.getCountColumns(); j++)
			{
				f(M[i][j]);
			}
		}
	}

	template<typename T>
	Matrix<T> applyFunction(Matrix<T> M, std::function<T(T)> f)
	{

		for (size_t i = 0; i < M.getCountRows(); i++)
		{
			for (size_t j = 0; j < M.getCountColumns(); j++)
			{
				M(i, j) = f(M(i, j));
			}
		}
		return M;
	}

	

	template<typename T>
	bool isnan(LinearAlgebra::Matrix<T> Mat)
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