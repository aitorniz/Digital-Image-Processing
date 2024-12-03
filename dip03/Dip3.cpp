//============================================================================
// Name    : Dip3.cpp
// Author      : Ronny Haensch, Andreas Ley, Simon Matern
// Version     : 3.0
// Copyright   : -
// Description : 
//============================================================================

#include "Dip3.h"

#include <stdexcept>
#include <cmath>

namespace dip3 {

const char * const filterModeNames[NUM_FILTER_MODES] = {
    "FM_SPATIAL_CONVOLUTION",
    "FM_FREQUENCY_CONVOLUTION",
    "FM_SEPERABLE_FILTER",
    "FM_INTEGRAL_IMAGE",
};



/**
 * @brief Generates 1D gaussian filter kernel of given size
 * @param kSize Kernel size (used to calculate standard deviation)
 * @returns The generated filter kernel
 */
cv::Mat_<float> createGaussianKernel1D(int kSize){

    // TO DO !!!
    cv::Mat_<float> kernel = cv::Mat::zeros(1, kSize, CV_32F);
    if (kSize % 2 == 0) {
        std::cerr << "Kernel size must be odd!" << std::endl;
        return {};
    }
    float sigma = kSize/5.0f;
    int center = kSize/2.0f;
    float sum = 0.0f;
    for (int i = 0; i < kSize; i++) {
        float numerator = (i - center);
        kernel.at<float>(0,i) = 1.0f/(std::sqrt(2.0f*M_PI)*sigma) * std::exp(-0.5f*(numerator*numerator/sigma*sigma)); //std::pow(sigma,2)));
        sum += kernel(0,i);
    }
    for (int i = 0; i < kSize; i++) {
        kernel.at<float>(0,i)/=sum;
    }
    return kernel;
}
/**
 * @brief Generates 2D gaussian filter kernel of given size
 * @param kSize Kernel size (used to calculate standard deviation)
 * @returns The generated filter kernel
 */
cv::Mat_<float> createGaussianKernel2D(int kSize){

    // TO DO !!!
    cv::Mat_<float> kernel = cv::Mat::zeros(kSize, kSize, CV_32F);
    if (kSize % 2 == 0) {
        std::cerr << "Kernel size must be odd!" << std::endl;
        return {};
    }
    float sigma = kSize/5.0f;
    int center = kSize/2.0f;
    float sum = 0.0f;
    for (int i = 0; i < kSize; i++) {
        for (int j = 0; j < kSize; j++) {
            float x = (i - center);
            float y = (j - center);
            kernel.at<float>(i,j) = 1.0f/(2.0f*M_PI*sigma*sigma) * std::exp(-0.5f*(x*x+y*y/sigma*sigma)); //std::pow(sigma,2)));
            sum += kernel(i,j);
        }
    }
    for (int i = 0; i < kSize; i++) {
        for (int j = 0; j < kSize; j++) {
            kernel.at<float>(i,j)/=sum;
        }
    }
    return kernel;
}

/**
 * @brief Performes a circular shift in (dx,dy) direction
 * @param in Input matrix
 * @param dx Shift in x-direction
 * @param dy Shift in y-direction
 * @returns Circular shifted matrix
 */
cv::Mat_<float> circShift(const cv::Mat_<float>& in, int dx, int dy){

   // TO DO !!!
   int rows = in.rows;
   int cols = in.cols;
   if (dx < 0) {
        dx += rows;
   }
   dx = dx % rows;
   if (dy < 0) {
        dy += cols;
   }
   dy = dy % cols;

   cv::Mat_<float> circShiftMat(rows,cols,CV_32F);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int newRow = (i + dx) % rows;
            int newCol = (j + dy) % cols;
            circShiftMat.at<float>(newRow,newCol) = in(i,j);
        }
   }
   return circShiftMat;
}


/**
 * @brief Performes convolution by multiplication in frequency domain
 * @param in Input image
 * @param kernel Filter kernel
 * @returns Output image
 */
cv::Mat_<float> frequencyConvolution(const cv::Mat_<float>& in, const cv::Mat_<float>& kernel){

   // TO DO !!!
    int rows = cv::getOptimalDFTSize(in.rows+kernel.rows-1);
    int cols = cv::getOptimalDFTSize(in.cols+kernel.cols-1);

    //CV_Assert(in.type() == CV_32F || in.type() == CV_64F);
    //CV_Assert(in.type() == kernel.type());

    cv::Mat_<float> in_padding;
    cv::copyMakeBorder(in,in_padding,0,rows - in.rows, 0, cols-in.cols,cv::BORDER_CONSTANT,cv::Scalar::all(0));

    cv::Mat_<float> kernel_padding;
    cv::copyMakeBorder(kernel,kernel_padding,0,rows - kernel.rows, 0, cols-kernel.cols,cv::BORDER_CONSTANT,cv::Scalar::all(0));

    cv::Mat_<float> shiftedKernel = circShift(kernel_padding,-kernel.cols/2,-kernel.rows/2);

    cv::Mat_<float> freq_in;
    cv::Mat_<float> freq_kernel;

    cv::dft(in_padding,freq_in,0);
    cv::dft(shiftedKernel,freq_kernel,0);

    cv::Mat_<float> dftProduct;
    cv::mulSpectrums(freq_in,freq_kernel,dftProduct,0);

    cv::Mat_<float> output;
    cv::dft(dftProduct,output,cv::DFT_INVERSE + cv::DFT_SCALE);

    output = output(cv::Rect(0,0,in.cols,in.rows));

   return output;
}


