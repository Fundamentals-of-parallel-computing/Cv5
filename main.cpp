#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {
    Mat inputImage = imread("/home/alex/OpenCVProject/stop.png");

    if (inputImage.empty()) {
        cout << "Ошибка: изображение не найдено. Проверьте путь." << endl;
        return -1;
    }
    resize(inputImage, inputImage, Size(400, 400));

    Mat hsvImage;
    cvtColor(inputImage, hsvImage, COLOR_BGR2HSV);

    Mat redAreaMask, lowerRedMask, upperRedMask, resultRed;
    inRange(hsvImage, Scalar(0, 100, 100), Scalar(10, 255, 255), lowerRedMask);
    inRange(
        hsvImage, Scalar(160, 100, 100), Scalar(180, 255, 255), upperRedMask);

    bitwise_or(lowerRedMask, upperRedMask, redAreaMask);
    bitwise_and(inputImage, inputImage, resultRed, redAreaMask);

    Mat grayscaleImage;
    cvtColor(inputImage, grayscaleImage, COLOR_BGR2GRAY);

    GaussianBlur(grayscaleImage, grayscaleImage, Size(9, 9), 2);

    vector<Vec3f> detectedCircles;
    HoughCircles(
        grayscaleImage, detectedCircles, HOUGH_GRADIENT, 1, grayscaleImage.rows,
        55, 45, 300, 380);

    for (size_t i = 0; i < detectedCircles.size(); ++i) {
        Vec3i circle = detectedCircles[i];
        Point center(circle[0], circle[1]);
        int radius = circle[2];

        cv::circle(resultRed, center, 3, Scalar(0, 255, 0), -1, LINE_AA);
        cv::circle(resultRed, center, radius, Scalar(255, 0, 0), 2, LINE_AA);
    }

    resize(redAreaMask, redAreaMask, Size(400, 400));
    resize(resultRed, resultRed, Size(400, 400));

    imshow("Original Image", inputImage);
    imshow("Red Mask", redAreaMask);
    imshow("Red Detection", resultRed);

    waitKey(0);
    destroyAllWindows();

    return 0;
}
