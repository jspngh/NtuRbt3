#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
#include <stdio.h>
#include <stack>

using namespace std;

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

//size of image is m x n
int main(int argc, char* argv[])
{
    int** dummy = new int*[12];
    for(int x = 0; x < 12; x++)
    {
        dummy[x] = new int[12];
        dummy[x][0] = 0;
        dummy[x][1] = 1;
        dummy[x][2] = 1;
        dummy[x][3] = 0;
        dummy[x][4] = 0;
        dummy[x][5] = 1;
        dummy[x][6] = 1;
        dummy[x][7] = 1;
        dummy[x][8] = 0;
        dummy[x][9] = 0;
        dummy[x][10] = 0;
        dummy[x][11] = 0;
        dummy[x][12] = 0;
    }

    int m = 12;
    int n = 12;
    for (int k = 0; k < m; k++) // height
    {
        for (int j = 0; j < n; j++) // width
        {
            if (dummy[k][j] == 1)
            {
                dummy[k][j] = 255;
            }
        }
    }

    int i = 0;

    for (int k = 0; k < m; k++) // height
    {
        for (int j = 0; j < n; j++) // width
        {
            if (dummy[k][j] == 255)
            {
                i++;
                // region growing algorithm
                grow_region(dummy, k, j, m, n, i);
            }
        }
    }

    for (int x = 0; x < m; ++x)
    {
        for (int y = 0; y < n; ++y)
        {
            std::cout << dummy[x][y] << ' ';
        }
        std::cout << std::endl;
    }
}
