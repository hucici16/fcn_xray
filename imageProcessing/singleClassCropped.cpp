#include "imageProcessing.h"

extern const std::string PATHTOLABELCOORDS = "/home/inwatec/Xray_Seg/raw_images/IMAGES_622/labels_sorted.txt";
extern const std::string PATHTORAWIMAGES = "/home/inwatec/Xray_Seg/raw_images/IMAGES_622/";
extern const std::string PATHTOSAVEIMAGES = "/media/inwatec/28E0FB57E0FB29A8/Cecilia/Xray_Seg/ver5.0/images/";
extern const std::string PATHTOSAVELABELS = "/media/inwatec/28E0FB57E0FB29A8/Cecilia/Xray_Seg/ver5.0/labels/";

int main( int argc, char** argv )
{
  std::string specifiedClass;
  if (argc == 1) {
    specifiedClass = "penlike_object";
  }
  else if (argc == 2) {
    specifiedClass.assign(argv[1]);
  }
  else {
    std::cout << "Usage: ./singleclass <class of object>" << std::endl;
    return 0;
  }

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
  // int counter = 0;

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

    if (!class_name.compare(specifiedClass) && classColorMatch(class_name, R, G, B)) {
      image = imread(PATHTORAWIMAGES+name, CV_LOAD_IMAGE_COLOR);
      if(image.data) { // Check for invalid input
        label = Mat::zeros(image.size(),image.type());
        if (checkLabelCoordinates(image, x, y, w, h)) {
          rectangle(label,Point(x,y),Point(x+w,y+h),Scalar(B,G,R),CV_FILLED,8,0);
          rectangle(label,Point(x,y),Point(x+w,y+h),Scalar(198,228,225),3,8,0);

          images.clear();
          labels.clear();
          images.push_back(image);
          labels.push_back(label);
          ImageProcessing(images, labels, labelCoords, labelRGBs);
          if (images.size() != labels.size() || images.size() != 16 * 24) {
            std::cout << "Whoops! partialImagesProcessing has problem! " << std::endl;
          }
          std::cout << "saving images***************************" << std::endl;
          saveImages(PATHTOSAVEIMAGES, images, _totalImagesSaved);
          std::cout << "saving labels***************************" << std::endl;
          saveImages(PATHTOSAVELABELS, labels, _totalLabelsSaved);
          images.clear();
          labels.clear();
        }
      }
    }
    // display(image);
  }
  myfile.close();

  if (_totalImagesSaved != _totalLabelsSaved) { // Check for numbers of contents
    std::cout << "Whoops! You have different numbers between images and labels" << std::endl;
    return 0;
  }

  return 1;
}
