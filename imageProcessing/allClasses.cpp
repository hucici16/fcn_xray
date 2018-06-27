#include "imageProcessing.h"

extern const std::string PATHTOLABELCOORDS = "/media/inwatec/28E0FB57E0FB29A8/Cecilia/raw_images/IMAGES_12227/labels_sorted.txt";
extern const std::string PATHTORAWIMAGES = "/media/inwatec/28E0FB57E0FB29A8/Cecilia/raw_images/IMAGES_12227/";
extern const std::string PATHTOSAVEIMAGES = "/media/inwatec/28E0FB57E0FB29A8/Cecilia/Xray_Seg/ver5.3/images/";
extern const std::string PATHTOSAVELABELS = "/media/inwatec/28E0FB57E0FB29A8/Cecilia/Xray_Seg/ver5.3/labels/";

int main( int argc, char** argv )
{
  cv::Mat image, label;
  // cv::Mat croppedImage, croppedLabel;
  // cv::Rect roiCrop;

  std::vector<cv::Mat> images;
  std::vector<cv::Mat> labels;
  std::vector<int*> labelCoords;
  std::vector<int*> labelRGBs;

  std::ifstream myfile;
  std::string name, temp, class_name;
  int x, y, w, h;
  int _totalImagesSaved, _totalLabelsSaved, R, G, B;
  int counter = 0;

  temp = "";
  _totalImagesSaved = 0;
  _totalLabelsSaved = 0;

  myfile.open(PATHTOLABELCOORDS);
  while (myfile >> name)
  {
    myfile >> x;
    myfile >> y;
    myfile >> w;
    myfile >> h;
    myfile >> class_name;

    if(name.compare(temp)) {
      temp = name;
      if(image.data) { // Check for invalid input
        images.clear();
        labels.clear();
        images.push_back(image);
        labels.push_back(label);
        // ImageProcessing(images, labels, labelCoords, labelRGBs);
        if (images.size() != labels.size() || images.size() != 1) {
          std::cout << "Whoops! partialImagesProcessing has problem! " << std::endl;
        }
        std::cout << "saving images***************************" << std::endl;
        saveImages(PATHTOSAVEIMAGES, images, _totalImagesSaved);
        std::cout << "saving labels***************************" << std::endl;
        saveImages(PATHTOSAVELABELS, labels, _totalLabelsSaved);
        images.clear();
        labels.clear();
      }
      image = imread(PATHTORAWIMAGES+name, CV_LOAD_IMAGE_COLOR);
      label = Mat::zeros(image.size(),image.type());
      labelCoords.clear();
      labelRGBs.clear();
    }
    if (classColorMatch(class_name, R, G, B)) {
      if (checkLabelCoordinates(image, x, y, w, h)) {
        rectangle(label,Point(x,y),Point(x+w,y+h),Scalar(B,G,R),CV_FILLED,8,0);
        rectangle(label,Point(x,y),Point(x+w,y+h),Scalar(198,228,225),3,8,0);
        int coords[4];
        int RGB[3];
        coords[0] = x;
        coords[1] = y;
        coords[2] = w;
        coords[3] = h;
        RGB[0] = R;
        RGB[1] = G;
        RGB[2] = B;
        labelCoords.push_back(coords);
        labelRGBs.push_back(RGB);
      }
    }
    else {
      std::cout << "Whoops! New class of object!" << std::endl;
      return 0;
    }
    // display(image);
  }
  if(image.data) { // Check for invalid input
    images.clear();
    labels.clear();
    images.push_back(image);
    labels.push_back(label);
    // ImageProcessing(images, labels, labelCoords, labelRGBs);
    if (images.size() != labels.size() || images.size() != 1) {
      std::cout << "Whoops! partialImagesProcessing has problem! " << std::endl;
    }
    std::cout << "saving images***************************" << std::endl;
    saveImages(PATHTOSAVEIMAGES, images, _totalImagesSaved);
    std::cout << "saving labels***************************" << std::endl;
    saveImages(PATHTOSAVELABELS, labels, _totalLabelsSaved);
    images.clear();
    labels.clear();
  }
  myfile.close();

  if (_totalImagesSaved != _totalLabelsSaved) { // Check for numbers of contents
    std::cout << "Whoops! You have different numbers between images and labels" << std::endl;
    return 0;
  }

  return 1;
}
