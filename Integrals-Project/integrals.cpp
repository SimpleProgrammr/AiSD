#include <complex>
#include <fstream>
#include <random>
#include <sys/time.h>

using namespace std;

double easy_fun(double x) {
    return exp(-x);
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

    return (abs(top-bottom)*abs(a-b))*hits/static_cast<long double>(tries);
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

    double surface = func(a, b, N, easy_fun);

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

    double surface = func(a, b,bottom,top, N, easy_fun);

    gettimeofday(&endTV, nullptr);

    timersub(&endTV, &startTV, &diff);

    SAVE << setprecision(10) << N << "\t" << surface<<"\t"<< diff.tv_usec/1000.0L+ static_cast<long double>(diff.tv_sec) * 1000L << endl;
}

int main() {
    ofstream LEFT("LEFT.txt"), RIGHT("RIGHT.txt"), MID("MID.txt"), TRAP ("TRAP.txt"), MONTE("MONTE.txt");

            double a=0;
            double b=1;
#pragma omp parallel for nowait
    for (long N = 1; N < 100000; N+=1) {

#pragma omp sections
        {
#pragma omp section
            raport_normal_integral(N, a, b, left_rectangles_integral, (LEFT));
#pragma omp section
            raport_normal_integral(N, a, b, right_rectangles_integral, (RIGHT));
#pragma omp section
            raport_normal_integral(N, a, b, mid_rectangles_integral, (MID));
#pragma omp section
            raport_normal_integral(N, a, b, trapezoids_integral, (TRAP));
#pragma omp section
            raport_probabilistic_integral(N,a,b,0,2,monte_carlo_integral, (MONTE));
        }
    }
#pragma omp barrier

    LEFT.close();
    RIGHT.close();
    MID.close();
    TRAP.close();
    MONTE.close();
}

