class Region
{
public:
    int id;

    int top; // y coordinate
    int bottom; // y coordinate
    int left; // x coordinate
    int right; // x coordinate

    std::pair<double, double> centroid; // (x_c, y_c)
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
    this->centroid = std::pair<double, double>(-1, -1);
}

void Region::find_centroid(int **image)
{
    int A = m_kj(image, 0, 0);
    double x_c = ((double)m_kj(image, 1, 0)) / A;
    double y_c = ((double)m_kj(image, 0, 1)) / A;
    centroid = std::pair<double, double>(x_c, y_c);
}

void Region::find_principle_angle(int **image)
{
    double phi = 1.0/2 * atan2(2*mu_kj(image,1,1), mu_kj(image,2,0) - mu_kj(image,0,2));
    principle_angle = phi;
}

int Region::m_kj(int **image, int k, int j)
{
    int sum = 0;
    for (int t = top; t <= bottom; t++) {
        for (int u = left; u <= right; u++) {
            if (image[u][t] == id) {
                sum += pow(u, k) * pow(t, j);
            }
        }
    }
    return sum;
}

int Region::mu_kj(int **image, int k, int j)
{
    int sum = 0;
    for (int t = top; t <= bottom; t++) {
        for (int u = left; u <= right; u++) {
            if (image[u][t] == id) {
                sum += pow((u - centroid.first), k) * pow((t - centroid.second), j);
            }
        }
    }
    return sum;
}
