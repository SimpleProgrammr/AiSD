#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <random>
#include "file_cleaner.h"

#if _WIN32
#include <chrono>

int gettimeofday(struct timeval* tp, struct timezone* tzp) {
    namespace sc = std::chrono;
    sc::system_clock::duration d = sc::system_clock::now().time_since_epoch();
    sc::seconds s = sc::duration_cast<sc::seconds>(d);
    tp->tv_sec = s.count();
    tp->tv_usec = sc::duration_cast<sc::microseconds>(d - s).count();

    return 0;
}
#elif defined(__linux__) || defined(__unix__)  || defined(__MACH__)
    #include <sys/time.h>
#endif
#define timersub(tvp,uvp,vvp) do {(vvp)->tv_sec = (tvp)->tv_sec - (uvp)->tv_sec; (vvp)->tv_usec = (tvp)->tv_usec - (uvp)->tv_usec; if ((vvp)->tv_usec < 0) {(vvp)->tv_sec--;(vvp)->tv_usec += 1000000;}} while (0)

using namespace std;

constexpr long max_N = 1000;
const double a = 0;
const double b = 2.5;
const double bottom = 0;
const double top = 2.5;
constexpr long double predicted_value = 3.1099; //Input from your calculator (for example wolframalpha.com)!!!

//Tu zapisz swoją funkcję :)
double FUNCTION_TO_TEST(double x) {
    return pow(pow((x-1),3)-sqrt(3*x+1),4)/10;
}

std::list<std::pair<double, double>> points = {};

double left_rectangles_integral(double a, double b, long double parts, const function<double(double x)> &func) {

    double surface = 0;

    double jump = abs(a-b)/parts;
    while (a<b) {

        double h = func(a);
        surface += h*jump;
        a+=jump;
    }

    return surface;
}

double right_rectangles_integral(double a, double b, long double parts, const function<double(double x)> &func) {

    double surface = 0;

    double jump = abs(a-b)/parts;
    while (a<b) {

        double h = func(b);
        surface += h*jump;
        b-=jump;
    }

    return surface;
}

double mid_rectangles_integral(double a, double b, long double parts, const function<double(double x)> &func) {

    double surface = 0;

    double jump = abs(a-b)/parts;
    while (a<b) {

        double h = func(a+jump/2);
        surface += h*jump;
        a+=jump;
    }

    return surface;
}

double trapezoids_integral(double a, double b, long double parts, const function<double(double x)> &func) {

    double surface = 0;

    double jump = abs(a-b)/parts;
    while (a<b) {

        double h1 = func(a);
        double h2 = func(a+=jump);
        surface += (h1+h2)*jump/2;
    }

    return surface;
}

double monte_carlo_integral(double a, double b, double bottom, double top, long tries, const function<double(double x)> &func) {
    double hits = 0;

    uniform_real_distribution<double> randX(a,b);
    uniform_real_distribution<double> randY(bottom,top);
    default_random_engine re;

    for (int i=0; i<tries; i++) {
        double pX = randX(re);
        double pY = randY(re);

        points.emplace_back(pX,pY);
        if (abs(pY) <= abs(func(pX))) {
            hits++;
        }
    }

    return abs(top-bottom)*abs(a-b)*hits/static_cast<long double>(tries);
}

void raport_normal_integral(long N,
    double a,
    double b,
    const function<double(
        double a,
        double b,
        long parts,
        const function<double(double x)> &f)> &func,
    ofstream &SAVE) {
    timeval diff{}, startTV{}, endTV{};

    gettimeofday(&startTV, nullptr);

    double surface = func(a, b, N, FUNCTION_TO_TEST);

    gettimeofday(&endTV, nullptr);

    timersub(&endTV, &startTV, &diff);

    const long double time_val_ms = diff.tv_usec / 1000.0L + static_cast<long double>(diff.tv_sec) * 1000L;

    SAVE << setprecision(10) << N << "\t"
        << surface << "\t"
        << predicted_value << "\t"
        << (surface - predicted_value) << "\t"
        << time_val_ms << endl;
}



void raport_probabilistic_integral(long N,
    double a,
    double b,
    double bottom,
    double top,
    const function<double(
        double a,
        double b,
        double bottom,
        double top,
        long tries,
        const function<double(double x)> &f)> &func,
    ofstream &SAVE){

    points.clear();

    timeval diff{}, startTV{}, endTV{};

    gettimeofday(&startTV, nullptr);

    double surface = func(a, b,bottom,top, N, FUNCTION_TO_TEST);

    gettimeofday(&endTV, nullptr);

    timersub(&endTV, &startTV, &diff);

    const long double time_val_ms = diff.tv_usec / 1000.0L + static_cast<long double>(diff.tv_sec) * 1000L;

    SAVE << setprecision(10) << N << "\t"
        << surface << "\t"
        << predicted_value << "\t"
        << (surface - predicted_value) << "\t"
        << time_val_ms << endl;
}

void save_points_from_monte_carlo() {
    ofstream POINTS("POINTS.txt");
    if (!POINTS.is_open()) {
        throw runtime_error("Points file not open");
    }

    POINTS << "X\tY" << endl;
    for (const auto &p : points) {
        POINTS << setprecision(10) << p.first << "\t" << p.second << endl;
    }
    POINTS.close();
}

int main() {

    // Otwieramy pliki poza sekcją równoległą
    ofstream LEFT("LEFT.txt");
    ofstream RIGHT("RIGHT.txt");
    ofstream MID("MID.txt");
    ofstream TRAP("TRAP.txt");
    ofstream MONTE("MONTE.txt");

    if (!LEFT || !RIGHT || !MID || !TRAP || !MONTE) {
        cerr << "Błąd otwierania plików!" << std::endl;
        return 1;
    }
    string header = "N\tCalculated Value\tPredicted Value\tDifference\tTime[ms]\n";
    LEFT << header;
    RIGHT << header;
    MID << header;
    TRAP << header;
    MONTE << header;

    timeval diff{}, startTV{}, endTV{};
    gettimeofday(&startTV, nullptr);

    for (long N = 1; N <= max_N; N += 1) {

#pragma omp parallel
        {
#pragma omp sections
            {
#pragma omp section
                raport_normal_integral(N, a, b, left_rectangles_integral, LEFT);

#pragma omp section
                raport_normal_integral(N, a, b, right_rectangles_integral, RIGHT);

#pragma omp section
                raport_normal_integral(N, a, b, mid_rectangles_integral, MID);

#pragma omp section
                raport_normal_integral(N, a, b, trapezoids_integral, TRAP);
            }
        }
    }


    for (long N = 1; N <= max_N; N += 1) {
        raport_probabilistic_integral(N,a,b,bottom,top,monte_carlo_integral,MONTE);
    }
    save_points_from_monte_carlo();
#pragma omp barrier
    gettimeofday(&endTV, nullptr);
    timersub(&endTV, &startTV, &diff);

    cout << "Calculations time: " << diff.tv_usec/1000.0L+ static_cast<long double>(diff.tv_sec) * 1000L << "ms\n\n" << endl;

    // Zamknięcie plików

    LEFT.close();
    RIGHT.close();
    MID.close();
    TRAP.close();
    MONTE.close();


    file_cleaner::replace_all("LEFT.txt", '.', ',');
    file_cleaner::replace_all("RIGHT.txt", '.', ',');
    file_cleaner::replace_all("MID.txt", '.', ',');
    file_cleaner::replace_all("TRAP.txt", '.', ',');
    file_cleaner::replace_all("MONTE.txt", '.', ',');

    return 0;
}

