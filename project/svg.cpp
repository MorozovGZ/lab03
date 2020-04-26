#include "svg.h"
#include <vector>
#include <string>
#include <iostream>
using namespace std;
size_t find_max(const vector<size_t>& bins)
{
    size_t max = bins[0];
    for (const auto& bin : bins)
    {
        if (bin > max)
        {
            max = bin;
        }
    }
    return max;
}

void svg_begin(double width, double height)
{
    cout << "<?xml version='1.0' encoding='UTF-8'?>\n";
    cout << "<svg ";
    cout << "width='" << width << "' ";
    cout << "height='" << height << "' ";
    cout << "viewBox='0 0 " << width << " " << height << "' ";
    cout << "xmlns='http://www.w3.org/2000/svg'>\n";
}

void svg_end()
{
    cout << "</svg>\n";
}

void svg_text(double left, double baseline, size_t text)
{
    cout << "<text x='" << left << "' y='" << baseline << "'>" << text << "</text>";
}

void svg_rect(double x, double y, double width, double height, string stroke, string filll)
{
    cout << "<rect x='" << x << "' y='" << y << "' width='" << width << "' height='" << height << "' stroke='" << stroke << "' fill='" << fill << "' />";
}



void show_histogram_svg(const vector<size_t>& bins)
{
double TEXT_WIDTH = 0;
    const auto IMAGE_WIDTH = 400;
    const auto IMAGE_HEIGHT = 500;
    const auto TEXT_LEFT = 10;
    const auto TEXT_BASELINE = 120;
const auto CONST_AUTO = 20;
    const auto BIN_HEIGHT = 30;
    const auto BLOCK_WIDTH = 10;
    const size_t MAX_ASTERISK = IMAGE_WIDTH - TEXT_LEFT - TEXT_WIDTH;

    size_t max_count = 0;
    for (size_t count : bins)
    {
        if (count > max_count)
        {
            max_count = count;
        }
    }

    const bool scaling_needed = max_count * BLOCK_WIDTH > MAX_ASTERISK;


    svg_begin(IMAGE_WIDTH, IMAGE_HEIGHT);

double HEIGHT_MAXIMUM = find_max(bins);
double top = 0;

        for (size_t bin : bins)
    {

        size_t height = bin;
        if (scaling_needed)
        {
            const double scaling_factor = (double)MAX_ASTERISK / (max_count * BLOCK_WIDTH);
            height = (size_t)(bin * scaling_factor);
        }

        const double bin_width = BLOCK_WIDTH * height;
        svg_text(top + TEXT_LEFT,TEXT_BASELINE, bin);
        svg_rect(TEXT_WIDTH, CONST_AUTO*(HEIGHT_MAXIMUM - height), BIN_HEIGHT, height*CONST_AUTO,"red","#aab5ff");
        TEXT_WIDTH += BIN_HEIGHT;
        top += BIN_HEIGHT;

    }

    svg_end();
}
