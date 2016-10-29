#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include <iostream>
#include <sstream>
#include <stack>
#include <stdio.h>
#include <string>
#include <time.h>


#define THRESHOLD 220

using namespace cv;
using namespace std;

void image_thresholding(Mat image, int **result);
void image_segmentation(int **image, int rows, int cols);
void image_centroid(int **image);
void image_principle_angle(int **image);
void grow_region(int** image, int k, int j, int m, int n, int i);

int main(int argc, char* argv[])
{
    
    Mat image = imread("./images/er7-1-small.jpg",CV_LOAD_IMAGE_GRAYSCALE);

    int **result = new int*[image.rows];
    image_thresholding(image, result);

    image_segmentation(result, image.rows, image.cols);
    
    // just to test
    for (int x = 0; x < image.rows; ++x)
    {
        for (int y = 0; y < image.cols; ++y)
        {
            std::cout << result[x][y] << ' ';
        }
        std::cout << std::endl;
    }

    //namedWindow("image", CV_WINDOW_AUTOSIZE);
    //imshow("image", image);
    //waitKey();

    // clean up
    delete result;
}

void image_thresholding(Mat image, int** result)
{

    for(int j=0;j<image.rows;j++) 
    {
        result[j] = new int[image.cols];
        for (int i=0;i<image.cols;i++)
        {
            int value = (int)image.at<uchar>(j,i);

            if (value > THRESHOLD)
                result[j][i] = 1;
            else
                result[j][i] = 0;
        }
    }
}

void image_segmentation(int** image, int m, int n)
{
    for (int k = 0; k < m; k++) // height
    {
        for (int j = 0; j < n; j++) // width
        {
            if (image[k][j] == 1)
            {
                image[k][j] = 255;
            }
        }
    }

    int i = 0;

    for (int k = 0; k < m; k++) // height
    {
        for (int j = 0; j < n; j++) // width
        {
            if (image[k][j] == 255)
            {
                i++;
                // region growing algorithm
                grow_region(image, k, j, m, n, i);
            }
        }
    }
}

void grow_region(int** image, int k, int j, int m, int n, int i)
{
    stack<pair<int, int>> region;
    region.push(make_pair(k, j));
    region.push(make_pair(-1, -1));
    pair<int, int> current_pos(k, j);

    while(!region.empty())
    {
        k = current_pos.first;
        j = current_pos.second;

        //top
        if (k > 0 && image[k-1][j] == 255)
        {
            region.push(make_pair(k-1, j));
            image[k-1][j] = i;
        }
        //bottom
        if (k < (m-1) && image[k+1][j] == 255)
        {
            region.push(make_pair(k+1, j));
            image[k+1][j] = i;
        }

        //right
        if (j < (n-1) && image[k][j+1] == 255)
        {
            region.push(make_pair(k, j+1));
            image[k][j+1] = i;
        }

        //left
        if (j > 0 && image[k][j-1] == 255)
        {
            region.push(make_pair(k, j-1));
            image[k][j-1] = i;
        }

        current_pos = region.top();
        region.pop();
        if(current_pos.first == -1 || current_pos.second == -1)
            break;
    }
}

