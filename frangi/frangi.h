#include<opencv2/opencv.hpp>
#include <cmath>

#define PI 3.1415926

// //定义导出函数
// #if (defined WIN32 || defined _WIN32 || defined WINCE) && defined FRANGILIB_EXPORTS
// # define FRANGI_EXPORTS __declspec(dllexport)
// #else
// # define FRANGI_EXPORTS
// #endif

class FrangiFilter {
private:
	float sigma_start, sigma_end, sigma_step;
	float beta1, beta2;
	bool black_ridge;
public:
	//FrangiFilter();
	
	virtual void processImage(const cv::Mat &src,cv::Mat &dst)=0;
	virtual void setSigma_start(float sigma_start_)=0;
	virtual void setSigma_end(float sigma_end_)=0;
	virtual void setSigma_step(float sigma_step_)=0;
	virtual void setBeta1(float beta1_)=0;
	virtual void setBeta2(float beta2_)=0;
	virtual void setBlack_ridge(bool black_ridge_) = 0;

};

//导出createFrangiFilter函数
cv::Ptr<FrangiFilter> createFrangiFilter(float sigma_start_=1.0, float sigma_end_=10.0, float sigma_step_=2.0, float beta1_=0.5, float beta2_=15.0, bool black_ridge_=true);


class FrangiFilterImpl:public FrangiFilter {
private:
	float sigma_start, sigma_end, sigma_step;
	float beta1, beta2;
	bool black_ridge;

	//void Hessian2D(const cv::Mat &src, cv::Mat Dxx, cv::Mat Dxy, cv::Mat Dyy, float sigma);
	void Eig2Image(cv::Mat Dxx, cv::Mat Dxy, cv::Mat Dyy, cv::Mat &lambda1, cv::Mat &lambda2);
	void Gradient_x(const cv::Mat &src, cv::Mat &Dx);
	void Gradient_y(const cv::Mat &src, cv::Mat &Dy);
	void Hessian_matrix(const cv::Mat &src, float sigma, cv::Mat &Dxx, cv::Mat &Dxy, cv::Mat &Dyy);
public:
	virtual void processImage(const cv::Mat &src, cv::Mat &dst) override;
	virtual void setSigma_start(float sigma_start_) override;
	virtual void setSigma_end(float sigma_end_) override;
	virtual void setSigma_step(float sigma_step_) override;
	virtual void setBeta1(float beta1_) override;
	virtual void setBeta2(float beta2_) override;

	virtual void setBlack_ridge(bool black_ridge_) override;
};