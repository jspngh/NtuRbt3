#ifndef M_IMAGE
#define M_IMAGE

#include <stack>
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include "region.hpp"

#define THRESHOLD 220

using namespace cv;
using namespace std;

class Image
{
public:
    Mat cvImage;
    int** mImage;
    int nr_regions;
    Region** regions;

    Image(string file_loc);
    ~Image();

    void thresholding();
    void segmentation();
    void find_regions();

    void print_image();
private:
    // region growing algorithm
    void grow_region(int k, int j, int i);
};

#endif
