FROM nvidia/cuda:9.2-cudnn7-devel-ubuntu18.04
MAINTAINER Yi-Ren Liu

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update
RUN apt-get install apt-utils -y
RUN apt-get install build-essential -y

RUN apt-get install tzdata -y\
    && apt-get install libatlas-base-dev liblapack-dev libblas-dev -y\
    && apt-get install libopenblas-dev libfftw3-dev liblapacke-dev -y\
    && apt-get install libsuitesparse-dev -y\
    && apt-get install libboost-all-dev -y\
    && apt-get install cmake cmake-qt-gui git gitk libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev -y\
    && apt-get install python-dev python-numpy python-pip libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev  libdc1394-22-dev -y\
    && apt-get install libgoogle-glog-dev libgtest-dev -y\
    && apt-get install zip -y\
    && apt-get install qtcreator -y

WORKDIR /home/
RUN mkdir 3rdParty
WORKDIR 3rdParty/

RUN git clone https://github.com/jasperproject/jasper-client.git jasper \ 
    && chmod +x jasper/jasper.py \ 
    && pip install --upgrade setuptools \ 
    && pip install -r jasper/client/requirements.txt

WORKDIR /

# nvidia-docker2
# nvidia-container-runtime
ENV NVIDIA_VISIBLE_DEVICES \
    ${NVIDIA_VISIBLE_DEVICES:-all}
ENV NVIDIA_DRIVER_CAPABILITIES \
    ${NVIDIA_DRIVER_CAPABILITIES:+$NVIDIA_DRIVER_CAPABILITIES,}graphics

# install ROS
# install packages
RUN apt-get update && apt-get install -q -y \
    dirmngr \
    gnupg2 \
    lsb-release \
    && rm -rf /var/lib/apt/lists/*

# setup keys
RUN apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 421C365BD9FF1F717815A3895523BAEEB01FA116

# setup sources.list
RUN echo "deb http://packages.ros.org/ros/ubuntu `lsb_release -sc` main" > /etc/apt/sources.list.d/ros-latest.list

# install bootstrap tools
RUN apt-get update && apt-get install --no-install-recommends -y \
    python-rosdep \
    python-rosinstall \
    python-vcstools \
    && rm -rf /var/lib/apt/lists/*

# setup environment
ENV LANG C.UTF-8
ENV LC_ALL C.UTF-8

# bootstrap rosdep
RUN rosdep init \
    && rosdep update

# install ros packages
ENV ROS_DISTRO melodic
RUN apt-get update && apt-get install -y \
    ros-melodic-desktop-full \
    && rm -rf /var/lib/apt/lists/*

# install eigen
WORKDIR /home/3rdParty
COPY eigen3.3.5.zip /home/3rdParty/
RUN unzip eigen3.3.5.zip && mkdir -p eigen3.3.5/build
WORKDIR eigen3.3.5/build
RUN cmake .. && make install
WORKDIR /home/3rdParty
RUN rm -rf eigen3.3.5/

# install sophus
RUN git clone https://github.com/strasdat/Sophus && mkdir -p Sophus/build
WORKDIR Sophus
RUN git checkout b475c0a
WORKDIR build
RUN cmake .. && make -j12 && make install
WORKDIR /home/3rdParty
RUN rm -rf Sophus/build/

# install ceres
RUN git clone https://ceres-solver.googlesource.com/ceres-solver && mkdir -p ceres-solver/build
WORKDIR ceres-solver
RUN git checkout b040970
WORKDIR build
RUN cmake .. && make -j12 && make install
WORKDIR /home/3rdParty
RUN rm -rf ceres-solver/build/

# install opencv
COPY opencv3.4.3.zip /home/3rdParty/
COPY opencv_contrib-3.4.3.zip /home/3rdParty/
RUN unzip opencv3.4.3.zip && unzip opencv_contrib-3.4.3.zip && mkdir -p opencv-3.4.3/build
WORKDIR opencv-3.4.3/build
RUN cmake .. -DCMAKE_BUILD_TYPE=Release -DOPENCV_EXTRA_MODULES_PATH=/home/3rdParty/opencv_contrib-3.4.3/modules -DWITH_CUDA=ON && make -j12 && make install
WORKDIR /home/3rdParty
RUN rm -rf opencv-3.4.3/ && rm -rf opencv_contrib-3.4.3/

WORKDIR /

# setup entrypoint
COPY ./ros_entrypoint.sh /
RUN chmod +x ros_entrypoint.sh

ENTRYPOINT ["/ros_entrypoint.sh"]
CMD ["bash"]
