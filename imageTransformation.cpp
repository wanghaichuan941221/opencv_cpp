#include <opencv2/opencv.hpp>

void imageSmooth(const cv::Mat &image){
  // cv::namedWindow("example in",cv::WINDOW_AUTOSIZE);
  // cv::namedWindow("example out",cv::WINDOW_AUTOSIZE);

  cv::imshow("example in",image);
  cv::Mat out;

  cv::GaussianBlur(image,out,cv::Size(5,5),3,3);
  cv::GaussianBlur(out  ,out,cv::Size(5,5),3,3);

  cv::imshow("example",out);

}

void downSampling(const cv::Mat &image){

  // cv::namedWindow("originalImage",(1200,1200));
  // cv::namedWindow("downSamplingImage",(1200,1200));
  // cv::imshow("originalImage",image);

  cv::Mat downSamplingImage;
  cv::pyrDown(image,downSamplingImage);
  cv::imwrite("/home/haichuan/git/opencv/cpp/jpg/downSampling.jpg",downSamplingImage);
  cv::imshow("downSamplingImage",downSamplingImage);
}

void upSampling(const cv::Mat &image){
  // cv::namedWindow("originalImage",cv::WINDOW_AUTOSIZE);
  // cv::namedWindow("upSamplingImage",cv::WINDOW_AUTOSIZE);
  // cv::imshow("originalImage",image);

  cv::Mat upSamplingImage;
  cv::pyrUp(image,upSamplingImage);
  cv::imwrite("/home/haichuan/git/opencv/cpp/jpg/upSampling.jpg",upSamplingImage);
  cv::imshow("upSamplingImage",upSamplingImage);
}

void cannyEdge(cv::Mat &image_rgb){
  cv::Mat img_cny,img_gray;
  cv::cvtColor(image_rgb,img_gray,cv::COLOR_BGR2GRAY);
  cv::imshow("gray image",img_gray);
  cv::imwrite("/home/haichuan/git/opencv/cpp/png/gray2.png",img_gray);

  cv::Canny(img_gray,img_cny,10,100,3,true);
  cv::imshow("canny image",img_cny);
  cv::imwrite("/home/haichuan/git/opencv/cpp/png/cannyImage.png",img_cny);

}

int main(int argc, const char **argv){
  cv::Mat img = cv::imread(argv[1],-1);
  //imageSmooth(img);
  //downSampling(img);
  //upSampling(img);
  cannyEdge(img);
  cv::waitKey(0);
}
