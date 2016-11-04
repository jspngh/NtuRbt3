#include <iostream>
#include <string>

#include "image.hpp"

using namespace cv;
using namespace std;



int main(int argc, char* argv[])
{
    string file_name;
    cout << "Please, enter the file name ";
    getline (cin,file_name);

    Image* img = new Image(file_name);
    if (img->reading_error)
    {
        cout << "Could not open the image, is the name correct?" << endl;
        return -1;
    }

    cout << "Processing " << file_name << endl;
    img->thresholding();
    img->segmentation();
    img->find_regions();

    img->print_region_metadata();

    // uncomment to display the result
    // img->display_region_metadata();

    // uncomment to store the resulting image
    //cv::imwrite("./images/er7-3-result.jpg", img->get_cvImage_result());
    
    delete img;
}

