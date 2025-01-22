#include <stdio.h>
#include <math.h>
#include <conio.h>

#define EPSILON 1e-6
#define LOWER_BOUND (-100)
#define UPPER_BOUND 100
#define ESC 27
#define EQUATION_TRIGONOMETRY 1
#define EQUATION_LOGARITHM 2

#define METHOD_BISECTION 1
#define METHOD_NEWTON 2


double trigonometry(double x, double y) {
    if (fabs(x) < EPSILON) {
        printf("Error: Equation 1 is not defined when x approaches 0.\n");
        return NAN;
    }
    return cos(y / x) - 2.0 * sin(1.0 / x) + (1.0 / x);
}

double logarithm(double x, double y) {
    return sin(log(x)) - cos(log(x)) + y * log(x);
}

double derivative_trigonometry(double x, double y) {
    double epsilon = 1e-6;
    return (trigonometry(x + epsilon, y) - trigonometry(x, y)) / epsilon;
}

double derivative_logarithm(double x, double y) {
    double epsilon = 1e-6;
    return (logarithm(x + epsilon, y) - logarithm(x, y)) / epsilon;
}

double bisection_solve(double (*func)(double, double), double a, double b, double y) {
    double c;
    while (fabs(b - a) > EPSILON) {
        c = (a + b) / 2.0;
        if (func(a, y) * func(c, y) > 0) {
            a = c;
        } else {
            b = c;
        }
    }
    return (a + b) / 2.0;
}

double newton_solve(double (*func)(double, double), double x, double y, int equationChoice) {
    double delta;
    do {
        if (equationChoice == 1) {
            delta = func(x, y) / derivative_trigonometry(x, y);
        } else {
            delta = func(x, y) / derivative_logarithm(x, y);
        }
        x = x - delta;
    } while (fabs(delta) > EPSILON);
    return x;
}

bool get_input(double* a, double* b, double* y, int* equationChoice, int* methodChoice) {
    bool inputValid = false;

    do {
        printf("Choose an equation (1 or 2): ");
        if (scanf_s("%d", equationChoice) != 1 || (*equationChoice != 1 && *equationChoice != 2)) {
            printf("Error: Invalid input. Please enter 1 or 2.\n");
        } else {
            inputValid = true;
        }
    } while (!inputValid);
    fflush(stdin);

    inputValid = false;
    do {
        printf("Choose a method (1 - Bisection, 2 - Newton): ");
        if (scanf_s("%d", methodChoice) != 1 || (*methodChoice != 1 && *methodChoice != 2)) {
            printf("Error: Invalid input. Please enter 1 or 2.\n");
        } else {
            inputValid = true;
        }
    } while (!inputValid);
    fflush(stdin);

    inputValid = false;
    do {
        printf("Enter the interval [a, b] separated by a space [%d, %d]: ", LOWER_BOUND, UPPER_BOUND);
        if (scanf_s("%lf %lf", a, b) != 2 || *a < LOWER_BOUND || *a > UPPER_BOUND || *b < LOWER_BOUND ||
            *b > UPPER_BOUND || *a >= *b) {
            printf("Error: Invalid input. Please enter a valid interval [%d, %d] with a < b.\n", LOWER_BOUND,
                   UPPER_BOUND);
        } else {
            inputValid = true;
        }
    } while (!inputValid);
    fflush(stdin);

    inputValid = false;
    do {
        printf("Enter the value of y in the range (-10, 10) excluding 0: ");
        if (scanf_s("%lf", y) != 1 || *y <= -10 || *y >= 10 || *y == 0) {
            printf("Error: Invalid input. Please enter a valid value for y in the range (-10, 0) or (0, 10).\n");
        } else {
            inputValid = true;
        }
    } while (!inputValid);
    fflush(stdin);

    return true;
}

int main() {
    char choice;
    double a, b, y, result;
    int equationChoice, methodChoice;
    printf("Hello! This program will help you solve equations using the bisection method and Newton's method. \n"
           "1. cos(y/2) - 2*sin(1/x) + (1/x) = 0, [a; b]\n"
           "2. sin(lnx) - cos(lnx) + y * 1nx = 0, [a; b]\n");

    do {
        if (!get_input(&a, &b, &y, &equationChoice, &methodChoice)) {
            continue;
        }

        switch (equationChoice) {
            case EQUATION_TRIGONOMETRY:
                if (methodChoice == METHOD_BISECTION)
                    result = bisection_solve(trigonometry, a, b, y);
                else if (methodChoice == METHOD_NEWTON)
                    result = newton_solve(trigonometry, (a + b) / 2, y, equationChoice);
                else {
                    printf("Invalid method choice\n");
                    continue;
                }
                break;
            case EQUATION_LOGARITHM:
                if (methodChoice == METHOD_BISECTION)
                    result = bisection_solve(logarithm, a, b, y);
                else if (methodChoice == METHOD_NEWTON)
                    result = newton_solve(logarithm, (a + b) / 2, y, equationChoice);
                else {
                    printf("Invalid method choice\n");
                    continue;
                }
                break;
            default:
                printf("Invalid equation choice\n");
                continue;
        }
        if (!isnan(result)) {
            printf("Result: %lf\n", result);
        }
        printf("\nPress Esc to exit or any other key to continue\n");
    } while (_getch()!= ESC);
    return 0;
}
