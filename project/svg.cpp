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

void svg_rect(double x, double y, double width, double height, string stroke, string fill)
{
    cout << "<rect x='" << x << "' y='" << y << "' width='" << width << "' height='" << height << "' stroke='" << stroke << "' fill='" << fill << "' />";
}



void show_histogram_svg(const vector<size_t>& bins)
{
    const auto TEXT_HEIGHT = 75;
    double TEXT_WIDTH = 10;
    const auto IMAGE_WIDTH = 400;
    const auto IMAGE_HEIGHT = 500;
    const auto TEXT_LEFT = 10;
    const auto TEXT_BASELINE = 450;
    const auto CONST_AUTO = 20;
    const auto BIN_HEIGHT = 30;
    const auto BLOCK_WIDTH = 10;
    const size_t MAX_ASTERISK = IMAGE_WIDTH - TEXT_LEFT - TEXT_WIDTH;
    const auto BAR_HEIGHT = IMAGE_HEIGHT - TEXT_HEIGHT;

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

     double scaling_factor = (double)MAX_ASTERISK / (max_count * BLOCK_WIDTH);

    for (size_t bin : bins)
    {


        scaling_factor = (double)bin / (double) max_count;
        size_t height = scaling_factor * BAR_HEIGHT;
       double y = BAR_HEIGHT - height;



        const double bin_width = BLOCK_WIDTH * height;
        svg_text(top + TEXT_LEFT,TEXT_BASELINE, bin);

        svg_rect(TEXT_WIDTH, y, BIN_HEIGHT, height,"red","#aab5ff");
        TEXT_WIDTH += BIN_HEIGHT;
        top += BIN_HEIGHT;

    }

    svg_end();
}
