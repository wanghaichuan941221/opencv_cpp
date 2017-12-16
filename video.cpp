#include <iostream>
//#include "opencv2/opencv.hpp"
#include <fstream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

int g_slider_position = 0;
int g_run = 1,g_dontset = 0;//start out in single step mode
cv::VideoCapture g_cap;

void onTrackbarSlider(int pos,void*){
  g_cap.set(cv::CAP_PROP_POS_FRAMES,pos);
  if(!g_dontset)g_run=1;
  g_dontset = 0;
}

int main(int argc, char const **argv) {
  cv::namedWindow("example",cv::WINDOW_AUTOSIZE);
  g_cap.open(std::string(argv[1]));

  int frames = (int)g_cap.get(cv::CAP_PROP_FRAME_COUNT);
  int tmpw = (int)g_cap.get(cv::CAP_PROP_FRAME_WIDTH);
  int tmph = (int)g_cap.get(cv::CAP_PROP_FRAME_HEIGHT);

  std::cout << "video has " << frames << "frames of dimensions("
   <<tmpw <<"," << tmph << ")" << '\n';

  cv::createTrackbar("Position","example",&g_slider_position,frames,onTrackbarSlider);

  cv::Mat frame;
  for(;;){
    if(g_run != 0){
      g_cap>>frame;
      if(frame.empty())break; //run out of the frame
      int current_pos = (int)g_cap.get(cv::CAP_PROP_POS_FRAMES);
      g_dontset = 1;

      cv::setTrackbarPos("Position","example",current_pos);
      cv::imshow("example",frame);
      g_run = 1;
    }
    char c = (char)cv::waitKey(10);
    if(c == 's'){g_run = 1;std::cout << "single mode, run=" <<g_run<< '\n';}
    if(c == 'r'){g_run = -1;std::cout << "run mode, run ="<<g_run << '\n';}
    if(c==27)break;
  }
  return 0;
}