/**
 * @brief  Performs UnSharp Masking to enhance fine image structures
 * @param in The input image
 * @param filterMode How convolution for smoothing operation is done
 * @param size Size of used smoothing kernel
 * @param thresh Minimal intensity difference to perform operation
 * @param scale Scaling of edge enhancement
 * @returns Enhanced image
 */
cv::Mat_<float> usm(const cv::Mat_<float>& in, FilterMode filterMode, int size, float thresh, float scale){
   // TO DO !!!
   //std::cout << "it starts." << std::endl;
   // use smoothImage(...) for smoothing
   cv::Mat_<float> smoothedImg = smoothImage(in, size, filterMode);
   if(smoothedImg.empty()){
      throw std::runtime_error("smoothing failed");
   }

   //Compute the mask
   cv::Mat_<float> mask = in - smoothedImg;

   //Apply threshold to the mask
   for(int i=0; i<mask.rows; i++){
      for(int j=0; j<mask.cols; j++){
         if(std::abs(mask(i, j)) < thresh){
            mask(i, j) = 0.0f;
         }
      }
   }

   //Add scaled mask
   cv::Mat_<float> enhancedImg = in + scale*mask;

   cv::Mat_<float> output;
   //cv::threshold(enhancedImg, output, 1.0f, 1.0f, cv::THRESH_TRUNC);
   //cv::threshold(output, output, 0.0f, 0.0f, cv::THRESH_TOZERO);

   return output;
}


/**
 * @brief Convolution in spatial domain
 * @param src Input image
 * @param kernel Filter kernel
 * @returns Convolution result
 */
cv::Mat_<float> spatialConvolution(const cv::Mat_<float>& src, const cv::Mat_<float>& kernel)
{

   // Hopefully already DONE, copy from last homework
    int kernel_size = kernel.rows;
   int padding_size = kernel_size/2;

   cv::Mat_<float> src_padding;
   cv::copyMakeBorder(src, src_padding, padding_size, padding_size, padding_size, padding_size, cv::BORDER_CONSTANT, cv::Scalar(0));

   cv::Mat_<float> flipped_kernel;
   cv::flip(kernel, flipped_kernel, -1);

   cv::Mat_<float> output(src.size());
   for(int i=padding_size; i<src_padding.rows - padding_size; i++){
        for(int j=padding_size; j<src_padding.cols - padding_size; j++){
            float sum = 0.0f;
            for(int k= -padding_size; k<= padding_size; k++){
                for(int l=-padding_size; l<= padding_size; l++){
                    float pixel = src_padding.at<float>(i+k, j+l);
                    float kernel_value = flipped_kernel.at<float>(k+padding_size, l+padding_size);
                    sum += pixel*kernel_value;
                }
            }
            output.at<float>(i-padding_size, j-padding_size) = sum;

        }
    }
   return output;
}


/**
 * @brief Convolution in spatial domain by seperable filters
 * @param src Input image
 * @param size Size of filter kernel
 * @returns Convolution result
 */
cv::Mat_<float> separableFilter(const cv::Mat_<float>& src, const cv::Mat_<float>& kernel){

   // TO DO !!!
   int kSize = kernel.cols;
   int padSize = kSize/2;

   //Convolution along rows
   cv::Mat_<float> rowFiltered;
   cv::copyMakeBorder(src, rowFiltered, 0, 0, padSize, padSize, cv::BORDER_REFLECT);
   cv::Mat_<float> temp = cv::Mat_<float>::zeros(src.size());
   for(int i=0; i<src.rows; i++){
      for(int j=padSize; j<rowFiltered.cols - padSize; j++){
         float sum = 0.0f;
         for(int k = -padSize; k <= padSize; k++){
            sum += rowFiltered(i, j+k)*kernel(0, k+padSize);
         }
         temp(i, j-padSize) = sum;
      }
   }

   //Concolution along columns
   cv::Mat_<float> colFiltered;
   cv::copyMakeBorder(temp, colFiltered, padSize, padSize, 0, 0, cv::BORDER_REFLECT);
   cv::Mat_<float> output = cv::Mat_<float>::zeros(src.size());
   for(int i=padSize; i<colFiltered.rows; i++){
      for(int j=0; j<temp.cols - padSize; j++){
         float sum = 0.0f;
         for(int k = -padSize; k <= padSize; k++){
            sum += colFiltered(i+k, j)*kernel(0, k+padSize);
         }
         output(i-padSize, j) = sum;
      }
   }

   return output;

}


/**
 * @brief Convolution in spatial domain by integral images
 * @param src Input image
 * @param size Size of filter kernel
 * @returns Convolution result
 */
cv::Mat_<float> satFilter(const cv::Mat_<float>& src, int size){

   // optional

   return src;

}

/* *****************************
  GIVEN FUNCTIONS
***************************** */

/**
 * @brief Performs a smoothing operation but allows the algorithm to be chosen
 * @param in Input image
 * @param size Size of filter kernel
 * @param type How is smoothing performed?
 * @returns Smoothed image
 */
cv::Mat_<float> smoothImage(const cv::Mat_<float>& in, int size, FilterMode filterMode)
{
    switch(filterMode) {
        case FM_SPATIAL_CONVOLUTION: return spatialConvolution(in, createGaussianKernel2D(size));	// 2D spatial convolution
        case FM_FREQUENCY_CONVOLUTION: return frequencyConvolution(in, createGaussianKernel2D(size));	// 2D convolution via multiplication in frequency domain
        case FM_SEPERABLE_FILTER: return separableFilter(in, createGaussianKernel1D(size));	// seperable filter
        case FM_INTEGRAL_IMAGE: return satFilter(in, size);		// integral image
        default: 
            throw std::runtime_error("Unhandled filter type!");
    }
}



}

