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
void show_image(const char* name, cv::Mat image_matrix, int x_pos, int y_pos, double zoom_factor) {

    if (zoom_factor < 1.e-6)
    {
        std::cout << "Wrong Zoom-factor: too small -> it is set to one" << std::endl;
        zoom_factor = 1.0;
    }
    cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(name, x_pos, y_pos);
    cv::Mat cloned_image = image_matrix.clone();
    cv::Size image_size;
    image_size.height = cloned_image.rows;
    image_size.width = cloned_image.cols;
    if (image_size.height == 0 || image_size.width == 0) {
        cout << "Failure in show_image: height or width are identical to zero" << endl;
        return;
    }
    int type = cloned_image.type();
    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);


    if (fabs(zoom_factor - 1.0) > 1.e-6) {
        cv::Mat      tmp;
        cv::Size   tmp_size;
        tmp.rows = (int)(image_size.height * zoom_factor);
        tmp.cols = (int)(image_size.width * zoom_factor);
        tmp_size.height = tmp.rows;
        tmp_size.width = tmp.cols;
        cv::resize(cloned_image, tmp, tmp_size);
        cv::imshow(name, tmp);
    }
    else
        cv::imshow(name, cloned_image);
    return;
}

