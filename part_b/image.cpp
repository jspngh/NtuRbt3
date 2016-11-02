#include "image.hpp"

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
        // TODO Jonas: right and left were switched
        regions[i-1] = new Region(mImage, i, top, bottom, right, left);
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
    cout    << "Image [rows x cols]: ["
            << cvImage.rows << " x " << cvImage.cols << "]" << endl;

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
        double x_c = regions[i]->centroid.first;
        double y_c = regions[i]->centroid.second;
        double phi = regions[i]->principle_angle;

        cout << "id: " << regions[i]->id
             << ", top: " << regions[i]->top
             << ", bottom: " << regions[i]->bottom
             << ", left: " << regions[i]->left
             << ", right: " << regions[i]->right
             << "\ncenter: " << x_c  << " " << y_c
             << "\nphi: " << phi
             << endl;
    }
}