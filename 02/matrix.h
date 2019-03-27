#pragma once
#include <vector>
using namespace std;
class Matrix
{
private:

	class Row
	{
	public:
		Row(int col_tmp = 0) : col_num(col_tmp),row(vector<int>(col_tmp,0))
		{}
		const int& operator[](size_t i) const
		{
			if (i >= col_num)
				throw std::out_of_range("i > col_num");
			return row[i];
		}
		int& operator[](size_t i)
		{
			if (i >= col_num)
				throw std::out_of_range("i > col_num");
			return row[i];
		}

		size_t get_col() const
		{
			return col_num;
		}

		Row& operator*=(const int num)
		{
			for (auto& i : row)
				i *= num;
			return *this;
		}

		bool operator==(const Row& co_row) const
		{
			for (size_t i = 0; i < col_num; i++)
				if (row[i] != co_row[i])
					return false;
			return true;
		}

		bool operator!=(const Row& co_row) const
		{
			return !(*this == co_row);
		}
		~Row()
		{}

	private:

		size_t col_num;
		vector<int> row;
	};

	size_t row_num;
	size_t col_num;
	vector<Row> M;

public:
	Matrix(size_t row_tmp, size_t col_tmp) : row_num(row_tmp),col_num(col_tmp),M(vector<Row>(row_tmp , Row(col_num)))
	{}

	size_t getRows() const
	{
		return this->row_num;
	}

	size_t getColumns() const
	{
		return this->col_num;
	}

	const Row& operator[](size_t i) const
	{
		if (i >= row_num)
			throw std::out_of_range("i > row num");
		return M[i];
	}

	Row& operator[](size_t i)
	{
		if (i >= row_num)
			throw std::out_of_range("i > row num");
		return M[i];
	}

	Matrix& operator*=(const int num)
	{
		for (auto& i : M)
			i *= num;
		return *this;
	}

	bool operator==(const Matrix& co_matr) const
	{
		if (this->row_num != co_matr.row_num)
			return false;
		if (this->col_num != co_matr.col_num)
			return false;
		for (size_t i = 0; i < row_num; i++)
			if (M[i] != co_matr[i])
				return false;
		return true;
	}

	bool operator!=(const Matrix& co_matr) const
	{
		return !(*this == co_matr);
	}

	~Matrix()
	{}
};