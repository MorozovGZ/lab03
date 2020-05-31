#include "histogram.h"



#include "histogram.cpp"
#include <windows.h>
#include<sstream>
#include<curl/curl.h>

#include <iostream>
#include <vector>
#include <conio.h>
#include <curl/curl.h>
#include <sstream>
#include <string>

using namespace std;








    string print_name ()
{
    stringstream buffer;

    DWORD info = GetVersion();
    DWORD mask = 0x0000ffff;
    DWORD version = info & mask;

    DWORD platform = info >> 16;
    DWORD mask_minor = 0x000000ff;
    DWORD mask_major = 0x0000ff00;
    DWORD version_minor = info & mask_minor;


    DWORD version_major1 = info & mask_major;
    DWORD version_major = version_major1>>8;


    if ((info & 0b10000000'00000000'0000000'00000000) == 0)
    {
        DWORD build = platform;
        buffer << "Windows v" << version_minor << "." << version_major << " (build" << build << ")\n";

    }
    char system [MAX_COMPUTERNAME_LENGTH + 1];
    DWORD Size =sizeof (system);
    GetComputerNameA(system, &Size);
    buffer << "Name: "<<system<<"\n";

    return buffer.str();
}
/*vector<double> input_numbers(const size_t count)

{
    vector <double> result(count);
    for (int i=0; i<count; i++)
    {
        in >> result[i];
    }

    return result;

}
*/
vector<double>
input_numbers(istream& in, size_t count)
{
    vector<double> result(count);
    for (size_t i = 0; i < count; i++)
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


        const auto IMAGE_WIDTH = 1000;

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

        cout << "<text x='" << TEXT_LEFT << "' y='" << BIN_HEIGHT+top << "'>" << print_name() <<"</text>";

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
    cerr << data_size << endl;
    cerr << (const char*)items << endl;

        (* buffer ).write( (const char*)items, data_size);
    return data_size;
}
Input
download(const string& address) {
    stringstream buffer;
curl_global_init(CURL_GLOBAL_ALL);
 CURL *curl = curl_easy_init();
 if(curl)
    {
CURLcode res;
curl_easy_setopt(curl, CURLOPT_URL, address.c_str());
curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
 res = curl_easy_perform(curl);
  if (res != CURLE_OK)
  {
     cout << curl_easy_strerror(res) << endl;
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
