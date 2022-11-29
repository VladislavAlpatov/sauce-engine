/*
 * Copyright (c) 2022.
 * Created by Alpatov Softworks with love in Russia.
 */

#include "matrix.h"
#include <utility>
#include <stdexcept>
namespace math
{
	matrix::matrix(const size_t rows, const size_t columns)
	{
		if (rows == 0 and columns == 0)
			throw std::runtime_error("Matrix cannot be 0x0");

		m_iRows = rows;
		m_iColumns = columns;

		m_ppData = Allocate2DArray(m_iRows, m_iColumns);

		set(0.f);
	}

	matrix::matrix(const std::vector<std::vector<float>>& rows)
	{
		m_iRows = rows.size();
		m_iColumns = rows[0].size();

		m_ppData = Allocate2DArray(m_iRows, m_iColumns);

		for (size_t i = 0; i < m_iRows; ++i)
			for (size_t j = 0; j < m_iColumns; ++j)
				m_ppData[i][j] = rows[i][j];
	}

	matrix::matrix(const matrix& other)
	{
		m_iRows = other.m_iRows;
		m_iColumns = other.m_iColumns;

		m_ppData = Allocate2DArray(m_iRows, m_iColumns);

		for (size_t i = 0; i < m_iRows; ++i)
			for (size_t j = 0; j < m_iColumns; ++j)
				at(i, j) = other.at(i, j);
	}

	matrix::matrix(const size_t rows, const size_t columns, float* pRaw)
	{
		m_iRows = rows;
		m_iColumns = columns;

		m_ppData = Allocate2DArray(m_iRows, m_iColumns);

		for (size_t i = 0; i < m_iRows; ++i)
		{
			memcpy(*(m_ppData + i), pRaw, sizeof(float) * m_iColumns);
			pRaw += m_iColumns;
		}

	}

	size_t matrix::get_rows_count() const
	{

		return m_iRows;
	}

	matrix::matrix(matrix&& other) noexcept
	{
		m_iRows = other.m_iRows;
		m_iColumns = other.m_iColumns;
		m_ppData = other.m_ppData;

		other.m_ppData = nullptr;
	}

	size_t matrix::get_columns_count() const
	{
		return m_iColumns;
	}

	std::pair<size_t, size_t> matrix::get_size() const
	{
		return { get_rows_count(), get_columns_count() };
	}

	float& matrix::at(const size_t iRow, const size_t iCol)
	{
		return m_ppData[iRow][iCol];
	}

	float matrix::get_sum()
	{
		float sum = 0;

		for (size_t i = 0; i < m_iRows; i++)
			for (size_t j = 0; j < m_iColumns; j++)
				sum += at(i, j);

		return sum;
	}

	const float& matrix::at(const size_t iRow, const size_t iCol) const
	{
		return m_ppData[iRow][iCol];
	}

	matrix matrix::operator*(const matrix& other) const
	{
		if (m_iColumns != other.m_iRows)
			throw std::runtime_error("n != m");

		auto outMat = matrix(m_iRows, other.m_iColumns);

		for (size_t d = 0; d < m_iRows; ++d)
			for (size_t i = 0; i < other.m_iColumns; ++i)
				for (size_t j = 0; j < other.m_iRows; ++j)
					outMat.at(d, i) += at(d, j) * other.at(j, i);

		return outMat;
	}

	matrix& matrix::operator+=(const matrix& other)
	{
		if (other.m_iColumns != m_iColumns or other.m_iRows != m_iRows)
			throw std::runtime_error("matrices is not equal");

		for (size_t i = 0; i < m_iRows; i++)
			for (size_t j = 0; j < m_iColumns; j++)
				at(i, j) += other.at(i, j);

		return *this;
	}

	matrix matrix::operator*(const float f) const
	{
		auto out = *this;
		for (size_t i = 0; i < m_iRows; ++i)
			for (size_t j = 0; j < m_iColumns; ++j)
				out.at(i, j) *= f;

		return out;
	}

	matrix& matrix::operator*=(const float f)
	{
		for (size_t i = 0; i < get_rows_count(); i++)
			for (size_t j = 0; j < get_columns_count(); j++)
				at(i, j) *= f;
		return *this;
	}

	void matrix::clear()
	{
		set(0.f);
	}
	void matrix::print() const
	{
		for (size_t i = 0; i < get_rows_count(); i++)
		{
			for (size_t j = 0; j < get_columns_count(); j++)
			{
				printf("%.3f ", at(i, j));
			}
			printf("\n");

		}
	}

