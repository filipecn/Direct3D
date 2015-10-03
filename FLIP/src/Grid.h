#pragma once

#include "common\NumericalTypes.hpp"

#include <glm\glm.hpp>

enum class WrapMode { BORDER, EDGE, REPEAT };

template<class T, typename P = float>
class Grid {
public:
	Grid(int w, int h);
	
	~Grid();

	glm::dvec2 offset;
	glm::ivec2 size;
	T border;

	T& operator() (int i, int j);
	T  operator() (int i, int j) const;

	T sample(P i, P j);

private:
	T **data;
};

template<class T, typename P = float>
Grid<T,P>::Grid(int w, int h) {
	size = glm::ivec2(w, h);
	
	data = new T*[w];
	for (int i = 0; i < w; i++)
		data[i] = new T[h];

	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
			data[i][j] = 10;
}

template<class T, typename P = float>
Grid<T,P>::~Grid() {
	for (int i = 0; i < size[0]; i++)
		delete[] data[i];
	delete[] data;
}

template<class T, typename P = float>
T& Grid<T, P>::operator() (int i, int j) {
	assert(data != nullptr);
	assert(i >= 0 && i < size[0] && j >= 0 && j < size[1]);
	return data[i][j];
}

template<class T, typename P = float>
T Grid<T, P>::operator() (int i, int j) const {
	assert(data != nullptr);
	assert(i >= 0 && i < size[0] && j >= 0 && j < size[1]);
	return data[i][j];
}

template<class T, typename P = float>
T Grid<T, P>::sample(P i, P j) {

}