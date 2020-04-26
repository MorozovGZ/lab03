#ifndef SVG_H_INCLUDED
#define SVG_H_INCLUDED

#include <vector>
#include <string>
using namespace std;

void svg_begin(double width, double height);
void svg_end();
void svg_text(double left, double baseline, size_t text);
void svg_rect(double x, double y, double width, double height, string stroke, string fill );
void show_histogram_svg(const vector<size_t>& bins);


#endif // SVG_H_INCLUDED
