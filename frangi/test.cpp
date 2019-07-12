// test.cpp : 定义控制台应用程序的入口点。
//


#include <opencv2/opencv.hpp>
#include "frangi.h"
// #if _DEBUG
// #pragma comment(lib,"D://disc/FrangiLib/x64/Debug/FrangiLib.lib")
// #else
// #pragma comment(lib,"D://disc/FrangiLib/x64/Release/FrangiLib.lib")
// #endif // _DEBUG


// //定义导出函数
// #if (defined WIN32 || defined _WIN32 || defined WINCE) && defined FRANGILIB_EXPORTS
// # define FRANGI_EXPORTS __declspec(dllexport)
// #else
// # define FRANGI_EXPORTS
// #endif

// class FRANGI_EXPORTS FrangiFilter {
// private:
// 	float sigma_start, sigma_end, sigma_step;
// 	float beta1, beta2;
// 	bool black_ridge;
// public:
// 	//FrangiFilter();

// 	virtual void processImage(const cv::Mat &src, cv::Mat &dst) = 0;
// 	virtual void setSigma_start(float sigma_start_) = 0;
// 	virtual void setSigma_end(float sigma_end_) = 0;
// 	virtual void setSigma_step(float sigma_step_) = 0;
// 	virtual void setBeta1(float beta1_) = 0;
// 	virtual void setBeta2(float beta2_) = 0;
// 	virtual void setBlack_ridge(bool black_ridge_) = 0;

// };

// //导出createFrangiFilter函数
// FRANGI_EXPORTS cv::Ptr<FrangiFilter> createFrangiFilter(float sigma_start_ = 1.0, float sigma_end_ = 10.0, float sigma_step_ = 2.0, float beta1_ = 0.5, float beta2_ = 15.0, bool black_ridge_ = true);



int main()
{
	
	cv::Mat img = cv::imread("D://disc/data/1/gabor.bmp", 0);
	cv::Mat src;
	img.convertTo(src, CV_32F);
	const int wd = img.rows, ht = img.cols;

	cv::Mat res(img.size(), CV_32F);
	cv::Ptr<FrangiFilter> filter1 = createFrangiFilter(0.5, 1.0, 0.5);
	filter1->processImage(255.0-src, res);

	double minValue, maxValue;
	cv::minMaxLoc(res, &minValue, &maxValue);

	cv::Mat dst1 = (255 * (res - minValue) / (maxValue - minValue));
	cv::Mat dst;
	dst1.convertTo(dst, CV_8U);
	cv::Mat thresh;
	cv::threshold(dst, thresh, 1, 255, cv::THRESH_BINARY);

	cv::imwrite("D://disc/data/1/thresh.jpg", thresh);

    return 0;
}

