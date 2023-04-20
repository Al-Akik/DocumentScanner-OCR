//
//  DocumentScanner.cpp

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
 
using namespace cv;
using namespace std;
using namespace tesseract;

Mat imgOriginal, imgProcessed ,imgGray, imgCanny, imgBlurred, imgDilated, imgWarped,imgCropped;


float width = 420, height =596;

vector<Point> getContours(Mat image) {
 
 
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
 
    findContours(image, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    vector<vector<Point>> conPoly(contours.size());
    vector<Rect> boundRect(contours.size());
 
    vector<Point> biggest;
    int maxArea = 0;
    for (int i = 0; i < contours.size(); i++)
    {
        int area = contourArea(contours[i]);
        cout << area << endl;
 
        string objectType;
 
        if (area > 1000)
        {
            float peri = arcLength(contours[i], true);
            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
            
            if(area>maxArea && conPoly[i].size()==4){
                biggest = {conPoly[i][0],conPoly[i][1],conPoly[i][2],conPoly[i][3]};
                maxArea = area;
            }
 
            //drawContours(imgOriginal, conPoly, i, Scalar(255, 0, 255), 2);
        }
    }
    return biggest;
}

void drawPoints(vector<Point> points, Scalar color){
    for(int i =0; i<points.size();i++){
        circle(imgOriginal,points[i],10,color,FILLED);
        putText(imgOriginal, to_string(i), points[i], FONT_HERSHEY_PLAIN, 4, color,4);
    }
}

vector<Point> reorder(vector<Point> initialPoints){
    vector<Point> newPoints;
    vector<int> sumPoints,subPoints;
    
    for(int i =0; i<4;i++){
        sumPoints.push_back(initialPoints[i].x + initialPoints[i].y);
        subPoints.push_back(initialPoints[i].x - initialPoints[i].y);
    }
    
    newPoints.push_back(initialPoints[min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);
    newPoints.push_back(initialPoints[max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);
    newPoints.push_back(initialPoints[min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);
    newPoints.push_back(initialPoints[max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);
    
    return newPoints;
}

Mat preprocessing(Mat img){
    Mat kernel = getStructuringElement(MORPH_RECT, Size(5,5));
    cvtColor(img, imgGray, COLOR_BGR2GRAY);
    GaussianBlur(img, imgBlurred, Size(3,3), 3,0);
    Canny(img, imgCanny, 20, 75);
    dilate(imgCanny, imgDilated, kernel);
    return imgDilated;
}

Mat warpImg(Mat img, vector<Point> points,float width,float height){
    
    Point2f src[4] = { points[0],points[1],points[2],points[3] };
    Point2f dst[4] = { {0.0f,0.0f},{width,0.0f},{0.0f,height},{width,height} };

    Mat matrix = getPerspectiveTransform(src, dst);
    warpPerspective(img, imgWarped, matrix, Point(width, height));

    return imgWarped;
    
}

int main(){
    string path = "PATH_TO_THE_IMAGE";
    imgOriginal = imread(path);
    resize(imgOriginal, imgOriginal, Size(),0.5,0.5);
    //Preprocessing of the Image
    imgProcessed = preprocessing(imgOriginal);
    vector<Point> initialPoints = getContours(imgProcessed);
    //Get ordered border Points
    vector<Point> finalPoints = reorder(initialPoints);
    //drawPoints(finalPoints, Scalar(0,255,0));
    //Warp the image to only get the document
    imgWarped = warpImg(imgOriginal,finalPoints,width,height);
    //imshow("Original Image", imgOriginal);
    //imshow("Preprocessed image", imgProcessed);
    imshow("Warped Image", imgWarped);
    //Perform OCR on the warped image
    tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();

    if (ocr->Init(NULL, "eng")) {
        std::cerr << "Could not initialize tesseract." << std::endl;
        exit(1);
    }
    ocr->SetPageSegMode(PSM_AUTO);
    ocr->SetImage(imgWarped.data, imgWarped.cols, imgWarped.rows, 3, imgWarped.step);
    string outText = string(ocr->GetUTF8Text());
    //Text detected
    cout << outText;
    ocr->End();
    
    waitKey(0);
}
