#include "histogram.h"

#include <cassert>
#include "svg.h"
void
test_positive() {
    double min = 0;
    double max = 0;
    find_minmax({1, 2, 3}, min, max);
    assert(min == 1);
    assert(max == 3);
}
void
test_max()
{
 double max = 0;
 find_max({2,5,3})
 assert (max == 5);
}

int

main() {
    test_positive();
    test_max();
}
