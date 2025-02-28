//
// Created by henrik on 23/02/2025.
//

#ifndef IMAGEADJUSTER_H
#define IMAGEADJUSTER_H

#include "raylib_opencv_headers.h"

#define UPDATE_TEXTURE_WORKAROUND

namespace camTranslator {
    class ImageAdjuster {
    public:
        void adjustImage();
        ImageAdjuster();
        ~ImageAdjuster();
    private:
        std::unique_ptr<cv::VideoCapture> capture;
        bool updateTextureWorkaround = true;

        int screenWidth;
        int screenHeight;
        int imageWidth;
        int imageHeight;

        // Congiguration options
        float brightness = 128.0f;
        float contrast = 128.0f;
        float sharpness = 128.0f;
        float saturation = 128.0f;
        float focus = 128.0f;

        // Crop rectangle
        char cropX[20] = "0";
        char cropY[20] = "0";
        char cropWidth[20] = "0";
        char cropHeight[20] = "0";

        static void matToTexture(cv::Mat &frame, Texture2D &texture);
        void updateCameraSettings();
        void readCurrentCameraSettings();
        void resetCameraSettings();

        Rectangle getGuiRectangle(int index) const;
        Rectangle getGuiRectangle(int index, float length) const;
        Rectangle getGuiRectangle(int index, float length, float xOffset) const;
    };
}


#endif //IMAGEADJUSTER_H
