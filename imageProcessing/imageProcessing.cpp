#include "imageProcessing.h"

/*********************************************************************************/
void saveImages(std::string folder, std::vector<cv::Mat> images, int& _totalSamplesSaved)
{
  for (size_t i = 0; i < images.size(); i++)
  {
    std::stringstream name;
    name << folder << _totalSamplesSaved+1  << ".png";
    cv::imwrite(name.str(), images[i]);
    _totalSamplesSaved++;
    if (_totalSamplesSaved % 10 == 0 ) std::cout <<_totalSamplesSaved << std::endl;
  }
}
/*********************************************************************************/
void saveSingleImage(std::string folder, cv::Mat image, int& _totalSamplesSaved)
{
  std::stringstream name;
  name << folder << _totalSamplesSaved+1 << ".png";
  cv::imwrite(name.str(), image);
  _totalSamplesSaved++;
  if (_totalSamplesSaved % 100 == 0 ) std::cout << _totalSamplesSaved << std::endl;
}
/*********************************************************************************/
int ImageProcessing(std::vector<cv::Mat>& images, std::vector<cv::Mat>& labels, std::vector<int*> labelCoords, std::vector<int*> labelRGBs)
{

  for (int i = 0; i < 3; i++) flipLabelAreaImages(images, labels, labelCoords, i); //1 +3 =4

  manipulateRGBLabelAreaIntensity(images, labels, 20, 3, 4, labelCoords); // 4+4*3 = 16
  manipulateRGBImagesIntensity(images, labels, 20, 3, 4); // 16+4*3 =28

  rotateAndFlipWholeImages(images, images.size()); // 28*6 = 168
  rotateAndFlipWholeImages(labels, labels.size());

  return 1;
}
/*********************************************************************************/
int flipLabelAreaImages(std::vector<cv::Mat>& images, std::vector<cv::Mat>& labels,
                        std::vector<int*> labelCoords, int flipType)
{
  int x, y, w, h;
  cv::Mat srcImg, dstImg, srcLabel, dstLabel;
  cv::Mat map_x, map_y;
  images[0].copyTo(srcImg);
  labels[0].copyTo(srcLabel);
  dstImg.create(srcImg.size(), srcImg.type());
  dstLabel.create(srcLabel.size(), srcLabel.type());
  map_x.create(srcImg.size(), CV_32FC1);
  map_y.create(srcImg.size(), CV_32FC1);

  for (int r = 0; r < srcImg.rows; r++)
  {
    for (int c = 0; c < srcImg.cols; c++)
    {
      map_x.at<float>(r,c) = c;
      map_y.at<float>(r,c) = r;
    }
  }

  for (size_t j = 0; j < labelCoords.size(); j++) {
    x = labelCoords[j][0];
    y = labelCoords[j][1];
    w = labelCoords[j][2];
    h = labelCoords[j][3];
    for (int r = y; r <y+h; r++)
    {
      for (int c = x; c < x+w; c++)
      {
        if (flipType == 0) { //flip vertically
          map_x.at<float>(r,c) = (float)c;
          map_y.at<float>(r,c) = (float)((y+h) - (r-y));
        }
        else if (flipType == 1) { //flip horizontally
          map_x.at<float>(r,c) = (float)((x+w) - (c-x));
          map_y.at<float>(r,c) = (float)r;
        }
        else if (flipType == 2) { //flip both vertically and horizontally
          map_x.at<float>(r,c) = (float)((x+w) - (c-x));
          map_y.at<float>(r,c) = (float)((y+h) - (r-y));
        }
        else;
      }
    }
  }

  remap(srcImg, dstImg, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT);
  remap(srcLabel, dstLabel, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT);
  images.push_back(dstImg);
  labels.push_back(dstLabel);

  return 1;
}
/*********************************************************************************/
int rotateAndFlipWholeImages(std::vector<cv::Mat>& images, size_t n)
{
  for (size_t i = 0; i < n; i++)
  {
    cv::Mat temp_90CW; // rotate 90 degrees clockwise
    cv::transpose(images[i], temp_90CW);
    cv::flip(temp_90CW, temp_90CW, 1);
    images.push_back(temp_90CW);

    cv::Mat temp_90CCW; // rotate 90 degrees counter-clockwise
    cv::transpose(images[i], temp_90CCW);
    cv::flip(temp_90CCW, temp_90CCW, 0);
    images.push_back(temp_90CCW);

    cv::Mat temp_Ver; // filp vertically
    cv::flip(images[i], temp_Ver, 0);
    images.push_back(temp_Ver);

    cv::Mat temp_Hor; // filp horizontally
    cv::flip(images[i], temp_Hor, 1);
    images.push_back(temp_Hor);

    cv::Mat temp_VH; // flip v + h at once (same as rotate 180 degrees)
    cv::flip(images[i], temp_VH, -1);
    images.push_back(temp_VH);
  }

  return 1;
}
/*********************************************************************************/
int manipulateRGBLabelAreaIntensity(std::vector<cv::Mat>& images, std::vector<cv::Mat>& labels,
                              int amount, int times, size_t n, std::vector<int*> labelCoords)
{
  int value;
  int x, y, w, h;
  for (size_t i = 0; i < n; i++)
  {
    cv::Mat imgGray;
    cvtColor(images[i], imgGray, cv::COLOR_BGR2GRAY);
    for (int t = 1; t <= times; t++)
    {
      cv::Mat tempGray, tempColor, labelcpy;
      value = amount * t;
      imgGray.copyTo(tempGray);
      labels[i].copyTo(labelcpy);
      for (size_t j = 0; j < labelCoords.size(); j++) {
        x = labelCoords[j][0];
        y = labelCoords[j][1];
        w = labelCoords[j][2];
        h = labelCoords[j][3];
        for (int r = y; r <y+h; r++)
        {
          for (int c = x; c < x+w; c++)
          {
            tempGray.at<uchar>(r,c) = cv::saturate_cast<uchar>(imgGray.at<uchar>(r,c) + value);
          }
        }
      }
      cvtColor(tempGray, tempColor, cv::COLOR_GRAY2BGR);

      images.push_back(tempColor);
      labels.push_back(labelcpy);
    }
  }
  return 1;
}
/*********************************************************************************/
int manipulateRGBImagesIntensity(std::vector<cv::Mat>& images, std::vector<cv::Mat>& labels,
                              int amount, int times, size_t n)
{
  int value;
  for (size_t i = 0; i < n; i++)
  {
    cv::Mat imgGray;
    cvtColor(images[i], imgGray, cv::COLOR_BGR2GRAY);
    for (int t = 1; t <= times; t++)
    {
      cv::Mat tempGray, tempColor, labelcpy;
      value = amount * t;
      tempGray = Mat::zeros(imgGray.size(),imgGray.type());
      labels[i].copyTo(labelcpy);
      for (int r = 0; r < imgGray.rows; r++)
      {
        for (int c = 0; c < imgGray.cols; c++)
        {
          tempGray.at<uchar>(r,c) = cv::saturate_cast<uchar>(imgGray.at<uchar>(r,c) + value);
        }
      }
      cvtColor(tempGray, tempColor, cv::COLOR_GRAY2BGR);

      images.push_back(tempColor);
      labels.push_back(labelcpy);
    }
  }
  return 1;
}
/*********************************************************************************/
int manipulateGrayImagesIntensity(std::vector<cv::Mat>& images, std::vector<cv::Mat>& labels,
                              int amount, int times, size_t n)
{
  cv::Mat temp, labelcpy;
  int value;
  for (size_t i = 0; i < n; i++)
  {
    for (int t = 1; t <= times; t++)
    {
      value = amount * t;
      temp = Mat::zeros(images[i].size(),images[i].type());
      labels[i].copyTo(labelcpy);
      for (int r = 0; r < images[i].rows; r++)
      {
        for (int c = 0; c < images[i].cols; c++)
        {
          temp.at<uchar>(r,c) = cv::saturate_cast<uchar>(images[i].at<uchar>(r,c) + value);
        }
      }
      images.push_back(temp);
      labels.push_back(labelcpy);
    }
  }
  return 1;
}
/*********************************************************************************/
int classColorMatch(std::string& class_name, int& R, int& G, int& B)
{
  if (!class_name.compare("belongs_to_garment")) {
    // green
    R = 128;
    G = 192;
    B = 0;
  }
  else if (!class_name.compare("foreign_object")){
    // yellow
    R = 255;
    G = 230;
    B = 109;
  }
  else if (!class_name.compare("penlike_object")){
    // red
    R = 204;
    G = 0;
    B = 0;
  }
  else if (!class_name.compare("needle_hairpin")){
    // blue
    R = 91;
    G = 192;
    B = 235;
  }
  else if (!class_name.compare("paperclip")){
    // pink
    R = 255;
    G = 162;
    B = 179;
  }
  else if (!class_name.compare("rubber_band")){
    // grey
    R = 204;
    G = 204;
    B = 204;
  }
  else {
    std::cout << "Whoops! New class!" << std::endl;
    return 0;
  }
  return 1;
}
/*********************************************************************************/
int crop(cv::Rect& roiCrop, int cols, int rows, int x, int y, int w, int h)
{
  int x1, y1, x2, y2;

  if (x >= 0 && y >= 0 && x+w <= cols && y+h <= rows) // check if the give coordinates are right
  {
    if (x-w <= 0) x1 = 0;
    else x1 = x-w;

    if (y-h <= 0) y1 = 0;
    else y1 = y-h;

    if (x+w+w >= cols) x2 = cols;
    else x2 = x+w+w;

    if (y+h+h >= rows) y2 = rows;
    else y2 = y+h+h;

    roiCrop = Rect(Point(x1, y1), Point(x2, y2));

    return 1;
  }
  else return 0;
}
/*********************************************************************************/
int checkLabelCoordinates(cv::Mat image, int x, int y, int w, int h)
{
  if (x >= 0 && y >= 0 && x+w <= image.cols && y+h <= image.rows) return 1;
  else return 0;
}
