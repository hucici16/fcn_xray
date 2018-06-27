#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace cv;
using namespace std;

void saveImages(std::string folder, std::vector<cv::Mat> images, int& _totalSamplesSaved);
void saveSingleImage(std::string folder, cv::Mat image, int& _totalSamplesSaved);
int ImageProcessing(std::vector<cv::Mat>& images, std::vector<cv::Mat>& labels,
  std::vector<int*> labelCoords, std::vector<int*> labelRGBs);
int flipLabelAreaImages(std::vector<cv::Mat>& images, std::vector<cv::Mat>& labels,
  std::vector<int*> labelCoords, int flipType);
int rotateAndFlipWholeImages(std::vector<cv::Mat>& images, size_t n);
int manipulateRGBLabelAreaIntensity(std::vector<cv::Mat>& images, std::vector<cv::Mat>& labels,
  int amount, int times, size_t n, std::vector<int*> labelCoords);
int manipulateRGBImagesIntensity(std::vector<cv::Mat>& images, std::vector<cv::Mat>& labels,
  int amount, int times, size_t n);
int manipulateGrayImagesIntensity(std::vector<cv::Mat>& images, std::vector<cv::Mat>& labels,
  int amount, int times, size_t n);
int classColorMatch(std::string& class_name, int& R, int& G, int& B);
int crop(cv::Rect& roiCrop, int cols, int rows, int x, int y, int w, int h);
int checkLabelCoordinates(cv::Mat image, int x, int y, int w, int h);

// int display(cv::Mat image);
// int gray(cv::Mat image);
// cv::Mat rotate(cv::Mat src, double angle);
// int rotateImages(std::vector<cv::Mat>& images, double angle, size_t n);
// bool checkCoordinates(int x, int y, int w, int h, cv::Mat image);
// bool rejectImageBecauseTooWhiteOrBlack(cv::Mat imgCopy);

#endif
