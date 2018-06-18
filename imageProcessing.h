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
int imageProcessing(cv::Mat image, cv::Mat label, int& _totalImagesSaved, int& _totalLabelsSaved,
  std::string folderToSaveImages, std::string folderToSaveLabels);
int rotateAndFlipWholeImage(std::vector<cv::Mat>& images);
int manipulateImagesIntensity(std::vector<cv::Mat>& images, std::vector<cv::Mat>& labels,
  int amount, int times, size_t n);
int classColorMatch(std::string& class_name, int& R, int& G, int& B);
int display(cv::Mat image);
int gray(cv::Mat image);
cv::Mat rotate(cv::Mat src, double angle);
void flipWholeImages(std::vector<cv::Mat>& images, size_t n);
int rotateImages(std::vector<cv::Mat>& images, double angle, size_t n);

// bool checkCoordinates(int x, int y, int w, int h, cv::Mat image);
// bool rejectImageBecauseTooWhiteOrBlack(cv::Mat imgCopy);
// int manipulateImageIntensity(cv::Mat image, cv::Mat label, int amount, int times,
//  int& _totalImagesSaved, int& _totalLabelsSaved);
// void invertImage(std::vector<cv::Mat> small_patches_for_dl, std::vector<cv::Mat>& out);

#endif
