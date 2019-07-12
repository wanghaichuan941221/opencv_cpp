## frangi滤波的实现

### 代码解释

* 在frangi.cpp,frangi.h里面包括frangi滤波代码的实现,main.cpp里面是测试代码
* 使用的时候可以用createFrangiFilter函数初始化参数，然后用processImage获得输出图像
* processImage的输入和输出均为float类型


### 代码实现参考资料
* Frangi, A. F., Niessen, W. J., Vincken, K. L., & Viergever, M. A.
        (1998,). Multiscale vessel enhancement filtering. In International
        Conference on Medical Image Computing and Computer-Assisted
        Intervention (pp. 130-137). Springer Berlin Heidelberg.
        :DOI:`10.1007/BFb0056195`
* Kroon, D. J.: Hessian based Frangi vesselness filter.
* Ellis, D. G.: https://github.com/ellisdg/frangi3d/tree/master/frangi

