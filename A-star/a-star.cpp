#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <ostream>
#include <random>
#include "Points.h"
#include "GridMap.cpp"



using namespace std;

void establish_nearest(POINT p, POINT goal, double *min_dist, POINT* near_point) {
    double distance = p.getDistance(goal);
    if (*min_dist >= distance) {
        *min_dist = distance;
        *near_point = p;
    }
}

void establish_next_move(const list<POINT>& used_points, const GridMap &grid, const POINT goal,
    const unsigned char free_space_value, double *min_dist, POINT* near_point, void (*establishing_func)(POINT,POINT,double*, POINT*)) {

    long index = 0;
    for (const POINT &p : used_points) {
        if (p.x + 1 < grid.getHeight() && grid[p.x + 1][p.y] == free_space_value) {
            establishing_func(POINT{p.x+1,p.y}, goal, min_dist, near_point);
        };
        if (p.y + 1 < grid.getWidth() && grid[p.x][p.y + 1] == free_space_value) {
            establishing_func(POINT{p.x,p.y+1}, goal, min_dist, near_point);
        }
        if (p.x - 1 >= 0 && grid[p.x - 1][p.y] == free_space_value) {
            establishing_func(POINT{p.x-1,p.y}, goal, min_dist, near_point);
        }
        if (p.y - 1 >= 0 && grid[p.x][p.y - 1] == free_space_value) {
            establishing_func(POINT{p.x,p.y-1}, goal, min_dist, near_point);
        }
        index++;
    }
}


void print_grid(const GridMap &grid) {
    constexpr char block = static_cast<char>(178);

    for (long i = grid.getHeight()-1; i >= 0; --i) {
        cout << i%10 << "|";
        for (int j = 0; j < grid.getWidth(); j++) {
            auto point = grid[i][j];
            cout << (point < 1 ? ' ' : (point == 255 ? block : '*' )) << " ";
        }

        cout << endl;
    }
    cout << "   ";
    for (int i = 0; i < grid.getWidth(); i++) {
        const int num = i%10;
        cout << (num != 0 ? to_string(num) : " ") << " ";
    }
    cout << endl << "   ";
    for (int i = 0; i < grid.getWidth()/10; i++) {
        cout << std::left << std::setw(20) << i*10 ;
    }
    cout << endl;
}

void print_grid(const GridMap &grid, const POINT goal, const POINT start) {
    constexpr char block = static_cast<char>(178);

    for (long i = grid.getHeight()-1; i >= 0; --i) {
        cout << i%10 << " |";
        for (int j = 0; j < grid.getWidth(); j++) {
            auto point = grid[i][j];
            if (((goal.x == i && goal.y == j) || (start.x == i && start.y == j))) {
                cout <<( point == 255 ? "#":"$") << " ";
                continue;
            }


            cout << (point < 1 ? ' ' : (point == 255 ? block : '*' )) << " ";
        }

        cout << endl;
    }
    cout << "   ";
    for (int i = 0; i < grid.getWidth(); i++) {
        const int num = i%10;
        cout << (num != 0 ? to_string(num) : " ") << " ";
    }
    cout << endl << "   ";
    for (int i = 0; i < grid.getWidth()/10; i++) {
        cout << std::left << std::setw(20) << i*10 ;
    }
    cout << endl;
}

void clear_traces(const GridMap &grid, unsigned char free_space_value, unsigned char trace_value) {
    for (int h = 0; h < grid.getHeight(); ++h) {
        for (int w = 0; w < grid.getWidth(); ++w) {
            if (grid[h][w] == trace_value) {
                grid[h][w] = free_space_value;
            }
        }
    }
}