	matrix& matrix::operator=(const matrix& other)
	{
		if (this == &other)
			return *this;
		this->~matrix();
		m_ppData = Allocate2DArray(other.m_iRows, other.m_iColumns);

		for (size_t i = 0; i < m_iRows; ++i)
			for (size_t j = 0; j < m_iColumns; ++j)
				at(i, j) = other.at(i, j);

		return *this;

	}

	matrix& matrix::operator=(matrix&& other) noexcept
	{
		if (this == &other)
			return *this;

		std::swap(m_iRows, other.m_iRows);
		std::swap(m_iColumns, other.m_iColumns);
		std::swap(m_ppData, other.m_ppData);

		return *this;

	}

	matrix& matrix::operator/=(const float f)
	{
		for (size_t i = 0; i < m_iRows; ++i)
			for (size_t j = 0; j < m_iColumns; ++j)
				at(i, j) /= f;

		return *this;
	}

	matrix& matrix::operator*=(const matrix& other)
	{
		*this = *this * other;
		return *this;
	}

	matrix matrix::operator/(const float f) const
	{
		auto out = *this;
		for (size_t i = 0; i < m_iRows; ++i)
			for (size_t j = 0; j < m_iColumns; ++j)
				out.at(i, j) /= f;

		return out;
	}

	float matrix::det() const
	{
		if (m_iRows + m_iColumns == 2)
			return at(0, 0);

		if (m_iRows == 2 and m_iColumns == 2)
			return at(0, 0) * at(1, 1) - at(0, 1) * at(1, 0);

		float fDet = 0;
		for (size_t i = 0; i < m_iColumns; i++)
			fDet += alg_complement(0, i) * at(0, i);

		return fDet;
	}

	float matrix::alg_complement(const size_t i, const size_t j) const
	{
		const auto tmp = minor(i, j);
		return (!((i + j + 2) % 2)) ? tmp : -tmp;
	}

	matrix matrix::transpose()
	{
		matrix transposed = { m_iColumns, m_iRows };

		for (size_t i = 0; i < m_iRows; ++i)
			for (size_t j = 0; j < m_iColumns; ++j)
				transposed.at(j, i) = at(i, j);

		return transposed;
	}

	matrix::~matrix()
	{
		// If move constructor moved pointer to another class
		// so we must NOT delete this memory
		if (!m_ppData) return;

		for (size_t i = 0; i < m_iRows; i++)
			delete[] m_ppData[i];
		delete[] m_ppData;
	}

	matrix matrix::rotation_x(const float angle)
	{
		return matrix({
			{ 1.f, 0.f, 0.f, 0.f },
			{ 0.f, cosf(angle), sinf(angle), 0.f },
			{ 0.f, -sinf(angle), cosf(angle), 0.f },
			{ 0.f, 0.f, 0.f, 1.f }
		});

	}

	matrix matrix::rotation_y(const float angle)
	{
		return matrix({
			{ cosf(angle), sinf(angle), 0.f, 0.f },
			{ -sinf(angle), cosf(angle), 0.f, 0.f },
			{ 0.f, 0.f, 1.f, 0.f },
			{ 0.f, 0.f, 0.f, 1.f }
		});

	}

	matrix matrix::rotation_z(const float angle)
	{
		return matrix({
			{ cosf(-angle), -sinf(angle), 0.f, 0.f },
			{ 0.f, 1.f, 0.f, 0.f },
			{ sinf(angle), 0.f, cosf(angle), 0.f },
			{ 0.f, 0.f, 0.f, 1.f }
		});

	}

	float** matrix::Allocate2DArray(const size_t i, const size_t j)
	{
		const auto pArr = new float* [i];

		for (size_t x = 0; x < i; ++x)
			pArr[x] = new float[j];

		return pArr;
	}
	void matrix::set(const float val)
	{
		for (size_t i = 0; i < m_iRows; ++i)
			for (size_t j = 0; j < m_iColumns; ++j)
				at(i, j) = val;
	}
	matrix matrix::strip(const size_t row, const size_t column) const
	{
		matrix stripped = { m_iRows - 1, m_iColumns - 1 };
		size_t iStripRowIndex = 0;

		for (size_t i = 0; i < m_iRows; i++)
		{
			if (i == row)
				continue;

			size_t iStripColumnIndex = 0;
			for (size_t j = 0; j < m_iColumns; ++j)
			{
				if (j == column)
					continue;

				stripped.at(iStripRowIndex, iStripColumnIndex) = at(i, j);
				iStripColumnIndex++;
			}

			iStripRowIndex++;
		}

		return stripped;
	}

	float matrix::minor(const size_t i, const size_t j) const
	{
		return strip(i, j).det();
	}
}