//
// Created by henrik on 28/02/2025.
//

#ifndef RAYLIB_OPENCV_HEADERS_H
#define RAYLIB_OPENCV_HEADERS_H

#include <raylib.h>

#if defined(_WIN32)
#define NOGDI             // All GDI defines and routines
#define NOUSER            // All USER defines and routines
#endif

#include <opencv2/opencv.hpp>

#if defined(_WIN32)           // raylib uses these names as function parameters
#undef near
#undef far
#endif

#endif //RAYLIB_OPENCV_HEADERS_H
