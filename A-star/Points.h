//
// Created by Elmo on 16/03/2026.
//
#pragma once
#include <cmath>

typedef struct point {
    long x;
    long y;
    [[nodiscard]] bool isEqual(struct point p) const {
        return x == p.x && y == p.y;
    }
    [[nodiscard]] double getDistance(struct point p)const {
        return sqrt( pow( x - p.x, 2 ) + pow(y - p.y, 2 ) );
    }

    bool operator== (const point &p) {
        return x == p.x && y == p.y;
    };
} POINT;

inline bool sortPointsByX(const POINT p1, const POINT p2) {
    return p1.x > p2.x;
}