list<POINT> A_star_on_grid(const GridMap &grid, const POINT start, const POINT goal,
    const unsigned char free_space_value = 0, const unsigned char trace_value = 31) {
    if (start.isEqual(goal))
        return list<POINT>{goal};

    list<POINT> forwardUsedPoints = list<POINT>();
    forwardUsedPoints.push_back(start);
    grid[start.x][start.y] = trace_value;

    while ( !(forwardUsedPoints.front().isEqual(goal))) {

        POINT near_point = POINT{-1,-1};

        establish_next_move(forwardUsedPoints, grid, goal, free_space_value, &near_point, establish_nearest);

        if (near_point.isEqual(POINT{-1,-1})) {
            return list<POINT>{POINT{-1,-1}}; //Impossible to solve indicator
        }
        forwardUsedPoints.push_front(near_point);
        grid[near_point.x][near_point.y] = trace_value;
    }

    list<POINT> trace_list = list<POINT>{goal};
    grid[goal.x][goal.y] = trace_value;
    while ( !(trace_list.front().isEqual(start))) {
        
        auto next_point = POINT{-1,-1};

        establish_next_move(trace_list, grid, start, trace_value, &next_point , establish_nearest);

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

void speed_test_random_obstacles(long height, long width, int obstacles_reshuffles = 1, int repeats = 10,
                                 double obstacles_rate = 0.15, const std::string& save_file_name = "speedTestRandomObstacles.txt") {

    std::ofstream sf = std::ofstream(save_file_name, ios::app);
    if (!sf.is_open()) {
        std::cerr << "Unable to open file" << std::endl;
        throw std::runtime_error("Unable to open file");
    }

    list<POINT> trace;
    long trace_len = 0;

    long long avg_runTime = 0;
    int error_count = 0;

    POINT start = POINT{0,0}, goal = POINT{height-1,width-1};
    double line_dist = height+width;

    for (int i = 0; i < obstacles_reshuffles && error_count < 10; i++) {
        GridMap grid = GridMap(height, width, 0);



        do {
            grid._fill_grid_with(0);
            place_obstacles(grid, obstacles_rate);
        }while (grid[start.x][start.y] == 255 || grid[goal.x][goal.y] == 255);

        long long duration = 0;
        for (int j = 0; j < repeats; j++) {
            auto start_time = std::chrono::high_resolution_clock::now();

            trace = A_star_on_grid(grid, start, goal);

            auto end_time = std::chrono::high_resolution_clock::now();

            if (trace.front() == POINT{-1,-1}) {
                i--;
                error_count++;
                break;
            }
            clear_traces(grid, 0, 31);
            trace_len += trace.size()+1;

            auto t = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
            duration += t;
            cout << t << endl;
        }

        avg_runTime += duration;
    }
    if (error_count >= 10) {
        cout << height << "\t" << width << "\t" << obstacles_rate << "\t" << line_dist << "\t" << -1 << "\t" << -1 << endl;
    }
    else {
        sf << height << "\t" << width << "\t" << obstacles_rate << "\t" << line_dist << "\t" << static_cast<double>(trace_len)/repeats/obstacles_reshuffles << "\t" << avg_runTime/repeats/obstacles_reshuffles << endl;
        cout << height << "\t" << width << "\t" << obstacles_rate << "\t" << line_dist << "\t" << static_cast<double>(trace_len)/repeats/obstacles_reshuffles << "\t" << avg_runTime/repeats/obstacles_reshuffles << endl;
    }
    sf.close();
}

void runTest1() { // Varying only size
    std::ofstream sf = std::ofstream("speedTestRandomObstacles.txt");
    if (!sf.is_open()) {
        std::cerr << "Unable to open file" << std::endl;
        throw std::runtime_error("Unable to open file");
    }
    sf << "Height\tWidth\tObstacles\tDistance_In_Line\tCalculated_Distance\tTime[us]" << std::endl;
    sf.close();

    for (long size = 50; size <= 1500; size+=25) {
        speed_test_random_obstacles(size,size,  1, 10, 0.1);
    }
}

void runTest2() { // Varying amount of obstacles in different sizes
    ofstream sf = std::ofstream("speedTestObstaclesRate.txt");
    if (!sf.is_open()) {
        std::cerr << "Unable to open file" << std::endl;
        throw std::runtime_error("Unable to open file");
    }
    sf << "Height\tWidth\tObstacles\tDistance_In_Line\tCalculated_Distance\tTime[us]" << std::endl;
    sf.close();

    for (long size = 50; size < 1000; size+=50) {
        for (int i = 0; i <= 30; i++) {
            double obr = 0.1 + i * 0.01;
            speed_test_random_obstacles(size, size, 1, 10, obr, "speedTestObstaclesRate.txt");
        }
    }
}

int manualRun() {
    const int height = 10;
    const int width = 80;

    GridMap grid = GridMap(height,width, 0);

    place_obstacles(grid, 0.15);
    print_grid(grid);

    //  const POINT start = get_point_from_user(grid, "Starting point");
    //  const POINT goal = get_point_from_user(grid, "Goal point");

    constexpr auto start = POINT{0,0};
    const auto goal = POINT{height - 1,width - 1};

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

    return 1;
}


int main() {

    //Choose an option
    runTest1();
    // runTest2();
    // manualRun();

    return 1;

}

