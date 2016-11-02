#include "region.hpp"
#include "image.hpp"

#define THRESHOLD 220

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    Image* img = new Image("./images/er7-1-small.jpg");

    img->thresholding();
    img->segmentation();
    img->find_regions();

    img->print_image();
    //delete img;
}

