#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string> 
#include <opencv4/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv4/opencv2/imgproc/imgproc.hpp"
#include "opencv4/opencv2/videoio/videoio.hpp"
#include "opencv4/opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;


/* Memory errors: double free or corruption (out), somewhere in matrix assignment
*/

Mat convolution(Mat *gFrm, Mat *cFrm, float Krnl[][3]){
    int frmSize[2] = {gFrm->rows, gFrm->cols};
    int krnlSize[2] = {3,3};
    int center = round(krnlSize[0]/2);

    //Apply kernel iteratively to the image
    for(int i=0;i<frmSize[0];++i){
        for(int j=0;j<frmSize[1];++j){
            float accum = 0;
            for(int m=0;m<krnlSize[0];m++){
                for(int n=0;n<krnlSize[1];n++){
                    int x, y;
                    //Handle kernel assignment
                    if(m < center){ x = i - (m+1); }
                    else if(m == center){ x = i; }
                    else if(m > center){ x = i + ((m+1) - center); }

                    if(n < center){ y = j - (n+1); }
                    else if(n == center){ y = j; }
                    else if(n > center){ y = j + ((n+1) - center); }

                    if (x>0 && y>0 && x<frmSize[0] && y<frmSize[1]){
                        accum = accum + (Krnl[m][n] * (float)gFrm->at<uchar>(x,y));
                    }
                }
            }

            //Preservation of negatives for laplace cases.
            if(accum < 0){ accum += 128; }; //for sharpen
            //Clamp leftover values outside pixel range (keep for all kernels)
            accum = max(0, min((int)accum, 255));
            
            //Assign new value to target pixel
            cFrm->at<uchar>(i,j) = (int16_t)accum;
        }
    } 
    
    return *cFrm;
}

int main(){
    int width = 640;
    int height = 480;

    float sobelHorizontal[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};
    float sobelVertical[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
    float sharpen[3][3] = {{-1,-1,-1},{-1, 9,-1},{-1,-1,-1}};
    float lowPass[3][3] = {{0.5,0.5,0.5},{0.5,0.5,0.5},{0.5,0.5,0.5}};

    VideoCapture capture(0);
    cout << capture.isOpened() << endl;
    
    if (!capture.isOpened()){
        throw std::runtime_error("Error");
    }

    namedWindow("Window");
    
    int count = 0;
    while(true) {

        //Read and show frame of video
        Mat frame;
        capture >> frame;
        resize(frame, frame, Size(width,height));
        imshow("frame", frame);

        //Convert image to grayscale
        Mat grayFrame;
        cvtColor(frame, grayFrame, COLOR_BGR2GRAY);
        Mat *gFrmPtr = &grayFrame;
        imshow("gray frame", grayFrame);

        //Matrix of zeroes to apply convolution results
        Mat convFrame;
        convFrame = grayFrame.clone();
        Mat *cFrmPtr = &convFrame;

        for(int i=0; i<height; i++){
            for(int j=0; j<width; j++){
                convFrame.at<uchar>(i,j) = 0;
            }
        }

        //Perform convolution and display, define which kernel matrix to use
        Mat outFrame = convolution(gFrmPtr, cFrmPtr, sharpen);

        imshow("Convolved frame", outFrame);

        if (waitKey(30) >= 0){
            break;
        } 
    }

    return 0;
}



