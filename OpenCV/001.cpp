#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;

int main()
{
    cv::Mat image;
    image = cv::imread("D:/hpx/intellectual_manifest/CNN/Intellectual_manifest/OpenCV/data/2020/0/1.bmp");
    if (image.empty())
    {
        cout<<"aaa"<<endl;
        return 0;
    }
    cv::namedWindow("image 1",cv::WINDOW_NORMAL);
    cv::imshow("image 1", image);
    cv::waitKey(0);
    return 0;
}