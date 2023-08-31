#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

int main()
{
    cv::Mat image, gray;
    image = cv::imread("D:/hpx/intellectual_manifest/CNN/Intellectual_manifest/OpenCV/data/2020/1/1.bmp");
    if (image.empty())
    {
        cout << "aaa" << endl;
        return 0;
    }

    // for (int i = 0; i <= 150; i++)
    // {
    //     for (int j = 0; j <= 200; j++)
    //     {
    //         image.at<cv::Vec3b>(j, i) = cv::Vec3b(255, 255, 255);
    //     }
    // }

    cv::resize(image, image, cv::Size(image.cols / 4, image.rows / 4));
    cv::GaussianBlur(image, image, cv::Size(9, 9), 2, 2);
    cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY);
    cv::threshold(gray, gray, 100, 255, cv::THRESH_BINARY);
    vector<cv::Vec3f> circles;
    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 2, 50);
    for (size_t i = 0; i < circles.size(); i++)
    {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        cv::circle(image, center, 3, cv::Scalar(0, 255, 0), -1, 8, 0);
        cv::circle(image, center, radius, cv::Scalar(0, 0, 255), 3, 8, 0);
    }
    int center_x = cvRound(circles[0][0]);
    int center_y = cvRound(circles[0][1]);
    image = image(cv::Range(center_y - 150, center_y + 150), cv::Range(center_x - 150, center_x + 150));
    gray = gray(cv::Range(center_y - 150, center_y + 150), cv::Range(center_x - 150, center_x + 150));

    vector<cv::Vec4i> hierarchy;
    vector<vector<cv::Point>> contours0;
    cv::findContours(gray, contours0, cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE);
    cv::Mat contours = cv::Mat::zeros(gray.size(), CV_8UC1);
    for (size_t i = 0; i < contours0.size(); i++)
    {
        for (size_t j = 0; j < contours0[i].size(); j++)
        {
            contours.at<uchar>(cv::Point(contours0[i][j].x, contours0[i][j].y)) = 255;
        }
    }
    cv::namedWindow("contours", 1);
    cv::imshow("contours", contours);

    cv::namedWindow("origin", 0);
    cv::namedWindow("threshold", 0);
    cv::imshow("origin", image);
    cv::imshow("threshold", gray);
    cv::waitKey(0);
    return 0;
}