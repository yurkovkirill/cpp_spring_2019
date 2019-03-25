#pragma once
#include <vector>
using namespace std;
class Matrix
{
private:

	class Row
	{
	public:
		Row(int col_tmp = 0) : col_num(col_tmp)
		{
			this->row = vector<int>(col_num,0);
		}
		const int& operator[](unsigned i) const
		{
			if (i >= col_num)
				throw std::out_of_range("i > col_num");
			return row[i];
		}
		int& operator[](unsigned i)
		{
			if (i >= col_num)
				throw std::out_of_range("i > col_num");
			return row[i];
		}

		unsigned get_col() const
		{
			return col_num;
		}

		Row& operator*=(const int num)
		{
			for (unsigned i = 0; i < col_num; i++)
				row[i] *= num;
			return *this;
		}

		const bool operator==(const Row& co_row) const
		{
			for (unsigned i = 0; i < col_num; i++)
				if (row[i] != co_row[i])
					return false;
			return true;
		}

		const bool operator!=(const Row& co_row) const
		{
			for (unsigned i = 0; i < col_num; i++)
				if (row[i] != co_row[i])
					return true;
			return false;
		}
		~Row()
		{
			row.clear();
		}

	private:

		unsigned col_num;
		vector<int> row;
	};

	unsigned row_num;
	unsigned col_num;
	vector<Row> M;

public:
	Matrix(unsigned row_tmp, unsigned col_tmp) : row_num(row_tmp),col_num(col_tmp)
	{
		M = std::vector<Row>(row_num);
		for (unsigned i = 0; i < row_num; i++)
			M[i] = Row(col_num);
	}

	unsigned getRows() const
	{
		return this->row_num;
	}

	unsigned getColumns() const
	{
		return this->M[0].get_col();
	}

	const Row& operator[](unsigned i) const
	{
		if (i >= row_num)
			throw std::out_of_range("i > row num");
		return M[i];
	}

	Row& operator[](unsigned i)
	{
		if (i >= row_num)
			throw std::out_of_range("i > row num");
		return M[i];
	}

	Matrix& operator*=(const int num)
	{
		for (unsigned i = 0; i < row_num; i++)
			M[i] *= num;
		return *this;
	}

	const bool operator==(const Matrix& co_matr) const
	{
		if (this->row_num != co_matr.row_num)
			return false;
		if (this->col_num != co_matr.col_num)
			return false;
		for (unsigned i = 0; i < row_num; i++)
			if (M[i] != co_matr[i])
				return false;
		return true;
	}

	const bool operator!=(const Matrix& co_matr) const
	{
		if (this->row_num != co_matr.row_num)
			return true;
		if (this->col_num != co_matr.col_num)
			return true;
		for (unsigned i = 0; i < row_num; i++)
			if (M[i] != co_matr[i])
				return true;
		return false;
	}

	~Matrix()
	{
		M.clear();
	}
};