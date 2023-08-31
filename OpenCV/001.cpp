#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

#define max(a,b) a>b?a:b
#define min(a,b) a>b?b:a


#define CENTER_BIAS 450
#define IMAGE_1 "D:/hpx/intellectual_manifest/CNN/Intellectual_manifest/OpenCV/data/2020/0/1.bmp"
#define IMAGE_2 "D:/hpx/intellectual_manifest/CNN/Intellectual_manifest/OpenCV/data/2020/0/2.bmp"
#define IMAGE_3 "D:/hpx/intellectual_manifest/CNN/Intellectual_manifest/OpenCV/data/2020/1/1.bmp"

int calc_distance(int x,int y,int center_x,int center_y)
{
    return pow(x-center_x,2)+pow(y-center_y,2);
}

int main()
{
    cv::Mat image, gray, image_2;
    image = cv::imread(IMAGE_1);
    if (image.empty())
    {
        cout << "aaa" << endl;
        return 0;
    }

    // for (int i = 0; i <= 100; i++)
    // {
    //     for (int j = 0; j <= 200; j++)
    //     {
    //         image.at<cv::Vec3b>(j, i) = cv::Vec3b(255, 255, 255);
    //     }
    // }

    // cv::resize(image, image, cv::Size(image.cols / 4, image.rows / 4));
    cv::GaussianBlur(image, image_2, cv::Size(9, 9), 2, 2);
    cv::cvtColor(image_2, gray, cv::COLOR_RGB2GRAY);
    cv::threshold(gray, gray, 100, 255, cv::THRESH_BINARY);
    vector<cv::Vec3f> circles;
    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 2, 50);
    int center_x, center_y;
    {
        int index = -1;
        int circleR = 0;
        for (size_t i = 0; i < circles.size(); i++)
        {
            cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            cv::circle(image, center, 3, cv::Scalar(0, 255, 0), -1, 8, 0);
            cv::circle(image, center, radius, cv::Scalar(0, 0, 255), 3, 8, 0);
            if (circleR < radius && radius < 150)
            {
                circleR = radius;
                index = i;
            }
        }
        if (index == -1)
        {
            cout << "bbb";
            return 0;
        }
        center_x = cvRound(circles[index][0]);
        center_y = cvRound(circles[index][1]);
    }
    image = image(cv::Range(center_y - CENTER_BIAS, center_y + CENTER_BIAS), cv::Range(center_x - CENTER_BIAS, center_x + CENTER_BIAS));
    gray = gray(cv::Range(center_y - CENTER_BIAS, center_y + CENTER_BIAS), cv::Range(center_x - CENTER_BIAS, center_x + CENTER_BIAS));

    /**
     * @brief contour detect
     */
    vector<cv::Vec4i> hierarchy;
    vector<vector<cv::Point>> contours0;
    cv::findContours(gray, contours0, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    cv::Mat contours = cv::Mat::zeros(gray.size(), CV_8UC1);
    cv::drawContours(contours, contours0, -1, 80);

    /**
     * @brief find max/min distance
     */
    int dis_max = 0, dis_min = 90000;
    {
        int index = -1;
        int tmp_dis;
        for (size_t i = 0; i < contours0.size(); i++)
        {
            for (size_t j = 0; j < contours0[i].size(); j++)
            {
                tmp_dis=calc_distance(contours0[i][j].x,contours0[i][j].y,CENTER_BIAS,CENTER_BIAS);
                dis_max=max(dis_max,tmp_dis);
                dis_min=min(dis_min,tmp_dis);
            }
        }
    }
    // test--
    cout<<sqrt(dis_max)<<endl<<sqrt(dis_min);
    cv::circle(contours,cv::Point(CENTER_BIAS,CENTER_BIAS),sqrt(dis_max),255);
    cv::circle(contours,cv::Point(CENTER_BIAS,CENTER_BIAS),sqrt(dis_min),255);
    // --test
    cv::namedWindow("contours", 1);
    cv::imshow("contours", contours);

    cv::namedWindow("origin", 0);
    cv::namedWindow("threshold", 0);
    cv::imshow("origin", image);
    cv::imshow("threshold", gray);
    cv::waitKey(0);
    return 0;
}