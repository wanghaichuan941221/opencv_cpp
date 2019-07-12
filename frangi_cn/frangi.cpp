#include "frangi.h"

//计算图像在横向的gradient
void FrangiFilterImpl::Gradient_x(const cv::Mat &src, cv::Mat &Dx) {
	const int wd = src.rows, ht = src.cols;
	for (int i = 0; i < wd; ++i) {
		for (int j = 0; j < ht; ++j) {
			if (j >= 1 && j < ht - 1) {
				Dx.at<float>(i, j) = (src.at<float>(i, j + 1) - src.at<float>(i, j - 1)) / 2.0;
			}
			else if (j == 0) {
				Dx.at<float>(i, j) = (src.at<float>(i, j + 1) - src.at<float>(i, j));
			}
			else if (j == ht - 1) {
				Dx.at<float>(i, j) = (src.at<float>(i, j) - src.at<float>(i, j - 1));
			}
		}
	}
}

//计算图像在纵向的gradient
void FrangiFilterImpl::Gradient_y(const cv::Mat &src, cv::Mat &Dy) {
	const int wd = src.rows, ht = src.cols;
	for (int i = 0; i < wd; ++i) {
		for (int j = 0; j < ht; ++j) {
			if (i >= 1 && i < wd - 1) {
				Dy.at<float>(i, j) = (src.at<float>(i + 1, j) - src.at<float>(i - 1, j)) / 2.0;
			}
			else if (i == 0) {
				Dy.at<float>(i, j) = (src.at<float>(i + 1, j) - src.at<float>(i, j));
			}
			else if (i == wd - 1) {
				Dy.at<float>(i, j) = (src.at<float>(i, j) - src.at<float>(i - 1, j));
			}
		}
	}

}

////计算图像hessian数组的值
void FrangiFilterImpl::Hessian_matrix(const cv::Mat &src, float sigma, cv::Mat &Dxx, cv::Mat &Dxy, cv::Mat &Dyy) {
	const int wd = src.rows, ht = src.cols;

	int n_kern_x = 2 * round(3 * sigma) + 1;
	int n_kern_y = n_kern_x;

	cv::Mat kern = cv::getGaussianKernel(n_kern_x, sigma, CV_32F);
	cv::Mat kern1 = kern*kern.t();
	cv::Mat dst;
	cv::filter2D(src, dst, -1, kern1, cv::Point(-1, -1), 0, cv::BORDER_REFLECT);

	cv::Mat Dx(wd, ht, CV_32F), Dy(wd, ht, CV_32F);
	Gradient_x(dst, Dx);
	Gradient_y(dst, Dy);

	Gradient_x(Dx, Dxx);
	Gradient_x(Dy, Dxy);
	Gradient_y(Dy, Dyy);
}


//计算特征值
void FrangiFilterImpl::Eig2Image(cv::Mat Dxx, cv::Mat Dxy, cv::Mat Dyy, cv::Mat & lambda1, cv::Mat & lambda2)
{
	cv::Mat temp1;
	cv::sqrt((Dxx + Dyy).mul(Dxx + Dyy) - 4 * (Dxx.mul(Dyy) - Dxy.mul(Dxy)), temp1);

	lambda1 = (Dxx + Dyy) / 2 + temp1 / 2;
	lambda2 = (Dxx + Dyy) / 2 - temp1 / 2;

}


void FrangiFilterImpl::processImage(const cv::Mat & src, cv::Mat & dst)
{
	beta1 = 2 * beta1*beta1;
	beta2 = 2 * beta2*beta2;
	const int wd = src.rows, ht = src.cols;

	std::vector<cv::Mat> filter_array;
	for (float sigma =sigma_start; sigma < sigma_end; sigma += sigma_step) {
		cv::Mat Dxx(wd, ht, CV_32F), Dxy(wd, ht, CV_32F), Dyy(wd, ht, CV_32F);
		Hessian_matrix(src, sigma, Dxx, Dxy, Dyy);

		Dxx = (sigma*sigma)*Dxx;
		Dxy = (sigma*sigma)*Dxy;
		Dyy = (sigma*sigma)*Dyy;

		cv::Mat lambda1(wd, ht, CV_32F), lambda2(wd, ht, CV_32F);
		Eig2Image(Dxx, Dxy, Dyy, lambda1, lambda2);

		lambda1.setTo(1e-10, lambda1 == 0);
		cv::Mat rb = lambda2.mul(1.0 / lambda1);
		rb = rb.mul(rb);
		cv::Mat s2 = lambda1.mul(lambda1) + lambda2.mul(lambda2);

		cv::Mat temp1, temp2;
		cv::exp(-rb / beta1, temp1);
		cv::exp(-s2 / beta2, temp2);
		cv::Mat filtered = temp1.mul(cv::Mat::ones(wd, ht, CV_32F) - temp2);

		if (black_ridge) {
			filtered.setTo(0, lambda1 < 0);
		}
		else {
			filtered.setTo(0, lambda1 > 0);
		}
		filter_array.push_back(filtered);
	}

	dst = filter_array[0].clone();
	for (int i = 1; i < filter_array.size(); ++i) {
		cv::max(filter_array[i], dst, dst);
	}
}

void FrangiFilterImpl::setSigma_start(float sigma_start_)
{
	if (sigma_start_ <= 0)sigma_start_ = 0.001;
	sigma_start = sigma_start_;
}

void FrangiFilterImpl::setSigma_end(float sigma_end_)
{
	if (sigma_end_ <= 0)sigma_end_ = 0.001;
	sigma_end = sigma_end_;
}

void FrangiFilterImpl::setSigma_step(float sigma_step_)
{
	if (sigma_step_ <= 0)sigma_step_ = 0.001;
	sigma_step = sigma_step_;
}

void FrangiFilterImpl::setBeta1(float beta1_)
{
	if (beta1_ <= 0)beta1_ = 0.001;
	beta1 = beta1_;
}

void FrangiFilterImpl::setBeta2(float beta2_)
{
	if (beta2_ <= 0)beta2_ = 0.001;
	beta2 = beta2_;
}

void FrangiFilterImpl::setBlack_ridge(bool black_ridge_)
{
	black_ridge = black_ridge_;
}


/*
    scale_start: float,minimal sigma
	scale_end: float,maximum sigma
    scale_step : float, optional
        Step size between sigmas.
    beta = beta1 : float, optional
        Frangi correction constant that adjusts the filter's
        sensitivity to deviation from a blob-like structure.
    gamma = beta2 : float, optional
        Frangi correction constant that adjusts the filter's
        sensitivity to areas of high variance/texture/structure.
    black_ridges : boolean, optional
        When True (the default), the filter detects black ridges; when
        False, it detects white ridges.
 */
cv::Ptr<FrangiFilter> createFrangiFilter(float sigma_start_, float sigma_end_, float sigma_step_, float beta1_, float beta2_, bool black_ridge_)
{
	cv::Ptr<FrangiFilter> frangiPointer = cv::makePtr<FrangiFilterImpl>();
	frangiPointer->setSigma_start(sigma_start_);
	frangiPointer->setSigma_end(sigma_end_);
	frangiPointer->setSigma_step(sigma_step_);
	frangiPointer->setBeta1(beta1_);
	frangiPointer->setBeta2(beta2_);
	frangiPointer->setBlack_ridge(black_ridge_);
	
	return frangiPointer;
}
