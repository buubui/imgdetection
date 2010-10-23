#pragma once
#include "cv.h"
Mat* imFilter(const Mat& img,bool equalize);
Mat* imFilterChannels(const Mat& img,bool equalize);
