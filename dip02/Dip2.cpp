//============================================================================
// Name        : Dip2.cpp
// Author      : Ronny Haensch
// Version     : 2.0
// Copyright   : -
// Description :
//============================================================================

#include "Dip2.h"

namespace dip2 {


/**
 * @brief Convolution in spatial domain.
 * @details Performs spatial convolution of image and filter kernel.
 * @params src Input image
 * @params kernel Filter kernel
 * @returns Convolution result
 */
cv::Mat_<float> spatialConvolution(const cv::Mat_<float>& src, const cv::Mat_<float>& kernel)
{
   // TO DO !!
   int kernel_size = kernel.rows;
   int padding_size = kernel_size/2;

   cv::Mat_<float> src_padding;
   cv::copyMakeBorder(src, src_padding, padding_size, padding_size, padding_size, padding_size, cv::BORDER_CONSTANT, cv::Scalar(0));

   cv::Mat_<float> flipped_kernel;
   cv::flip(kernel, flipped_kernel, -1);

   cv::Mat_<float> output(src.size());
   //std::cout << output << std::endl;
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
 * @brief Moving average filter (aka box filter)
 * @note: you might want to use Dip2::spatialConvolution(...) within this function
 * @param src Input image
 * @param kSize Window size used by local average
 * @returns Filtered image
 */
cv::Mat_<float> averageFilter(const cv::Mat_<float>& src, int kSize)
{
   // TO DO !!
   cv::Mat_<float> kernel = cv::Mat::ones(kSize, kSize, CV_32F);
   float scalar = 1/pow(kSize, 2);

   cv::Mat_<float> Avg_Kernel = kernel*scalar;
   //std::cout << Avg_Kernel << std::endl;
   cv::Mat_<float> output = spatialConvolution(src, Avg_Kernel);

   return output;
}

/**
 * @brief Median filter
 * @param src Input image
 * @param kSize Window size used by median operation
 * @returns Filtered image
 */
cv::Mat_<float> medianFilter(const cv::Mat_<float>& src, int kSize)
{
   // TO DO !!
   int padding_size = kSize/2;

   cv::Mat_<float> src_padding;
   cv::copyMakeBorder(src, src_padding, padding_size, padding_size, padding_size, padding_size, cv::BORDER_CONSTANT, cv::Scalar(0));

   cv::Mat_<float> output(src.size());
   //std::cout << output << std::endl;
   for(int i=padding_size; i<src_padding.rows - padding_size; i++){
        for(int j=padding_size; j<src_padding.cols - padding_size; j++){
            std::vector<float> neighbors;
            for(int k= -padding_size; k<= padding_size; k++){
                for(int l=-padding_size; l<= padding_size; l++){
                    neighbors.push_back(src[i+k][j+l]);
                }
            }
            std::sort(neighbors.begin(),neighbors.end());
            output.at<float>(i - padding_size,j - padding_size) = neighbors[neighbors.size()/2];
        }
    }

   return output;
}

/**
 * @brief Bilateral filer
 * @param src Input image
 * @param kSize Size of the kernel
 * @param sigma_spatial Standard-deviation of the spatial kernel
 * @param sigma_radiometric Standard-deviation of the radiometric kernel
 * @returns Filtered image
 */
cv::Mat_<float> bilateralFilter(const cv::Mat_<float>& src, int kSize, float sigma_spatial, float sigma_radiometric)
{
    // TO DO !!
    int padding_size = kSize/2;
    cv::Mat_<float> output = cv::Mat_<float>::zeros(src.size());

    cv::Mat_<float> src_padding;
    cv::copyMakeBorder(src, src_padding, padding_size, padding_size, padding_size, padding_size, cv::BORDER_CONSTANT, cv::Scalar(0));

    for(int i=padding_size; i<src_padding.rows - padding_size; i++){
        for(int j=padding_size; j<src_padding.cols - padding_size; j++){
            float weighted_sum = 0.0;
            float weight_sum = 0.0;
            for(int k= -padding_size; k<= padding_size; k++){
                for(int l=-padding_size; l<= padding_size; l++){

                    int ki = i + k;
                    int li = j + l;

                    float spatial_dist = k*k + l*l;
                    float spatial_weight = exp(-spatial_dist/(2*sigma_spatial*sigma_spatial));

                    float intensity_diff = src_padding.at<float>(i,j) - src_padding.at<float>(ki,li);
                    float radiometric_weight = exp(-intensity_diff*intensity_diff/(2*sigma_radiometric*sigma_radiometric));

                    float weight = spatial_weight*radiometric_weight;

                    weighted_sum += weight*src_padding.at<float>(ki,li);
                    weight_sum += weight;
                }
            }

            output.at<float>(i - padding_size,j - padding_size) = weighted_sum/weight_sum;
        }
    }

    return output;
}

/**
 * @brief Non-local means filter
 * @note: This one is optional!
 * @param src Input image
 * @param searchSize Size of search region
 * @param sigma Optional parameter for weighting function
 * @returns Filtered image
 */
cv::Mat_<float> nlmFilter(const cv::Mat_<float>& src, int searchSize, double sigma)
{
	int image_rows = src.sizeof(src)/src.sizeof(src[0]);
	int image_cols = src.sizeof(src[0])/src.sizeof(int);

    for(int i = 0 ; i < image_rows ; i++){
	   for(int j = 0 ; j < image_cols ; j++){
		   float local_mean = averageFilter(src[i][j], searchSize);
                   float list_weights[image_rows][image_cols];
		   //pixel's mean we will compare with the whole image
		  for (int k = 0 ; k < searchSize ; k++){
			 for(l = 0 ; l < searchSize ; l++){
				cv::Mat_<float>&
				float non_local_mean = averageFilter(src
				//average the subset of searchSize range
				float  normalized_weight = exp

			 }
		  }
	   }
	   }

	return src.clone();
}



/**
 * @brief Chooses the right algorithm for the given noise type
 * @note: Figure out what kind of noise NOISE_TYPE_1 and NOISE_TYPE_2 are and select the respective "right" algorithms.
 */
NoiseReductionAlgorithm chooseBestAlgorithm(NoiseType noiseType)
{

    // TO DO !!
    switch(noiseType){
        case NOISE_TYPE_1:
            return NR_MEDIAN_FILTER;
        case NOISE_TYPE_2:
            return NR_BILATERAL_FILTER;
        default:
            return (NoiseReductionAlgorithm) -1;
    }
    //return (NoiseReductionAlgorithm) -1;
}



cv::Mat_<float> denoiseImage(const cv::Mat_<float> &src, NoiseType noiseType, dip2::NoiseReductionAlgorithm noiseReductionAlgorithm)
{
    // TO DO !!

    // for each combination find reasonable filter parameters

    switch (noiseReductionAlgorithm) {
        case dip2::NR_MOVING_AVERAGE_FILTER:
            switch (noiseType) {
                case gaussian:
                    return dip2::averageFilter(src, 5);
                case shot:
                    return dip2::averageFilter(src, 3);
                default:
                    throw std::runtime_error("Unhandled noise type!");
            }
        case dip2::NR_MEDIAN_FILTER:
            switch (noiseType) {
                case impulse:
                    return dip2::medianFilter(src, 3);
                case random:
                    return dip2::medianFilter(src, 5);
                default:
                    throw std::runtime_error("Unhandled noise type!");
            }
        case dip2::NR_BILATERAL_FILTER:
            switch (noiseType) {
                case gaussian:
                    return dip2::bilateralFilter(src, 5, 10.0f, 10.0f);
                case random:
                    return dip2::bilateralFilter(src, 3, 5.0f, 5.0f);
                default:
                    throw std::runtime_error("Unhandled noise type!");
            }
        default:
            throw std::runtime_error("Unhandled filter type!");
    }
}





// Helpers, don't mind these

const char *noiseTypeNames[NUM_NOISE_TYPES] = {
    "NOISE_TYPE_1",
    "NOISE_TYPE_2",
};

const char *noiseReductionAlgorithmNames[NUM_FILTERS] = {
    "NR_MOVING_AVERAGE_FILTER",
    "NR_MEDIAN_FILTER",
    "NR_BILATERAL_FILTER",
};


}
