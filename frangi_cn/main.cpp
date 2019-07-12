#include "frangi.h"

int main()
{
	
	cv::Mat img = cv::imread("image.bmp", 0);
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

	cv::imwrite("thresh.bmp", thresh);

    return 0;
}
