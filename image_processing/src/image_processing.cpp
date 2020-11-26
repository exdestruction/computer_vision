#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
void show_image(const char* name, cv::Mat cv_matrix, int x_pos, int y_pos, double zoom_factor);
int main()
{
    cv::Mat image = cv::imread("..//images//fanta.bmp", cv::IMREAD_GRAYSCALE);
    while (1) {
        if (image.data == 0) {
            std::cout << "Image not found\n" << "Press ENTER to quit\n";
            std::cin.get();
            return 1;
        }
        show_image("Fanta", image, 600, 50, 0.4);
        //cv::imshow("RGB", image);
        if ((cv::waitKey(0) & 255) == 'q') 
        {
            break;
        }
    }
    return 0;
}

//enables resizing window while opening
void show_image(const char* name, cv::Mat image, int x_pos, int y_pos, double zoom_factor) {

    if (zoom_factor < 1.e-6)
    {
        std::cout << "Wrong Zoom-factor: too small -> it is set to one\n";
        zoom_factor = 1.0;
    }
    cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(name, x_pos, y_pos);
    if (image.rows == 0 || image.cols == 0) {
        cout << "Failure in show_image: height or width are identical to zero" << endl;
        return;
    }
    int type = image.type();
    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);


    if (fabs(zoom_factor - 1.0) > 1.e-6) {
        cv::Mat resized_image;
        cv::Size new_size;
        new_size.height = (int)(image.rows * zoom_factor);
        new_size.width = (int)(image.cols * zoom_factor);
        cv::resize(image, resized_image, new_size);
        cv::imshow(name, resized_image);
    }
    else
    {
        cv::imshow(name, image);
    }

    return;
}

