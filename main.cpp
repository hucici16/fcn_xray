#include "imageProcessing.h"

extern const std::string PATHTOLABELCOORDS = "./labels_sorted0.txt";
extern const std::string PATHTORAWIMAGES = "./raw_images/";
extern const std::string PATHTOSAVEIMAGES = "./images/";
extern const std::string PATHTOSAVELABELS = "./labels/";

int main( int argc, char** argv )
{
  cv::Mat image;
  cv::Mat label;

  std::ifstream myfile;
  std::string x_s, y_s, w_s, h_s, name, temp, class_name;
  int x, y, w, h;
  int _totalImagesSaved, _totalLabelsSaved, R, G, B;


  temp = "";
  _totalImagesSaved = 0;
  _totalLabelsSaved = 0;

  myfile.open(PATHTOLABELCOORDS);
  int i = 0;
  while (myfile >> name)
  {
    if( name.compare(temp) ) {
      temp = name;
      cout <<"name yes?" <<endl;
      if (image.data) {
        imageProcessing(image, label, labelCoords, labelRGBs, _totalImagesSaved,
          _totalLabelsSaved, PATHTOSAVEIMAGES, PATHTOSAVELABELS);
      }
      else {
        cout <<  "Could not open or find the image" << endl ;
        return -1;
      }
      std::cout << i << "\n";
      i++;
      image = imread(PATHTORAWIMAGES+name, CV_LOAD_IMAGE_GRAYSCALE);
      label = imread(PATHTORAWIMAGES+name, CV_LOAD_IMAGE_COLOR);
      label = Scalar(0,0,0);
      cout <<"main yes?" <<endl;
      std::vector<int*> labelCoords;
      std::vector<int*> labelRGBs;
      cout <<"main yes?" <<endl;

    }
    myfile >> x_s;
    myfile >> y_s;
    myfile >> w_s;
    myfile >> h_s;
    x = stoi(x_s);
    y = stoi(y_s);
    w = stoi(w_s);
    h = stoi(h_s);
    myfile >> class_name;

    int coords[4];
    int RGB[3];
    if (classColorMatch(class_name, R, G, B)) {
      rectangle(label,Point(x,y),Point(x+w,y+h),Scalar(B,G,R),CV_FILLED,8,0);
      rectangle(label,Point(x,y),Point(x+w,y+h),Scalar(198,228,225),3,8,0);
      coords[0] = x;
      coords[1] = y;
      coords[2] = w;
      coords[3] = h;
      RGB[0] = R;
      RGB[1] = G;
      RGB[2] = B;
      labelCoords.push_back(coords);
      labelRGBs.push_back(RGB);
      // crop images?
    }
    else return 0;
  }
  imageProcessing(image, label, labelCoords, labelRGBs, _totalImagesSaved,
    _totalLabelsSaved, PATHTOSAVEIMAGES, PATHTOSAVELABELS);
  std::cout << i << "\n";
  i++;

  myfile.close();

  if (_totalImagesSaved != _totalLabelsSaved) { // Check for numbers of contents
    std::cout << "Whoops! You have different numbers between images and labels" << std::endl;
    return 0;
  }

  return 1;
}
