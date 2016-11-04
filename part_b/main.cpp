#include "image.hpp"

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    Image* img = new Image("./images/er7-4.jpg");

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

