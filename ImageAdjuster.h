//
// Created by henrik on 23/02/2025.
//

#ifndef IMAGEADJUSTER_H
#define IMAGEADJUSTER_H
#include <raylib.h>
#include <opencv2/opencv.hpp>

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

        // Congiguration options
        float brightness = 128.0f;
        float contrast = 128.0f;
        float sharpness = 128.0f;
        float saturation = 128.0f;
        float focus = 128.0f;

        static void matToTexture(cv::Mat &frame, Texture2D &texture);
        void updateCameraSettings();
        void readCurrentCameraSettings();
        void resetCameraSettings();

        Rectangle getGuiRectangle(int index) const;
    };
}


#endif //IMAGEADJUSTER_H
