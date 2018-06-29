#include "imageProcessing.h"

extern const std::string SOURCEFOLDER = "./source/";
extern const std::string INFERENCEFOLDER = "./inference/";

int blend(std::string imageName,std::string taskName)
{
  cv::Mat src1, src2, dst;

  src1 = imread(SOURCEFOLDER+"foreign_object/"+imageName, CV_LOAD_IMAGE_COLOR);
  src2 = imread(INFERENCEFOLDER+taskName+"/result_with_foreign_object/layers/"+imageName, CV_LOAD_IMAGE_COLOR);

  if( !src1.data ) {
    std::cout << "Error loading src1" << std::endl;
    return -1;
  }
  if( !src2.data ) {
    std::cout << "Error loading src2" << std::endl;
    return -1;
  }

  cv::add(src1,src2,dst);

  // namedWindow("Linear Blend", WINDOW_NORMAL);
  // imshow("Linear Blend", dst);
  // waitKey(0);
  // destroyAllWindows();

  cv::imwrite(INFERENCEFOLDER+taskName+"/result_with_foreign_object/"+imageName, dst);
  return 1;
}

int main( int argc, char** argv )
{
  // with pen
  /*
  std::string taskName = "rgb_full_rec_7classes_12227";
  blend("0_1_15192786528_3.png",taskName);
  blend("0_15012290272_3.png",taskName);
  blend("1_0_15064063146_3.png",taskName);
  */


  // without pen
  /*
  std::string taskName = "rgb_full_rec_7classes_12227";
  blend("0_15016541618_3.png",taskName);
  blend("1_0_15000156934_3.png",taskName);
  blend("1_0_15090865762_3.png",taskName);
  */

  // with paperclip
  /*
  std::string taskName = "rgb_full_rec_7classes_12227";
  blend("0_14658715159_1.png",taskName);
  blend("0_14660507141_1.png",taskName);
  blend("0_14660502260_1.png",taskName);
  */


  // with foreign_object

  std::string taskName = "rgb_full_rec_7classes_12227";
  blend("1_0_15191251770_3.png",taskName);
  blend("0_1_15239536996_3.png",taskName);
  blend("0_1_15221329292_3.png",taskName);



  return 1;

}
