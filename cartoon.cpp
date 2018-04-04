#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char const *argv[]) {
  int num_down = 2;//downSampling
  int num_bilateral = 7;//bilateralFilter
  cv::Mat img_rgb = cv::imread(argv[1],1);
  //img_rgb.convertTo(img_rgb,CV_32FC3,1/255.0);
  cv::Mat img_color = img_rgb;

  for(size_t i=0;i<num_down;i++){
    pyrDown(img_color,img_color,cv::Size());
  }

  int d=15;//Diameter of each pixel neighborhood
  double sigmaColor = 80;
  double sigmaSpace = 80;
  size_t i=0;
  cv::Mat img_temp;
  while(i<num_bilateral){
    cv::bilateralFilter(img_color,img_temp,9,9,9);
    img_color = img_temp;
    img_temp = cv::Mat::zeros(cv::Size(img_color.size()),CV_8UC1);
    i++;
  }

  for(size_t i=0;i<num_down;i++){
    pyrUp(img_color,img_color,cv::Size());
  }

  cv::Mat img_gray,img_blur,img_edge,img_edge1,img_cartoon,img_color1;
  cv::cvtColor(img_rgb,img_gray,cv::COLOR_BGR2GRAY);
  cv::medianBlur(img_gray,img_blur,7);
  cv::adaptiveThreshold(img_blur,img_edge,255,CV_ADAPTIVE_THRESH_MEAN_C,
  CV_THRESH_BINARY,3,5);

   cv::cvtColor(img_edge,img_edge1,cv::COLOR_GRAY2BGR);
   cv::resize(img_color,img_color1,cv::Size(img_edge1.size()));
   //std::cout << img_edge1.size() <<img_color1.size() << '\n';
  cv::bitwise_and(img_edge1,img_color1,img_cartoon);
  // cv::putText(img_cartoon,"hello",cv::Point(100,100),//coordinates
  // cv::FONT_HERSHEY_PLAIN,100.0,cv::Scalar(0,0,0),1,2);
  //cv::namedWindow("img_color",cv::WINDOW_NORMAL);
  cv::imwrite("jpg/Cartoon.jpg",img_cartoon);
  //cv::imshow("img_color",img_cartoon);
  //cv::waitKey(5000);
  return 0;
}
