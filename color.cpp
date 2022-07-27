#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
   VideoCapture video_load(0);
   namedWindow("Adjust");

   int Hue_Lower_Value = 135;
   int Hue_Lower_Upper_Value = 160;

   int Saturation_Lower_Value = 40;
   int Saturation_Upper_Value = 255;

   int Value_Lower = 160;
   int Value_Upper = 255;

   int Circle_Lower = 75;
   int Circle_Upper = 400;

   int Min_Dist = 100;
   int Max_Dist = 100;


   createTrackbar("Hue_Lower", "Adjust", &Hue_Lower_Value, 179);
   createTrackbar("Hue_Upper", "Adjust", &Hue_Lower_Upper_Value, 179);

   createTrackbar("Sat_Lower", "Adjust", &Saturation_Lower_Value, 255);
   createTrackbar("Sat_Upper", "Adjust", &Saturation_Upper_Value, 255);

   createTrackbar("Val_Lower", "Adjust", &Value_Lower, 255);
   createTrackbar("Val_Upper", "Adjust", &Value_Upper, 255);

   createTrackbar("Circle_Lower", "Adjust", &Circle_Lower, 400);
   createTrackbar("Circle_Upper", "Adjust", &Circle_Upper, 400);

   createTrackbar("Min_Dist", "Adjust", &Min_Dist, 100);
   createTrackbar("Max_Dist", "Adjust", &Max_Dist, 100);

   while (1) {
      Mat orig_image;
      bool temp = video_load.read(orig_image);

      Mat hsv_image;
      medianBlur(orig_image, orig_image, 3);
      cvtColor(orig_image, hsv_image, COLOR_BGR2HSV);
      vector<Vec3f> circles;

      // Threshold the HSV image, keep only the red pixels
      cv::Mat lower_red_hue_range;
      cv::Mat upper_red_hue_range;
      cv::inRange(hsv_image, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), lower_red_hue_range);
      cv::inRange(hsv_image, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), upper_red_hue_range);

      // Combine the above two images
      cv::Mat red_hue_image;
      cv::addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);
      cv::GaussianBlur(red_hue_image, red_hue_image, cv::Size(9, 9), 2, 2);
      cv::HoughCircles(red_hue_image, circles, cv::HOUGH_GRADIENT, 1.2, Min_Dist, 100, 30, Circle_Lower, Circle_Upper);

      Mat lower_pink;
      inRange(hsv_image, Scalar(Hue_Lower_Value, Saturation_Lower_Value, Value_Lower), Scalar(Hue_Lower_Upper_Value, Saturation_Upper_Value, Value_Upper), lower_pink);
      cv::GaussianBlur(lower_pink, lower_pink, Size(9, 9), 2, 2);
      cv::HoughCircles(lower_pink, circles, cv::HOUGH_GRADIENT, 1.2, Min_Dist, 100, 30, Circle_Lower, Circle_Upper);

      for(size_t cur = 0; cur < circles.size(); ++cur) {
        Point center(circles[cur][0], circles[cur][1]);
        int radius = circles[cur][2]; 
        circle(orig_image, center, radius, Scalar(239, 152, 38), 2); 

        waitKey(10);
      }

      imshow("Upper Red Image", lower_red_hue_range);
      imshow("Lower Red Image", upper_red_hue_range);
      imshow("Combined Red Image", red_hue_image);
      imshow("Pink Image", lower_pink);
      imshow("Original", orig_image);

      if (waitKey(30) == 27){ 
         break;
      }
   }
   return 0;
}