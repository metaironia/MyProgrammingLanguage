#include <math.h>

#include "math_operation.h"

bool IsDoubleEqual       (const double first_number, const double second_number) {

    return (fabs (first_number - second_number) < EPS);
}

bool IsDoubleLessOrEqual (const double first_number, const double second_number) {

    return (fabs (first_number - second_number) < EPS || (first_number - second_number) <= -EPS);
}

bool IsDoubleLess        (const double first_number, const double second_number) {

    return (first_number - second_number) < -EPS;
}
