#include "GeometricTransformer.h"
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <string>
using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{

    if (argc < 5) 
    {
        cout << "Error: no input argument." << endl;
        return -1;
    }

    string* str = new string[argc - 1];
    for (int i = 1; i < argc; i++)
    {
        str[i - 1] = argv[i];
        cout << str[i - 1] << "." << endl;
    }

    //Khai báo cái biến cần thiết hỗ trợ tính toán
    GeometricTransformer gTranform;
    PixelInterpolate *pixel;

    Mat image;
    Mat dstImage;

    //Xác định phương pháp nội suy người dùng nhập là phương pháp nào
    if(str[1] == "-nn")
        pixel = new NearestNeighborInterpolate;
    else if(str[1] == "-bl")
        pixel = new BilinearInterpolate;
    else
    {
        cout << "Error Syntax!!!";
        return -1;
    }

    //Xây dựng các câu lệnh dựa trên yêu cầu của người dùng
    if(str[0] == "-zoom")
    {
        image = imread(str[4], IMREAD_COLOR);
        if(gTranform.Scale(image, dstImage, atof(str[2].c_str()), atof(str[3].c_str()), pixel) == 0)
        {
            cout << "Error!!!\n";
            return -1;
        }
        imwrite(str[5], dstImage);
    }
    if(str[0] == "-resize")
    {
        image = imread(str[4], IMREAD_COLOR);
        if(gTranform.Resize(image, dstImage, atoi(str[2].c_str()), atoi(str[3].c_str()), pixel) == 0)
        {
            cout << "Error!!!\n";
            return -1;
        }
        imwrite(str[5], dstImage);
    }
    if(str[0] == "-rotK")
    {
        image = imread(str[3], IMREAD_COLOR);
        if(gTranform.RotateUnkeepImage(image, dstImage, atof(str[2].c_str()), pixel) == 0)
        {
            cout << "Error!!!\n";
            return -1;
        }
        return 0;
        imwrite(str[4], dstImage);
    }
    if(str[0] == "-rotP")
    {
        image = imread(str[3], IMREAD_COLOR);
        if(gTranform.RotateKeepImage(image, dstImage, atof(str[2].c_str()), pixel) == 0)
        {
            cout << "Error!!!\n";
            return -1;
        }
        return 0;
        imwrite(str[4], dstImage);
    }
    if(str[0] == "-flipV")
    {
        image = imread(str[2], IMREAD_COLOR);
        if(gTranform.Flip(image, dstImage, 1, pixel) == 0)
        {
            cout << "Error!!!\n";
            return -1;
        }
        imwrite(str[3], dstImage);
    }
    if(str[0] == "-flipH")
    {
        image = imread(str[2], IMREAD_COLOR);
        if(gTranform.Flip(image, dstImage, 0, pixel) == 0)
        {
            cout << "Error!!!\n";
            return -1;
        }
        imwrite(str[3], dstImage);
    }

    imshow("SourceImage", image);
    imshow("DstImage", dstImage);
    waitKey(0);
    return 0;
}
