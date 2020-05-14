#include "histogram.h"

#include <iostream>
#include <vector>
#include <conio.h>
#include <curl/curl.h>

using namespace std;





vector<double> input_numbers(istream& in, size_t count)
{
    vector <double> result(count);
    for (int i=0; i<count; i++)
    {
        in >> result[i];
    }

    return result;

}

Input read_input(istream& in, bool prompt)
{
    Input data;
    if ( prompt )
    {
        cerr << "Enter number count: ";
    }
    size_t number_count;
    in >> number_count;
    if ( prompt )
    {
        cerr << "Enter numbers: ";
    }
    data.numbers = input_numbers(in, number_count);

    if ( prompt )
    {
        cerr << "Enter column count: ";
    }
    in >> data.bin_count;


    return data;
}




void show_histogram_text(vector<size_t> bins)
{
    const size_t SCREEN_WIDTH = 80;
    const size_t MAX_ASTERISK = SCREEN_WIDTH - 4 - 1;

    size_t max_count = 0;
    for (size_t count : bins)
    {
        if (count > max_count)
        {
            max_count = count;
        }
    }
    const bool scaling_needed = max_count > MAX_ASTERISK;

    for (size_t bin : bins)
    {
        if (bin < 100)
        {
            cout << ' ';
        }
        if (bin < 10)
        {
            cout << ' ';
        }
        cout << bin << "|";

        size_t height = bin;
        if (scaling_needed)
        {
            const double scaling_factor = (double)MAX_ASTERISK / max_count;
            height = (size_t)(bin * scaling_factor);
        }

        for (size_t i = 0; i < height; i++)
        {
            cout << '*';
        }
        cout << '\n';
    }

}
void
svg_begin(double width, double height)
{
    cout << "<?xml version='1.0' encoding='UTF-8'?>\n";
    cout << "<svg ";
    cout << "width='" << width << "' ";
    cout << "height='" << height << "' ";
    cout << "viewBox='0 0 " << width << " " << height << "' ";
    cout << "xmlns='http://www.w3.org/2000/svg'>\n";
}

void
svg_end()
{
    cout << "</svg>\n";
}
void svg_text(double left,double baseline, string text)
{
    cout<< "<text x='" << left << "' y='" << baseline << "'>'"<< text << "'</text>";
}

void svg_rect(double x, double y, double width, double height, string stroke = "black", string fill = "black")
{
    cout<< "<rect x='" << x << "' y='"<< y << "' width='" << width << "' height='" << height << "' stroke='red' fill='blue'/>";
}
void
show_histogram_svg(const vector<size_t>& bins)
{
    svg_begin(400, 300);
    svg_text(20,20,to_string(bins[0]));
    const auto IMAGE_WIDTH = 400;
    const auto IMAGE_HEIGHT = 300;
    const auto TEXT_LEFT = 20;
    const auto TEXT_BASELINE = 20;
    const auto TEXT_WIDTH = 50;
    const auto BIN_HEIGHT = 30;
    const auto BLOCK_WIDTH = 10;
    double top = 0;
    for (size_t bin : bins)
    {
        const double bin_width = BLOCK_WIDTH * bin;
        svg_text(TEXT_LEFT, top + TEXT_BASELINE, to_string(bin));
        svg_rect(TEXT_WIDTH, top, bin_width, BIN_HEIGHT);
        top += BIN_HEIGHT;
    }
    svg_end();
}
void svg_text(double left, double baseline, string text);
void svg_rect(double x, double y, double width, double height,
              string stroke, string fill);





int main(int argc, char* argv[] )

{
    curl_global_init(CURL_GLOBAL_ALL);

    Input data=read_input(cin,true);

    const auto bins = make_histogram(data);

    show_histogram_svg(bins);
    if (argc>1)
    {
        CURL* curl = curl_easy_init();
        if(curl)
        {
            CURLcode res;
            curl_easy_setopt(curl, CURLOPT_URL, argv[1]);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }

        cout << " argc = " << argc << endl;
        for (int i = 0; i < argc; ++i)
        {

            cout << "argv[" << i << "] = " << argv[i] << " ";
        }
    }
    return 0;
}
