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
int imageProcessing(cv::Mat image, cv::Mat label, std::vector<int*> labelCoords, std::vector<int*> labelRGBs,
  int& _totalImagesSaved, int& _totalLabelsSaved, std::string folderToSaveImages, std::string folderToSaveLabels)
{
  if (!image.data) return -1;

  std::vector<cv::Mat> images;
  std::vector<cv::Mat> labels;

  images.push_back(image);
  labels.push_back(label);

  for (int i = 0; i < 3; i++) FlipLabelAreaImages(images, labels, labelCoords, 1, i);

  // scaleAndReflect(images, labels, x, y, w, h);

  rotateAndFlipWholeImage(images, images.size());
  cout << "rotate? " << endl;
  rotateAndFlipWholeImage(labels, labels.size());
  cout << "rotate? " << endl;
  cout << "images num: " << images.size() << endl;
  cout << "labels num: " << labels.size() << endl;

  //manipulateImagesIntensity(images, labels, 20, 3, images.size());
  //cout << "intensity? " << endl;

  saveImages(folderToSaveImages, images, _totalImagesSaved);
  saveImages(folderToSaveLabels, labels, _totalLabelsSaved);

  return 1;
}
/*********************************************************************************/
int rotateAndFlipWholeImage(std::vector<cv::Mat>& images, size_t n)
{
  for (size_t i = 0; i < n; i++) {
    cv::Mat temp_90CW;
    cv::transpose(images[i], temp_90CW);
    cv::flip(temp_90CW, temp_90CW, 1);
    images.push_back(temp_90CW);

    cv::Mat temp_180CW; // same as flip v + h at once
    cv::flip(images[i], temp_180CW, -1);
    images.push_back(temp_180CW);

    cv::Mat temp_270CW;
    cv::transpose(images[i], temp_270CW);
    cv::flip(temp_270CW, temp_270CW, 0);
    images.push_back(temp_270CW);

    cv::Mat temp_Ver;
    cv::flip(images[i], temp_Ver, 0);
    images.push_back(temp_Ver);

    cv::Mat temp_Hor; // filp horizontally
    cv::flip(images[i], temp_Hor, 1);
    images.push_back(temp_Hor);
  }


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
      for (int r = 0; r < images[i].rows; r++) {
        for (int c = 0; c < images[i].cols; c++) {
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
/*********************************************************************************
int scaleAndReflect(int scaleAndReflect(std::vector<cv::Mat>& images, std::vector<cv::Mat>& labels,
  std::vector<int*> labelCoords, std::vector<int*> labelRGBs)
{
  cv::Mat src, dst, temp;
  cv::Mat map_x, map_y;
  images[0].copyTo(src);
  labels[0].copyTo(temp);
  cout << "in here" << endl;

  /// Create dst, map_x and map_y with the same size as src:
  dst.create( src.size(), src.type() );
  map_x.create( src.size(), CV_32FC1 );
  map_y.create( src.size(), CV_32FC1 );

  cout << "after this" << endl;

  for (int r = 0; r < src.rows; r++) {
    for (int c = 0; c < src.cols; c++) {
      if (r >= y && r < y+h && c >= x && c < x+w) {
        if ( c > x+w*0.25 && c < x+w*0.75 && r > y+h*0.25 && r < y+h*0.75 ) {
          map_x.at<float>(r,c) = 2*c - x-w*0.5;
          map_y.at<float>(r,c) = 2*r - y-h*0.5;
        }
        else {
          map_x.at<float>(r,c) = src.cols - c ;
          map_y.at<float>(r,c) = src.rows - r ;
          temp.at<Vec3b>(Point(c,r)) = Vec3b(0,0,0);
        }
      }
      else {
        map_x.at<float>(r,c) = c;
        map_y.at<float>(r,c) = r;
      }
    }
  }
  cout << "and here" << endl;
  rectangle(temp,Point(x+w/4,y+h/4),Point(x+w*3/4,y+h*3/4),Scalar(198,228,225),3,8,0);
  cout << "what about this" << endl;
  remap( src, dst, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0,0,0) );
  cout << "what about this: remap? " << endl;
  images.push_back(dst);
  display(dst);
  labels.push_back(temp);
  return 1;
}
*/
/*********************************************************************************/
int FlipLabelAreaImages(std::vector<cv::Mat>& images, std::vector<cv::Mat>& labels, std::vector<int*> labelCoords, size_t n, int flipType)
{
  int x, y, w, h;
  for (size_t i = 0; i < n; i++) {
    cv::Mat src, dst, temp;
    cv::Mat map_x, map_y;
    images[i].copyTo(src);
    labels[i].copyTo(temp);
    dst.create(src.size(), src.type());
    map_x.create(src.size(), CV_32FC1);
    map_y.create(src.size(), CV_32FC1);

    for (int r = 0; r < src.rows; r++) {
      for (int c = 0; c < src.cols; c++) {
        map_x.at<float>(r,c) = c;
        map_y.at<float>(r,c) = r;
      }
    }
    for (size_t j = 0; j < labelCoords.size(); j++) {
      x = labelCoords[j][0];
      y = labelCoords[j][1];
      w = labelCoords[j][2];
      h = labelCoords[j][3];
      for (int r = y; r <y+h; r++) {
        for (int c = x; c < x+w; c++) {
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
    remap( src, dst, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT);
    images.push_back(dst);
    labels.push_back(temp);

  }
  return 1;
}
