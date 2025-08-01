![Top Language](https://img.shields.io/github/languages/top/Zungu02/Automatic-Tracking-Tracking-Patrol-Laser-System)
![Language Count](https://img.shields.io/github/languages/count/Zungu02/Automatic-Tracking-Tracking-Patrol-Laser-System)
![Code Size](https://img.shields.io/github/languages/code-size/Zungu02/Automatic-Tracking-Tracking-Patrol-Laser-System)
![Repo Size](https://img.shields.io/github/repo-size/Zungu02/Automatic-Tracking-Tracking-Patrol-Laser-System)
![License](https://img.shields.io/github/license/Zungu02/Automatic-Tracking-Tracking-Patrol-Laser-System)
![Last Commit](https://img.shields.io/github/last-commit/Zungu02/Automatic-Tracking-Tracking-Patrol-Laser-System)

# Automatic-Tracking,Detection & Patrol-Laser-System
![Tracking System Output](media/014728.png")
## Overview
The prototype uses OpenCv with MobileNet-SSD to detect an object(Human ,Class ID = 15) infront of the camera ,then tries to calculate the center of the bounding box to fetch the coordinate position which is then transmitted via serial communication toward the ESP32 to drive the ST3125 bus servos (A Pan Tilt Mechanism) for smooth trackiing of the detected person in a restricted range (45 degerees Pan axis & 10 degrees tilt axis) and manual control of laser module(currently) . In the absence of the detected person the system offers a patrol mechanism to search for any movement which might suspectedly be person .

## Hardware Used
1. Raspberrypi 4B.
2. General Driver Board for Robotics.
3. ST3125 Serial Bus Servos.
4. PiCamera G.
5. A Laser module.
   
## Setup and Installation
### Setting Up Open-CV on Raspberry Pi 'Buster' OS

This is an in-depth procedure to follow to get your Raspberry Pi to install Open-CV that will work with Computer Vision for Object Identification.Turn on a Raspberry Pi 4 Model B running a fresh version of Raspberry Pi 'Buster' OS and connect it to the Internet.

      sudo apt-get update && sudo apt-get upgrade

We must now expand the swapfile before running the next set of commands. To do this type into terminal this line.

      sudo nano /etc/dphys-swapfile

 Then change the number on CONF_SWAPSIZE = 100 to CONF_SWAPSIZE=2048 and restart.

      sudo systemctl restart dphys-swapfile

### Install build tools & OpenCV prerequisites

      sudo apt install -y \
        build-essential cmake pkg-config \
        libjpeg-dev libtiff5-dev libjasper-dev libpng12-dev \
        libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
        libxvidcore-dev libx264-dev \
        libgtk2.0-dev libgtk-3-dev \
        libatlas-base-dev gfortran \
        python3-pip
### Install NumPy & download OpenCV source

      sudo pip3 install numpy
      wget -O opencv.zip https://github.com/opencv/opencv/archive/4.4.0.zip
      wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/4.4.0.zip
      unzip opencv.zip
      unzip opencv_contrib.zip
This fetches OpenCV 4.4.0 and its extra modules.

### Build & install OpenCV

      cd opencv-4.4.0
      mkdir build && cd build
      cmake -D CMAKE_BUILD_TYPE=RELEASE \
            -D CMAKE_INSTALL_PREFIX=/usr/local \
            -D INSTALL_PYTHON_EXAMPLES=ON \
            -D OPENCV_EXTRA_MODULES_PATH=~/opencv_contrib-4.4.0/modules \
            -D BUILD_EXAMPLES=ON ..
      make -j$(nproc)        # may take 1–2 hours
      sudo make install
      sudo ldconfig
      sudo reboot
Once you reboot, OpenCV will be available system‑wide

### Clone/download the demo face‑tracking code

      wget https://core-electronics.com.au/media/kbase/507/Face-Tracking-Pimoroni-Codes.zip
      unzip Face-Tracking-Pimoroni-Codes.zip
this gives you a demo,then copy the tracker.py file in the same unzipped directory.
Verify import cv2 works and the sample script runs
     
      python3 facetracker.py

Below shows what you would see on your Desktop as you run the above code.

