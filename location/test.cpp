#include <iostream>
#include <opencv2/opencv.hpp>
#include <numeric>

void CalLocationHelper(uchar *src,int wd,int ht){
    int *sum1 = new int[ht];
    memset(sum1,0,ht*sizeof(int));
    for(int i=0;i<wd*ht;++i){
       if(i%ht>400 && i%ht<ht-15){
            //std::cout<<src[i]<<" ";
            sum1[i%ht]+=std::accumulate(src+i,src+i+15,0);
        }
    }

    for(int i=0;i<ht;++i){
        std::cout<<sum1[i]<<" ";
    }

    delete []sum1;
}

void CalLocation(uchar **src,int wd,int ht,int num){
    for(int i=0;i<num;++i){
        CalLocationHelper(src[i],wd,ht);
    }
}

int main(){
    const int num = 1,wd=500,ht=860;
    cv::Mat img;
    uchar **src = new uchar*[num];
    for(int index=0;index<num;++index){
        src[index]=new uchar[wd*ht];
        img = cv::imread("125636/img"+std::to_string(index+1)+".bmp",0);
        assert(!img.empty());
        for(int i=0;i<wd*ht;++i){
            *(src[index]+i)=img.data[i];
        }
    }

    CalLocation(src,wd,ht,num);

    return 0;
}