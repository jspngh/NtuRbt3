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

#define THRESHOLD 220

using namespace cv;
using namespace std;

class Region
{
public:
    int id;

    int top; // y coordinate
    int bottom; // y coordinate
    int left; // x coordinate
    int right; // x coordinate

    pair<double, double> centroid; // (x_c, y_c)
    double principle_angle; // phi

    Region(int id, int top, int bottom, int left, int right);
    void find_centroid(int** image);
    void find_principle_angle(int** image);
private:
    int m_kj(int** image, int k, int j); // moment kj
    int mu_kj(int** image, int k, int j); // central moment kj
};

Region::Region(int id, int top, int bottom, int left, int right)
{
    this->id = id;
    this->top = top;
    this->bottom = bottom;
    this->left = left;
    this->right = right;
}

void Region::find_centroid(int **image)
{
    int A = m_kj(image, 0, 0);
    double x_c = ((double)m_kj(image, 1, 0)) / A;
    double y_c = ((double)m_kj(image, 0, 1)) / A;
    centroid = pair<double, double>(x_c, y_c);
}

void Region::find_principle_angle(int **image)
{
    double phi = 1.0/2 * atan2(2*mu_kj(image,1,1), mu_kj(image,2,0) - mu_kj(image,0,2));
    principle_angle = phi;
}

int Region::m_kj(int **image, int k, int j)
{
	int sum = 0;
	for (t = Region.top; t<Region.bottom; t++) {
		for (u = Region.left; u<Region.right; u++) {
			if (image[u][t] == Region.id) {
				sum += (u^k)*(t^j)
			}
		}
	}
	return sum;
}

int Region::mu_kj(int **image, int k, int j)
{
	int Region::mu_kj(int **image, int k, int j)
	{
		int sum2 = 0;
		for (t = Region.top; t<Region.bottom; t++) {
			for (u = Region.left; u = Region.right; u++) {
				if (image[u][t] == Region.id) {
					sum2 += ((u - centroid.first) ^ k)*((t - centroid.second) ^ j)
				}
			}
			return sum2;
}

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
    void find_centroids();
    void find_principle_angles();

    void print_image();
private:
    // region growing algorithm
    void grow_region(int k, int j, int i);
};


int main(int argc, char* argv[])
{
    Image* img = new Image("./images/er7-1-small.jpg");

    img->thresholding();
    img->segmentation();
    img->find_regions();

    img->print_image();

    delete img;
}

Image::Image(string file_loc)
{
    cvImage = imread(file_loc, CV_LOAD_IMAGE_GRAYSCALE);
    mImage = new int*[cvImage.rows];
    for (int i = 0; i < cvImage.rows; ++i)
    {
        mImage[i] = new int[cvImage.cols];
    }
    nr_regions = 0;
}

Image::~Image()
{
    for (int i = 0; i < cvImage.rows; ++i)
        delete[] mImage[i];
    delete[] mImage;

    for (int i = 0; i < nr_regions; ++i)
        delete regions[i];
    delete[] regions;
}

void Image::thresholding()
{

    for(int j=0; j<cvImage.rows; j++)
    {
        mImage[j] = new int[cvImage.cols];
        for (int i = 0; i < cvImage.cols; i++)
        {
            int value = (int)cvImage.at<uchar>(j,i);

            if (value > THRESHOLD)
                mImage[j][i] = 255;
            else
                mImage[j][i] = 0;
        }
    }
}

void Image::segmentation()
{
    int i = 0;

    for (int k = 0; k < cvImage.rows; k++) // height
    {
        for (int j = 0; j < cvImage.cols; j++) // width
        {
            if (mImage[k][j] == 255)
            {
                i++;
                grow_region(k, j, i);
            }
        }
    }
    nr_regions = i;
}

void Image::find_regions()
{
    if (nr_regions <= 0)
        return;

    regions = new Region*[nr_regions];
    for (int i = 1; i <= nr_regions; i++)
    {
        int top=cvImage.rows, bottom=0, right=cvImage.cols, left=0;
        for (int y = 0; y < cvImage.rows; y++)
        {
            for (int x = 0; x < cvImage.cols; x++)
            {
                if (mImage[y][x] == i)
                {
                    if (y < top)
                        top = y;

                    if (y > bottom)
                        bottom = y;

                    if (x < right)
                        right = x;

                    if (x > left)
                        left = x;
                }
            }
        }
        regions[i-1] = new Region(i, top, bottom, left, right);
    }
}

void Image::grow_region(int k, int j, int i)
{
    int m = cvImage.rows; // height
    int n = cvImage.cols; // width
    stack<pair<int, int>> region;
    region.push(make_pair(-1, -1));
    pair<int, int> current_pos(k, j);

    while(!region.empty())
    {
        k = current_pos.first;
        j = current_pos.second;

        //top
        if (k > 0 && mImage[k-1][j] == 255)
        {
            region.push(make_pair(k-1, j));
            mImage[k-1][j] = i;
        }
        //bottom
        if (k < (m-1) && mImage[k+1][j] == 255)
        {
            region.push(make_pair(k+1, j));
            mImage[k+1][j] = i;
        }

        //right
        if (j < (n-1) && mImage[k][j+1] == 255)
        {
            region.push(make_pair(k, j+1));
            mImage[k][j+1] = i;
        }

        //left
        if (j > 0 && mImage[k][j-1] == 255)
        {
            region.push(make_pair(k, j-1));
            mImage[k][j-1] = i;
        }

        current_pos = region.top();
        region.pop();
        if(current_pos.first == -1 || current_pos.second == -1)
            break;
    }
}

void Image::print_image()
{
    cout << "Image" << endl;
    for (int x = 0; x < cvImage.rows; ++x)
    {
        for (int y = 0; y < cvImage.cols; ++y)
        {
            cout << mImage[x][y] << ' ';
        }
        cout << endl;
    }

    cout << "Regions" << endl;
    for (int i = 0; i < nr_regions; ++i)
    {
        cout << "id: " << regions[i]->id
             << " ,top: " << regions[i]->top
             << " ,bottom: " << regions[i]->bottom
             << " ,left: " << regions[i]->left
             << " ,right: " << regions[i]->right
             << endl;
    }
}

