#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>

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

//Tu zapisz swoją funkcję :)
double FUNCTION_TO_TEST(double x) {
    return 1-x;
}

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

    SAVE << setprecision(10) << N << "\t" << surface<<"\t"<< diff.tv_usec/1000.0L+ static_cast<long double>(diff.tv_sec) * 1000L << endl;
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
    timeval diff{}, startTV{}, endTV{};

    gettimeofday(&startTV, nullptr);

    double surface = func(a, b,bottom,top, N, FUNCTION_TO_TEST);

    gettimeofday(&endTV, nullptr);

    timersub(&endTV, &startTV, &diff);

    SAVE << setprecision(10) << N << "\t" << surface<<"\t"<< diff.tv_usec/1000.0L+ static_cast<long double>(diff.tv_sec) * 1000L << endl;
}

int main() {
    constexpr long max_N = 10000;
    const double a = 0;
    const double b = 1;

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

    LEFT << "N\tCalculated Value\tTime"<<endl;
    RIGHT << "N\tCalculated Value\tTime"<<endl;
    MID << "N\tCalculated Value\tTime"<<endl;
    TRAP << "N\tCalculated Value\tTime"<<endl;
    MONTE << "N\tCalculated Value\tTime"<<endl;

    timeval diff{}, startTV{}, endTV{};
    gettimeofday(&startTV, nullptr);

    for (long N = 1; N < max_N; N += 1) {

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

#pragma omp section
                raport_probabilistic_integral(N, a, b, 0, 2, monte_carlo_integral, MONTE);
            }
        }
    }
    gettimeofday(&endTV, nullptr);
    timersub(&endTV, &startTV, &diff);

    cout << "Calculations time: " << diff.tv_usec/1000.0L+ static_cast<long double>(diff.tv_sec) * 1000L << "ms\n\n" << endl;

    // Zamknięcie plików
    LEFT.close();
    RIGHT.close();
    MID.close();
    TRAP.close();
    MONTE.close();

    return 0;
}

