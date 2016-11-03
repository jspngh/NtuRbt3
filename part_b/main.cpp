#include "image.hpp"

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    Image* img = new Image("./images/er7-3.jpg");

    img->thresholding();
    img->segmentation();
    img->find_regions();

    // img->print_image();
    img->display_region_metadata();

    delete img;
}

