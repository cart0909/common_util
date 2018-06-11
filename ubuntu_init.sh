sudo apt-get update
sudo apt-get upgrade
sudo apt-get update

# disk control
sudo apt-get install gparted

# change cpu mode
sudo apt-get install indicator-cpufreq

# basic compiler env
sudo apt-get install build-essential

# lapack + blas
# Using OpenBLAS
sudo apt-get install libopenblas-dev libfftw3-dev liblapacke-dev
# Using ATLAS
sudo apt-get install libatlas3-base libatlas-dev libfftw3-dev liblapacke-dev

# suitesparse
sudo apt-get install libsuitesparse-dev

# opencl
sudo apt-get install ocl-icd-dev
sudo apt-get install opencl-headers

# nvidia driver
sudo apt-get nvidia-375(up) nvidia-modprobe

# Cuda
sudo dpkg -i cuda-repo-*amd64.deb
sudo apt-get update
sudo apt-get install cuda

# boost
sudo apt-get install libboost-all-dev

# eigen
wget http://bitbucket.org/eigen/eigen/get/3.2.10.tar.gz

# opencv
sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev

# git
sudo apt-get install gitk

# terminator
sudo apt-get install terminator

# qt
sudo apt-get install qt5-default

# clang-format
sudo apt-get install clang-format-5.0
