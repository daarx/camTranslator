//
// Created by henrik on 23/02/2025.
//

#include "ImageAdjuster.h"
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <memory>
#include <opencv2/opencv.hpp>

namespace camTranslator {
    ImageAdjuster::ImageAdjuster() {
        setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);

        std::cout << "Opening camera" << std::endl;

        cv::setNumThreads(1);
        capture = std::make_unique<cv::VideoCapture>(0, cv::VideoCaptureAPIs::CAP_DSHOW);
        if (!capture->isOpened()) {
            throw std::runtime_error("Could not open the camera!");
        }

        capture->set(cv::VideoCaptureProperties::CAP_PROP_FRAME_WIDTH, 3840);
        capture->set(cv::VideoCaptureProperties::CAP_PROP_FRAME_HEIGHT, 2160);
        capture->set(cv::VideoCaptureProperties::CAP_PROP_N_THREADS, 1);

        screenWidth = static_cast<int>(capture->get(cv::CAP_PROP_FRAME_WIDTH));
        screenHeight = static_cast<int>(capture->get(cv::CAP_PROP_FRAME_HEIGHT));

        readCurrentCameraSettings();
    }

    ImageAdjuster::~ImageAdjuster() {
        capture->release();

        std::cout << "Printing current camera settings: " << std::endl;
        std::cout << "Brightness: " << brightness << std::endl;
        std::cout << "Contrast  : " << contrast << std::endl;
        std::cout << "Sharpness : " << sharpness << std::endl;
        std::cout << "Saturation: " << saturation << std::endl;
        std::cout << "Focus     : " << focus << std::endl;
    }

    void ImageAdjuster::adjustImage() {
        SetTraceLogLevel(LOG_DEBUG);
        InitWindow(screenWidth, screenHeight, "Hello World");
        SetTargetFPS(30);

#ifndef UPDATE_TEXTURE_WORKAROUND
        Image tempImage = GenImageColor(screenWidth, screenHeight, BLACK);
        Texture2D texture = LoadTextureFromImage(tempImage); // Create empty texture
        UnloadImage(tempImage);

        Rectangle rect{ 0, 0, (float) screenWidth, (float) screenHeight };
#endif

        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(BLACK);

            cv::Mat frame;
            (*capture) >> frame;
            if (frame.empty()) {
                throw std::runtime_error("No image data available");
            };

            // Ensure frame matches texture size (avoid segmentation faults)
            if (frame.cols != screenWidth || frame.rows != screenHeight) {
                cv::resize(frame, frame, cv::Size(screenWidth, screenHeight));
            }

#ifndef UPDATE_TEXTURE_WORKAROUND
            matToTexture(frame, texture);
#endif

            cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

#ifdef UPDATE_TEXTURE_WORKAROUND
            Image image = {
                frame.data,
                screenWidth,
                screenHeight,
                1,
                PixelFormat::PIXELFORMAT_UNCOMPRESSED_R8G8B8
            };
            Texture2D texture = LoadTextureFromImage(image);
#endif
            // DrawTexturePro(texture, rect, rect, Vector2{}, 0.0f, WHITE);
            DrawTexture(texture, 0, 0, WHITE);

            // GUI

            bool resetValues = GuiButton(getGuiRectangle(0), "Reset values");

            if (resetValues) {
                std::cout << "Reset values" << std::endl;
                resetCameraSettings();
            }

            GuiSliderBar(getGuiRectangle(1), "Brightness", TextFormat("%.2f", brightness), &brightness, 0, 256);
            GuiSliderBar(getGuiRectangle(2), "Contrast", TextFormat("%.2f", contrast), &contrast, 0, 256);
            GuiSliderBar(getGuiRectangle(3), "Sharpness", TextFormat("%.2f", sharpness), &sharpness, 0, 256);
            GuiSliderBar(getGuiRectangle(4), "Saturation", TextFormat("%.2f", saturation), &saturation, 0, 256);
            GuiSliderBar(getGuiRectangle(5), "Focus", TextFormat("%.2f", focus), &focus, 0, 256);

            updateCameraSettings();

            EndDrawing();

#ifdef UPDATE_TEXTURE_WORKAROUND
            UnloadTexture(texture);
#endif
        }

#ifndef UPDATE_TEXTURE_WORKAROUND
        UnloadTexture(texture);
#endif
        CloseWindow();
    }

    void ImageAdjuster::matToTexture(cv::Mat &frame, Texture2D &texture) {
        // Ensure the frame is in the correct format (RGB)
        if (frame.channels() == 3) {
            cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);  // Convert BGR to RGB
        } else if (frame.channels() == 4) {
            cv::cvtColor(frame, frame, cv::COLOR_BGRA2RGBA);  // Convert BGRA to RGBA
        } else {
            printf("Error: Unexpected frame channels: %d\n", frame.channels());
            return;
        }

        // Ensure the Mat data is continuous (avoid segmentation faults)
        if (!frame.isContinuous()) {
            frame = frame.clone();
        }

        printf("Frame: %d x %d | Channels: %d | Continuous: %d\n", frame.cols, frame.rows, frame.channels(), frame.isContinuous());

        // Update the texture safely
        UpdateTexture(texture, frame.data);
    }

    void ImageAdjuster::updateCameraSettings() {
        capture->set(cv::VideoCaptureProperties::CAP_PROP_BRIGHTNESS, brightness);
        capture->set(cv::VideoCaptureProperties::CAP_PROP_CONTRAST, contrast);
        capture->set(cv::VideoCaptureProperties::CAP_PROP_SHARPNESS, sharpness);
        capture->set(cv::VideoCaptureProperties::CAP_PROP_SATURATION, saturation);
        capture->set(cv::VideoCaptureProperties::CAP_PROP_FOCUS, focus);
    }

    void ImageAdjuster::readCurrentCameraSettings() {
        brightness = capture->get(cv::VideoCaptureProperties::CAP_PROP_BRIGHTNESS);
        contrast = capture->get(cv::VideoCaptureProperties::CAP_PROP_CONTRAST);
        sharpness = capture->get(cv::VideoCaptureProperties::CAP_PROP_SHARPNESS);
        saturation = capture->get(cv::VideoCaptureProperties::CAP_PROP_SATURATION);
        focus = capture->get(cv::VideoCaptureProperties::CAP_PROP_FOCUS);
    }

    void ImageAdjuster::resetCameraSettings() {
        brightness = 128;
        contrast = 128;
        sharpness = 128;
        saturation = 128;
        focus = 0;
    }

    Rectangle ImageAdjuster::getGuiRectangle(int index) const {
        return {
            screenWidth * 4.0f / 5.0f,
            screenHeight / 20.0f + 35.0f * index,
            256,
            30
        };
    }
}
