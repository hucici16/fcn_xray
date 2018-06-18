#include "imageProcessing.h"

/*********************************************************************************/
void saveImages(std::string folder, std::vector<cv::Mat> images, int& _totalSamplesSaved)
{
  for (size_t i = 0; i < images.size(); i++) {
    std::stringstream name;
    name << folder << _totalSamplesSaved+1  << ".png";
    cv::imwrite(name.str(), images[i]);
    _totalSamplesSaved++;
    if (_totalSamplesSaved % 100 == 0 ) std::cout << _totalSamplesSaved << std::endl;
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
int imageProcessing(cv::Mat image, cv::Mat label, int& _totalImagesSaved, int& _totalLabelsSaved,
  std::string folderToSaveImages, std::string folderToSaveLabels)
{
  if (!image.data) return -1;

  std::vector<cv::Mat> images;
  std::vector<cv::Mat> labels;

  images.push_back(image);
  labels.push_back(label);

  rotateAndFlipWholeImage(images);
  rotateAndFlipWholeImage(labels);

  manipulateImagesIntensity(images, labels, 20, 3, images.size());

  saveImages(folderToSaveImages, images, _totalImagesSaved);
  saveImages(folderToSaveLabels, labels, _totalLabelsSaved);

  return 1;
}
/*********************************************************************************/
int rotateAndFlipWholeImage(std::vector<cv::Mat>& images)
{
  cv::Mat temp_90CW;
  cv::transpose(images[0], temp_90CW);
  cv::flip(temp_90CW, temp_90CW, 1);
  images.push_back(temp_90CW);

  cv::Mat temp_180CW; // same as flip v + h at once
  cv::flip(images[0], temp_180CW, -1);
  images.push_back(temp_180CW);

  cv::Mat temp_270CW;
  cv::transpose(images[0], temp_270CW);
  cv::flip(temp_270CW, temp_270CW, 0);
  images.push_back(temp_270CW);

  cv::Mat temp_Ver;
  cv::flip(images[0], temp_Ver, 0);
  images.push_back(temp_Ver);

  cv::Mat temp_Hor; // filp horizontally
  cv::flip(images[0], temp_Hor, 1);
  images.push_back(temp_Hor);

  return 1;
}
/*********************************************************************************/
int manipulateImagesIntensity(std::vector<cv::Mat>& images, std::vector<cv::Mat>& labels,
                              int amount, int times, size_t n)
{
  for (int t = 1; t <= times; t++) {
    for (size_t i = 0; i < n; i++) {
      int value = amount * t;
      cv::Mat temp = Mat::zeros(images[i].size(),images[i].type());
      cv::Mat labelcpy;
      labels[i].copyTo(labelcpy);
      // int r = ;
      // int c = ;
      // for all pixels
      for (int r = 0; r < images[i].rows; r++) {
        for (int c = 0; c < images[i].cols; c++) {
          // for (int j = 0; j < 3; j++) {
            temp.at<uchar>(r,c) = cv::saturate_cast<uchar>(images[i].at<uchar>(r,c) *(value));
          // }
        }
      }
      // if (!rejectImageBecauseTooWhiteOrBlack(temp))
      images.push_back(temp);
      labels.push_back(labelcpy);
      // cout << "intensity changed: " << i << endl;
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
int display(cv::Mat image)
{
  namedWindow( "Display window", WINDOW_NORMAL); // Create a window for display.
  imshow( "Display window", image ); // Show our image inside it.
  waitKey(0); // Wait for a keystroke in the window
  return 1;
}
/*********************************************************************************/
int gray(cv::Mat image)
{
 	cv::Mat gray_image;
 	cvtColor( image, gray_image, COLOR_BGR2GRAY );
 	imwrite( "Gray_Image.jpg", gray_image );
 	namedWindow( "Display", WINDOW_NORMAL);
 	namedWindow( "Gray image", WINDOW_NORMAL);
	imshow( "Display", image );
	imshow( "Gray image", gray_image );
 	waitKey(0);
	return 1;
}
/*********************************************************************************/
cv::Mat rotate(cv::Mat src, double angle)
{
  cv::Mat dst;
  Point2f pt(src.cols/2., src.rows/2.);
  cv::Mat r = getRotationMatrix2D(pt, angle, 1.0);
  warpAffine(src, dst, r, Size(src.cols, src.rows));
  return dst;
}
/*********************************************************************************/
int rotateImages(std::vector<cv::Mat>& images, double angle, size_t n)
{
  for (size_t i = 0; i < n; i++) {
    cv::Mat temp;
    Point2f pt(images[i].cols/2., images[i].rows/2.);
    warpAffine(images[i], temp, getRotationMatrix2D(pt, angle, 1.0), Size(images[i].cols, images[i].rows));
    cout << "rotated: " << i << endl;
  }
  return 1;
}
