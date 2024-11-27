# How do I clone my git repo inside my docker
Run the following:
'''
git clone 
'''

#How do I change one branch in my local directory
'''
git switch branch-name
'''

#How do I run my container mounting my local repo and at the same time my git repo
Run the following:
'''
docker run -it -v /home/aitor/projects/Digital-Image-Processing:/root/projects/Digital-Image-Processing dip_dip_et_coyote /bin/bash
'''
#How to run my C++ code inside my container ?
0. I assume you installed the same environment as me (see 'Dockerfile')
1. Make sure you have an empty directory (in my context a directory per homework)
2. Put your 'main.cpp' file (the whole '.zip' given by the teacher in facts)
3. Open a shell in thsi directory ( yourProjects/DIP/homeworkN/)
4. Run the following command:
'''
cmake -S . -B build
'''
this will create another directory called 'build' in order to compile and run your code
5. To compile your code, go in '/build' and run in a shell:
'''
make
'''

#My first bug in C++
'''
Segmentation fault (core dump)
'''
In my method createGaussianKernel1D, I changed matrix initialization
from 'cv::Mat<float> GaussianKernel1D[kSize]'
to   'cv::Mat<float> GaussianKernel1D' And changed the access to
his element to define i-th element with gaussian distribution
from 'GaussianKernel1D[i]'
to   'GaussianKernel1D.at<float>(i)'
And now I got the following:
'''
Floating point exception (core dumped)
'''
Another problem (maybe linked) was to iterate over negative indices
and use them inside a function.

