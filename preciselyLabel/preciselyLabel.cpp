/*
* Adopted from https://github.com/opencv/opencv/blob/master/samples/cpp/create_mask.cpp
* create_mask.cpp
*
* Author:
* Siddharth Kherada <siddharthkherada27[at]gmail[dot]com>
*
* This tutorial demonstrates how to make mask image (black and white).
* The program takes as input a source image and outputs its corresponding
* mask image.
*
* This program demonstrates using mouse events, usage
* left mouse button - set a point to create mask shape
* right mouse button - create mask from points
* middle mouse button - reset
*/

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp> // yes
#include <opencv2/highgui/highgui.hpp> // yes
#include <opencv2/imgproc/imgproc.hpp> // yes
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream> // yes
#include <stdlib.h> // yes

using namespace std;
using namespace cv;

extern const std::string PATHTOIMAGES = "/home/inwatec/Xray_Seg/ver4.1/images/";
extern const std::string PATHTOSAVELABELS = "./labels/";
extern const int startNum = 87;
extern const int totalNum = 165;

cv::Mat img0, img1, res1, final, label;

cv::Point point;
cv::Point* pts = new Point[100];

int minx, miny, maxx, maxy, lenx, leny;

int numpts = 100;
int drag = 0;
int var = 0;
int flag = 0;
int flag1 = 0;
int counter;

void mouseHandler(int, int, int, int, void*);

int main(int argc, char **argv)
{
  for (counter = startNum; counter <= totalNum; counter++) {
    cv::Mat src = cv::imread(PATHTOIMAGES+std::to_string(counter)+".png");

    minx = INT_MAX;
    miny = INT_MAX;
    maxx = INT_MIN;
    maxy = INT_MIN;

    img0 = src;

    res1 = cv::Mat::zeros(img0.size(), CV_8UC1);
    final = cv::Mat::zeros(img0.size(), CV_8UC3);

    //////////// source image ///////////////////
    cv::namedWindow("Source"+std::to_string(counter)+".png", 1);
    cv::setMouseCallback("Source"+std::to_string(counter)+".png", mouseHandler, NULL);
    cv::imshow("Source"+std::to_string(counter)+".png", img0);
    cv::waitKey(0);
    cv::destroyAllWindows(); // doesn't have to be here, but put it here to secure everything is clear
  }

  return 0;
}

/*********************************************************************************/
void mouseHandler(int event, int x, int y, int, void*)
{
  if (event == EVENT_LBUTTONDOWN && !drag)
  {
    if (flag1 == 0)
    {
      std::cout << "Button Down!!" << std::endl;
      if (var == 0) img1 = img0.clone();
      point = cv::Point(x, y);
      cv::circle(img1, point, 2, cv::Scalar(0, 0, 255), -1, 8, 0);
      pts[var] = point;
      var++;
      drag = 1;
      if (var > 1) cv::line(img1, pts[var-2], point, cv::Scalar(0, 0, 255), 2, 8, 0);

      cv::imshow("Source"+std::to_string(counter)+".png", img1);
    }
  }

  if (event == EVENT_LBUTTONUP && drag)
  {
    cv::imshow("Source"+std::to_string(counter)+".png", img1);
    drag = 0;
  }
  if (event == EVENT_RBUTTONDOWN)
  {
    flag1 = 1;
    img1 = img0.clone();
    for (int i = var; i < numpts; i++) pts[i] = point;

    if (var != 0)
    {
      const Point* pts3[1] = {&pts[0]};
      cv::polylines(img1, pts3, &numpts, 1, 1, cv::Scalar(0,0,0), 2, 8, 0);
    }

    for (int i = 0; i < var; i++)
    {
      minx = min(minx, pts[i].x);
      maxx = max(maxx, pts[i].x);
      miny = min(miny, pts[i].y);
      maxy = max(maxy, pts[i].y);
    }
    lenx = maxx - minx;
    leny = maxy - miny;

    cv::imshow("Source"+std::to_string(counter)+".png", img1);
  }

  if (event == EVENT_RBUTTONUP)
  {
    flag = var;


    const cv::Point* pts4[1] = {&pts[0]};

    /*** This section is for create black and white mask
    final = Mat::zeros(img0.size(),CV_8UC3);
    res1 = Mat::zeros(img0.size(),CV_8UC1);
    fillPoly(res1, pts4,&numpts, 1, Scalar(255, 255, 255), 8, 0);
    bitwise_and(img0, img0, final,res1);
    imshow("mask",res1);
    imwrite("mask.png",res1);
    imshow("Source", img1);
    ***/

    /***My Specification***/
    label = cv::Mat::zeros(img0.size(), img0.type());
    cv::fillPoly(label, pts4, &numpts, 1, cv::Scalar(0, 0, 204), 8, 0);
    cv::polylines(label, pts4, &numpts, 1, 1, cv::Scalar(198,228,225), 2, 8, 0);
    cv::imwrite(PATHTOSAVELABELS+ std::to_string(counter)+".png", label);
    cv::imshow("label"+std::to_string(counter)+".png", label);
    // Reset everything for next image
    for (int i = 0; i < numpts; i++)
    {
      pts[i].x = 0;
      pts[i].y = 0;
    }
    var = 0;
    flag1 = 0;
    minx = INT_MAX;
    miny = INT_MAX;
    maxx = INT_MIN;
    maxy = INT_MIN;
    drag = 0;
    /***My Specification Ends***/
  }
  if (event == EVENT_MBUTTONDOWN)
  {
    for (int i = 0; i < numpts; i++)
    {
      pts[i].x = 0;
      pts[i].y = 0;
    }
    var = 0;
    flag1 = 0;
    minx = INT_MAX;
    miny = INT_MAX;
    maxx = INT_MIN;
    maxy = INT_MIN;
    cv::imshow("Source"+std::to_string(counter)+".png", img0);
    drag = 0;
  }
}
