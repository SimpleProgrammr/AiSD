//
// Created by Elmo on 16.03.2026.
//

#include "GridMap.h"

void GridMap::_fill_grid_with(const unsigned char val = 0) const {
    for (int i = 0; i < _height; i++) {
        _data[i]._fill_row_with(val);
    }
}

GridMap::GridMap( long height, long width, unsigned char fill ) {
    _height = height;
    _width = width;
    _data = new _row[_height];
    for (int i = 0; i < _height; i++) {
        _data[i] = _row(_width);
    }
    _fill_grid_with(0);
}

long GridMap::getHeight() const {return _height;}

long GridMap::getWidth() const { return _width;}

unsigned char GridMap::operator()(long height, long width) const { return _data[height][width]; }

GridMap::_row GridMap::operator[](const long height) const {
    return _data[height];
}

