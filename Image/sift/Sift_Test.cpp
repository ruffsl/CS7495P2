#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>
using namespace cv;

int main(int argc, char** argv)
{

    if(argc < 2)
        return -1;

    Mat img = imread(argv[1]);

    SIFT sift;
    vector<KeyPoint> key_points;

    Mat descriptors;
    sift(img, Mat(), key_points, descriptors);

    Mat output_img;
    drawKeypoints(img, key_points, output_img);

    namedWindow("Image");
    imshow("Image", output_img);
    waitKey(0);
    destroyWindow("Image");

    return 0;
}
