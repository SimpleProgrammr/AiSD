#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <list>
#include <ostream>
#include <random>
#include "GridMap.cpp"

using namespace std;

typedef struct point {
    int x;
    int y;
    [[nodiscard]] bool isEqual(struct point p) const {
        return x == p.x && y == p.y;
    }
    [[nodiscard]] double getDistance(struct point p)const {
        return sqrt( pow( x - p.x, 2 ) + pow(y - p.y, 2 ) );
    }
} POINT;

bool sortPointsByX(POINT p1, POINT p2) {
    return p1.x > p2.x;
}

void establish_nearest(POINT p, POINT goal, double *min_dist, POINT* near_point) {
    double distance = p.getDistance(goal);
    if (*min_dist >= distance) {
        *min_dist = distance;
        *near_point = p;
    }
}

void establish_next_move(const list<POINT>& used_points,GridMap grid,  POINT goal, unsigned char free_space_value, double *min_dist, POINT* near_point) {
    for (const POINT &p : used_points) {
        if (p.x + 1 < grid.getHeight() && grid[p.x + 1][p.y] == free_space_value) {
            establish_nearest(POINT{p.x+1,p.y}, goal, min_dist, near_point);
        };
        if (p.y + 1 < grid.getWidth() && grid[p.x][p.y + 1] == free_space_value) {
            establish_nearest(POINT{p.x,p.y+1}, goal, min_dist, near_point);
        }
        if (p.x - 1 >= 0 && grid[p.x - 1][p.y] == free_space_value) {
            establish_nearest(POINT{p.x-1,p.y}, goal, min_dist, near_point);
        }
        if (p.y - 1 >= 0 && grid[p.x][p.y - 1] == free_space_value) {
            establish_nearest(POINT{p.x,p.y-1}, goal, min_dist, near_point);
        }
    }
}


void print_grid(GridMap grid) {
    char block = char(178);

    for (int i = grid.getHeight()-1; i >= 0; --i) {
        cout << i%10 << " |";
        for (int j = 0; j < grid.getWidth(); j++) {
            auto point = grid[i][j];
            cout << (point < 1 ? ' ' : (point == 255 ? block : '*' )) << "|";
        }

        cout << endl;
    }
    cout << "   0 ";
    for (int i = 1; i < grid.getWidth(); i++) {
        cout << i%10 << " ";
    }
    cout << endl;
}

void clear_traces(GridMap grid, unsigned char free_space_value, unsigned char trace_value) {
    for (int h = 0; h < grid.getHeight(); ++h) {
        for (int w = 0; w < grid.getWidth(); ++w) {
            if (grid[h][w] == trace_value) {
                grid[h][w] = free_space_value;
            }
        }
    }
}

void print_grid(GridMap grid, POINT goal, POINT start) {
    char block = char(178);

    for (int i = grid.getHeight()-1; i >= 0; --i) {
        cout << i%10 << " |";
        for (int j = 0; j < grid.getWidth(); j++) {
            auto point = grid[i][j];
            if (((goal.x == i && goal.y == j) || (start.x == i && start.y == j))) {
                cout <<( point == 255 ? "#":"$") << "|";
                continue;
            }


            cout << (point < 1 ? ' ' : (point == 255 ? block : '*' )) << "|";
        }

        cout << endl;
    }
    cout << "   0 ";
    for (int i = 1; i < grid.getWidth(); i++) {
        cout << i%10 << " ";
    }
    cout << endl;
}

list<POINT> A_star_on_grid(const GridMap &grid, const POINT start, const POINT goal,
    const unsigned char free_space_value = 0, const unsigned char trace_value = 31) {
    if (start.isEqual(goal))
        return list<POINT>{goal};

    list<POINT> forwardUsedPoints = list<POINT>();
    forwardUsedPoints.push_back(start);
    grid[start.x][start.y] = trace_value;

    while ( !(forwardUsedPoints.front().isEqual(goal))) {

        double min_distance = numeric_limits<double>::max();
        POINT near_point = POINT{-1,-1};

        establish_next_move(forwardUsedPoints, grid, goal, free_space_value, &min_distance, &near_point, establish_nearest);

        if (near_point.isEqual(POINT{-1,-1})) {
            return list<POINT>{POINT{-1,-1}}; //Impossible to solve indicator
        }
        forwardUsedPoints.push_front(near_point);
        grid[near_point.x][near_point.y] = trace_value;
    }

    list<POINT> trace_list = list<POINT>{goal};
    grid[goal.x][goal.y] = trace_value;
    while ( !(trace_list.front().isEqual(start))) {

        double min_distance = numeric_limits<double>::max();
        auto next_point = POINT{-1,-1};

        establish_next_move(trace_list, grid, start, trace_value, &min_distance, &next_point , establish_nearest);

        if (next_point.isEqual(POINT{-1,-1})) {
            return list<POINT>{POINT{-1,-1}}; //Impossible to solve indicator
        }
        trace_list.push_front(next_point);
        grid[next_point.x][next_point.y] = free_space_value;
    }
    clear_traces(grid, free_space_value, trace_value);
    for (const POINT &p : trace_list) {
        grid[p.x][p.y] = trace_value;
    }

    return trace_list;

};



list<POINT> place_obstacles(const GridMap &grid, const double rate) {
    const int obstacles_count = static_cast<int>(static_cast<double>(grid.getHeight()) * static_cast<double>(grid.getWidth()) * rate);
    list<POINT> obstacles;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> rand_height(0.0, static_cast<double>(grid.getHeight()));
    uniform_real_distribution<double> rand_width(0.0, static_cast<double>(grid.getWidth()));

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


POINT get_point_from_user(const GridMap &grid, const string& purpose = "") {

    while (true) {
        cout << purpose<<endl;
        POINT point;
        cout << "Enter x-coordinates: ";
        cin >> point.x;
        cout << "Enter y-coordinates: ";
        cin >> point.y;

        if (grid[point.x][point.y] == 255) {
            cout << "[" << point.x << ", " << point.y << "] - is an obstacle. Try again." << endl;
        }
        else if (point.x >= grid.getWidth() && point.y >= grid.getHeight()) {
            cout << "[" << point.x << ", " << point.y << "] - is out of bounds. Try again." << endl;
        }
        else
            return point;
    }
}

void speed_test(int height, int width, int repeats, int obstacles_reshuffles) ;

int main() {
    const int height = 10;
    const int width = 80;

    GridMap grid = GridMap(height,width, 0);

    place_obstacles(grid, 0.15);

     const POINT start = get_point_from_user(grid, "Starting point");
     const POINT goal = get_point_from_user(grid, "Goal point");
    //
    // constexpr auto start = POINT{0,0};
    // const auto goal = POINT{height - 1,width - 1};

    print_grid(grid,goal,start);

    if (grid[start.x][start.y] == 255 || grid[goal.x][goal.y] == 255) {
        cerr << "Unable to access those points"<<endl;
        return 404;
    }


    list<POINT> possible_traces = A_star_on_grid(grid, start, goal);
    if (possible_traces.back().isEqual(POINT{-1,-1})) {
        cout << "Unable to solve problem." << endl << endl;
        return 404;
    }

    print_grid(grid, goal, start);

}


// void speed_test(int height, int width, int repeats = 10, int obstacles_reshuffles = 1, double obstacles_rate = 0.15) {
//     fill_n(&grid[0][0], height*width, 0);
//
//     int run_counter = 0;
//     long double time_sum = 0;
//
//     for (int ob_r = 0; ob_r < obstacles_reshuffles; ob_r++ ) {
//         place_obstacles<height,width>(grid, obstacles_rate);
//     }
//
// }