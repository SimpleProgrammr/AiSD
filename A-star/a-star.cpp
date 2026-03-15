#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <list>
#include <ostream>
#include <random>
using namespace std;

typedef struct point {
    int x;
    int y;
    bool isEqual(struct point p) const {
        return x == p.x && y == p.y;
    }
    double getDistance(struct point p)const {
        return sqrt( pow( x - p.x, 2 ) + pow(y - p.y, 2 ) );
    }
} POINT;

void establish_nearest(POINT p, POINT goal, double *min_dist, POINT* near_point) {
    double distance = p.getDistance(goal);
    if (*min_dist >= distance) {
        *min_dist = distance;
        *near_point = p;
    }
}

template <int height, int width>
list<POINT> A_star_on_grid(unsigned char grid[height][width], POINT start, POINT goal) {
    if (start.isEqual(goal))
        return list<POINT>{goal};

    list<POINT> usedPoints = list<POINT>();
    usedPoints.push_back(start);
    grid[start.x][start.y] = 31;

    while ( !(usedPoints.back().isEqual(goal))) {

        double min_distance = numeric_limits<double>::max();
        POINT near_point = POINT{-1,-1};

        for (const POINT &p : usedPoints) {
            if (p.x + 1 < height && grid[p.x + 1][p.y] == 0) {
                establish_nearest(POINT{p.x+1,p.y}, goal, &min_distance, &near_point);
            };
            if (p.y + 1 < width && grid[p.x][p.y + 1] == 0) {
                establish_nearest(POINT{p.x,p.y+1}, goal, &min_distance, &near_point);
            }
            if (p.x - 1 >= 0 && grid[p.x - 1][p.y] == 0) {
                establish_nearest(POINT{p.x-1,p.y}, goal, &min_distance, &near_point);
            }
            if (p.y - 1 >= 0 && grid[p.x][p.y - 1] == 0) {
                establish_nearest(POINT{p.x,p.y-1}, goal, &min_distance, &near_point);
            }
        }

        if (near_point.isEqual(POINT{-1,-1})) {
            return list<POINT>{POINT{-1,-1}}; //Impossible to solve indicator
        }
        usedPoints.push_back(near_point);
        grid[near_point.x][near_point.y] = 31;
    }


    return usedPoints;
};

bool sortPointsByX(POINT p1, POINT p2) {
    return p1.x > p2.x;
}

template <int height, int width>
list<POINT> place_obstacles(unsigned char grid[height][width], double rate) {
    int obstacles_count = static_cast<int>(height * width * rate);
    list<POINT> obstacles;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> rand_height(0.0, height);
    uniform_real_distribution<double> rand_width(0.0, width);

    for (int i = 0; i < obstacles_count; i++) {
        int x = static_cast<int>(rand_height(gen));
        int y = static_cast<int>(rand_width(gen));
        if (grid[x][y] == 255) {
            i--;
            continue;
        }
        grid[x][y] = 255;
        obstacles.push_back(POINT{x, y});
    }
    obstacles.sort(sortPointsByX);
    return obstacles;
}

template <int height, int width>
void print_grid(unsigned char grid[height][width]) {
    char block = char(178);

    for (int i = height-1; i >= 0; --i) {
        cout << i%10 << " |";
        for (int j = 0; j < width; j++) {
            cout << (grid[i][j] < 1 ? ' ' : (grid[i][j] == 255 ? block : '*' )) << "|";
        }

        cout << endl;
    }
    cout << "   0 ";
    for (int i = 1; i < width; i++) {
        cout << i%10 << " ";
    }
    cout << endl;
}

//TODO: Create conversion to alternative map expression (nodes)
// struct cell {
//     double distance;
//     list<struct cell*> exits;
// };
// template <int height, int width>
// void convert_grid() {
// }

int main() {
    constexpr int height = 10, width = 30;
    unsigned char grid[height][width];
    fill_n(&grid[0][0], height*width, 0);
    place_obstacles<height,width>(grid, 0.2);

    print_grid<height,width>(grid);

    POINT start = POINT{0,0};
    POINT goal = POINT{6,15};

    list<POINT> possible_traces = A_star_on_grid<height, width>(grid, start, goal);
    if (possible_traces.back().isEqual(POINT{-1,-1})) {
        cout << "Unable to solve problem." << endl << endl;
        return 404;
    }

    print_grid<height,width>(grid);

}