#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include<iostream>
#include<stdio.h>

#define THRESHOLD 220

using namespace cv;
using namespace std;

int main()
{
    
    Mat image = imread("./images/er7-4.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    int **th_image = new int*[image.rows];


    for(int j=0;j<image.rows;j++) 
    {
        th_image[j] = new int[image.cols];
        for (int i=0;i<image.cols;i++)
        {
            int value = (int)image.at<uchar>(j,i);

            if (value > THRESHOLD)
                th_image[j][i] = 1;
            else
                th_image[j][i] = 0;
        }
    }

    //namedWindow("image", CV_WINDOW_AUTOSIZE);
    //imshow("image", image);
    //waitKey();
}
