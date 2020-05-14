#include "histogram.h"

#include <iostream>
#include <vector>
#include <conio.h>
#include <curl/curl.h>
#include <sstream>
#include <string>

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
size_t
write_data(void* items, size_t item_size, size_t item_count, void* ctx)
{
    int data_size=item_size * item_count;
    stringstream* buffer = reinterpret_cast<stringstream*>(ctx);
    cout << data_size << endl;
    cout << (const char*)items << endl;
    if ( buffer == nullptr )
    {
        cout << " buffer failed";

    }
    (* buffer ).write( (const char*)items, data_size);
    return 0;
}
Input download(const string& address)
{
    stringstream buffer;

    curl_global_init(CURL_GLOBAL_ALL);
    CURL* curl = curl_easy_init();
    if(curl)
    {
        CURLcode res;
        if ( CURLE_OK != curl_easy_setopt(curl, CURLOPT_URL, address))
        {
            cout << "error 4";

        }
            cerr << address << endl;

//        if ( CURLE_OK != curl_easy_setopt(curl, CURLOPT_URL, "http://uii.mpei.ru/study/courses/cs/lab03/marks.txt") )
 //       {
   //         cout << "error 1";

     //   }
        if ( CURLE_OK != curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer))
        {
            cout << "error 2";

        }

       if ( CURLE_OK != curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data))
        {
            cout << "error 3";

        }

        res = curl_easy_perform(curl);
        if ( res != CURLE_OK )
        {
            cerr << curl_easy_strerror( res ) << endl;
            exit(1);
        }
        curl_easy_cleanup(curl);
    }

    return read_input(buffer, false);
}


int main(int argc, char* argv[] )
{

    Input input;
    if (argc > 1)
    {
        input = download(argv[1]);
    }
    else
    {
        input = read_input(cin, true);
    }

    const auto bins = make_histogram(input);
    show_histogram_svg(bins);
    return 0;
}
