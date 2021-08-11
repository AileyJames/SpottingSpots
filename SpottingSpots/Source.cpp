
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;

void contourTime(Mat imgGray, Mat img);
Mat imgGray;

int main(int argc, char** argv)
{
    //reading in image
    string path = "Resources/test.jpg";
    Mat img = imread(path);

    //Preprocessing image
    cvtColor(img, imgGray, COLOR_BGR2GRAY);
    GaussianBlur(imgGray, imgGray, Size(3, 3), 3, 0);
    contourTime(imgGray, img);

    waitKey(0);
}

void contourTime(Mat imgGray, Mat img) {
    Mat imgCanny;
    
    Canny(imgGray, imgCanny, 25, 75);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    dilate(imgCanny, imgCanny, kernel);


    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(imgCanny, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0,0));

    vector<Moments> m1(contours.size());
    vector<Point2f> m2(contours.size());
    Rect rect;
    for (int i = 0; i < contours.size(); i++)
    {
        m1[i] = moments(contours[i], false);
        rect = boundingRect(contours[i]);
    }

    for (int i = 0; i < contours.size(); i++)
    {
        m2[i] = Point2f(m1[i].m10 / m1[i].m00, m1[i].m01 / m1[i].m00);
    }

    //start drawin

    Mat drawing(imgCanny.size(), CV_8UC3, Scalar(255, 255, 255));

    for (int i = 0; i < contours.size(); i++)
    {
        string num = to_string(i + 1);
        Scalar color = Scalar(25, 25, 255);
        Scalar text = Scalar(255, 100, 0);
        drawContours(img, contours, i, color, 1, 1, hierarchy, 0, Point() );
        circle(img, m2[i], 1, color, -1, 1, 0);
        putText(img, num, m2[i], FONT_HERSHEY_PLAIN, 1, text);
    }   

    namedWindow("Contours", WINDOW_AUTOSIZE);
    imshow("Contours", img);
    waitKey(0);

    
}