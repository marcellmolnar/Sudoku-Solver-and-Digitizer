#include <jni.h>
#include <string>
#include <opencv2/opencv.hpp>
//#include "opencv2/dnn/dnn.hpp"
//#include "opencv2/dnn.hpp"



using namespace cv;
using namespace std;

extern "C"{
jstring
Java_hu_marci_customsolutions_sudokusolveranddigitizer_MainActivity_stringFromJNI(JNIEnv* env, jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

int toGray(Mat& img, Mat& mRgbaflipped, Mat& gray, Mat& flipped){
    cvtColor(img, gray, CV_RGB2GRAY);
    transpose(img, mRgbaflipped);
    flip(mRgbaflipped, mRgbaflipped, +1);
    transpose(gray, flipped);
    flip(flipped, flipped, +1);
    if (gray.rows == img.rows && gray.cols == img.cols){
        return 1;
    }
    else{
        return 0;
    }
}

vector<Point> getCorners(vector<Point> contour){
    Moments mu = moments(contour, false);
    Point2f center = Point2f(static_cast<float>(mu.m10 / mu.m00),
                             static_cast<float>(mu.m01 / mu.m00));
    //circle( mRgb, center, 4, Scalar(255,255,0), -1, 8, 0 );
    vector<Point>::const_iterator mPoint = contour.begin();
    Point tl = *mPoint;
    Point tr = *mPoint;
    Point bl = *mPoint;
    Point br = *mPoint;
    double tl_radius_sq = 0;
    double tr_radius_sq = 0;
    double bl_radius_sq = 0;
    double br_radius_sq = 0;
    for(; mPoint != contour.end(); ++mPoint) {
        double rad = ((*mPoint).y-center.y)*((*mPoint).y-center.y) + ((*mPoint).x-center.x)*((*mPoint).x-center.x);
        // If *mPoint is in the top left quarter, and it's radius bigger then the current, change.
        if ((*mPoint).x < center.x && (*mPoint).y < center.y  && rad > tl_radius_sq){
            tl = *mPoint;
            tl_radius_sq = rad;
        }
        // If *mPoint is in the bottom right quarter, and it's radius bigger then the current, change.
        if ((*mPoint).x > center.x && (*mPoint).y > center.y  && rad > br_radius_sq){
            br = *mPoint;
            br_radius_sq = rad;
        }
        // If *mPoint is in the bottom left quarter, and it's radius bigger then the current, change.
        if ((*mPoint).x < center.x && (*mPoint).y > center.y  && rad > bl_radius_sq){
            bl = *mPoint;
            bl_radius_sq = rad;
        }
        // If *mPoint is in the top right quarter, and it's radius bigger then the current, change.
        if ((*mPoint).x > center.x && (*mPoint).y < center.y  && rad > tr_radius_sq){
            tr = *mPoint;
            tr_radius_sq = rad;
        }

    }
    vector<Point> myVec;
    myVec.push_back(tl);
    myVec.push_back(tr);
    myVec.push_back(bl);
    myVec.push_back(br);
    return myVec;
}

int detectSusoku(/*dnn::Net& net,*/ Mat& mRgb, Mat& sudoku){
    // Some Gaussian blurring, and adaptive threshold. Border lines and number will be white, background black.

    Mat sudoku_temp(sudoku.size(), sudoku.type());
    sudoku.copyTo(sudoku_temp);
    GaussianBlur(sudoku_temp, sudoku_temp, Size(11,11), 0);
    adaptiveThreshold(sudoku_temp, sudoku_temp, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 5, 2);
    // Not necessary when THRESH_BINATY_INV used in thresholding.
    //bitwise_not(sudoku, sudoku);

    /*vector<Vec4i> lines;
    HoughLinesP(sudoku, lines, 1, CV_PI/180, 5000, 50, 10 );
    RNG rng(12345);
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        if ( (l[0]-l[2])*(l[0]-l[2]) + (l[1]-l[3])*(l[1]-l[3]) > 10000){
            Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
            //line( mRgb, Point(l[0], l[1]), Point(l[2], l[3]), color, 1, CV_AA);
        }
    }*/



    // Finding contours.
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours( sudoku_temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );


    // Finding bounding rectangles.
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    for( int i = 0; i < contours.size(); i++ ) {
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect(Mat(contours_poly[i]));
    }


    if (!contours.empty()){

        /*vector<vector<Point>> biggestIndeces;
        int biggestContourIndex = 0;
        double biggestArea = boundRect[0].area();
        for( int i = 0; i< contours.size(); i++ ) {

            double area = boundRect[i].area();
            if (area > biggestArea){
                biggestContourIndex = i;
                biggestArea = area;
            }
        }*/

        sort(contours.begin(), contours.end(),
             [](const vector<Point>& a, const vector<Point>& b)
             {
                 return contourArea(a) > contourArea(b);
             });


        RNG rng(12345);
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        //rectangle( mRgb, biggestRect.tl(), biggestRect.br(), color, 2, 8, 0 );
        //drawContours( mRgb, contours_poly, biggestContourIndex, color, 1, 8, vector<Vec4i>(), 0, Point() );

        int biggestContoursIndex = 0;
        vector<Point> corners = getCorners(contours[biggestContoursIndex]);
        Point tl = corners[0];
        Point tr = corners[1];
        Point bl = corners[2];
        Point br = corners[3];
        Point center = Point((tl.x+tr.x+bl.x+br.x)/4,(tl.y+tr.y+bl.y+br.y)/4);
        circle( mRgb, tl, 4, Scalar(255,0,0), -1, 8, 0 );
        circle( mRgb, tr, 4, Scalar(0,255,0), -1, 8, 0 );
        circle( mRgb, bl, 4, Scalar(0,0,255), -1, 8, 0 );
        circle( mRgb, br, 4, Scalar(255,255,0), -1, 8, 0 );
        circle( mRgb, center, 4, Scalar(255,0,255), -1, 8, 0 );
        double d1 = (tl.x-tr.x)*(tl.x-tr.x)+(tl.y-tr.y)*(tl.y-tr.y);
        double d2 = (tr.x-br.x)*(tr.x-br.x)+(tr.y-br.y)*(tr.y-br.y);
        double d3 = (br.x-bl.x)*(br.x-bl.x)+(br.y-bl.y)*(br.y-bl.y);
        double d4 = (bl.x-tl.x)*(bl.x-tl.x)+(bl.y-tl.y)*(bl.y-tl.y);
        /*
        // Checking "squareness"
        while( ( (0.8*d2 < d1 && d1 < 1.2*d2) || (0.8*d1 < d2 && d2 < 1.2*d1) ) &&
                ( (0.8*d3 < d2 && d2 < 1.2*d3) || (0.8*d2 < d3 && d3 < 1.2*d2) ) &&
                ( (0.8*d4 < d3 && d3 < 1.2*d4) || (0.8*d3 < d4 && d4 < 1.2*d3) ) &&
                ( (0.8*d1 < d4 && d4 < 1.2*d1) || (0.8*d4 < d1 && d1 < 1.2*d4) ) &&
                biggestContoursIndex+1 < contours.size() ){
            biggestContoursIndex++;
            corners = getCorners(contours[biggestContoursIndex]);
            tl = corners[0];
            tr = corners[1];
            bl = corners[2];
            br = corners[3];
            center = Point((tl.x+tr.x+bl.x+br.x)/4,(tl.y+tr.y+bl.y+br.y)/4);
            d1 = (tl.x-tr.x)*(tl.x-tr.x)+(tl.y-tr.y)*(tl.y-tr.y);
            d2 = (tr.x-br.x)*(tr.x-br.x)+(tr.y-br.y)*(tr.y-br.y);
            d3 = (br.x-bl.x)*(br.x-bl.x)+(br.y-bl.y)*(br.y-bl.y);
            d4 = (bl.x-tl.x)*(bl.x-tl.x)+(bl.y-tl.y)*(bl.y-tl.y);
        }
        */
        //vector<Point> contour_poly;
        //approxPolyDP( Mat(contours[biggestContoursIndex]), contour_poly, 3, true );
        circle( sudoku, tl, 4, Scalar(255), -1, 10, 0 );
        circle( sudoku, tr, 4, Scalar(255), -1, 10, 0 );
        circle( sudoku, bl, 4, Scalar(255), -1, 10, 0 );
        circle( sudoku, br, 4, Scalar(255), -1, 10, 0 );
        drawContours( mRgb, contours, biggestContoursIndex, Scalar(255,0,0), 1, 8, vector<Vec4i>(), 0, Point() );

        Point2f inputQuad[4];
        Point2f outputQuad[4];
        inputQuad[0] = tl;
        inputQuad[1] = tr;
        inputQuad[2] = br;
        inputQuad[3] = bl;

        outputQuad[0] = Point2f( 0,0 );
        outputQuad[1] = Point2f( mRgb.cols-1,0);
        outputQuad[2] = Point2f( mRgb.cols-1,mRgb.rows-1);
        outputQuad[3] = Point2f( 0,mRgb.rows-1  );

        Mat lambda( 2, 4, CV_32FC1 );
        lambda = Mat::zeros( sudoku.rows, sudoku.cols, sudoku.type() );
        lambda = getPerspectiveTransform( inputQuad, outputQuad );
        warpPerspective(sudoku,sudoku,lambda,sudoku.size() );

        /*for (int i = 1; i < 9; i++){
            //float x1 = bl.x + (tl.x - bl.x) * float(i) / float(9.0);
            //float y1 = bl.y + (tl.y - bl.y) * float(i) / float(9.0);
            //float x2 = br.x + (tr.x - br.x) * float(i) / float(9.0);
            //float y2 = br.y + (tr.y - br.y) * float(i) / float(9.0);
            //auto x = static_cast<float>(float(mRgb.cols * i) / 9.0);
            auto y = static_cast<float>(float(mRgb.rows * i) / 9.0);
            float x1 = 0;
            float y1 = y;
            auto x2 = static_cast<float>(mRgb.cols);
            float y2 = y;
            line(mRgb, Point(static_cast<int>(x1), static_cast<int>(y1)),
                    Point(static_cast<int>(x2), static_cast<int>(y2)), Scalar(255,0,255), 1, CV_AA);
        }*/

        int numbers[9][9] = {
                {0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0} };

        auto w = static_cast<float>(sudoku.cols/9.0);
        auto h = static_cast<float>(sudoku.rows/9.0);
        //line(mRgb, Point(static_cast<int>(w), static_cast<int>(h)),
        //     Point(static_cast<int>(2*w), static_cast<int>(2*h)), Scalar(255,0,255), 1, CV_AA);


        Rect roi(static_cast<int>(w), static_cast<int>(h),
                 static_cast<int>(w), static_cast<int>(h));
        Mat cropped(sudoku, roi);
        // Resize image.
        Mat resized;
        cv::resize(cropped, resized, Size(28,28));

        //if (dst.cols == 28 && dst.rows == 28){
        const double IN_SCALE_FACTOR = 0.003921;
        const double MEAN_VAL = 127.5;
        Mat blob;
        //dnn::blobFromImage(resized, blob, 1.0f, Size(28,28));//,
                //Scalar(MEAN_VAL), false, false, CV_32F);


        //Mat blob;
        //normalize(resized, blob, 0, 1);


        /*Mat blob(resized.size(), CV_32FC1);
        for (int i = 0;  i < resized.cols; i++) {
            for (int j = 0; j < resized.rows; j++){
                blob.at<float>(i, j) = static_cast<float>(resized.at<uint8_t>(i, j) / 255.0);
            }
        }*/

        string mys = to_string(resized.size().height);
        //putText(mRgb, mys, Point(20,200), FONT_HERSHEY_SIMPLEX, 4, Scalar(0,255,0),2,LINE_AA);
        mys = to_string(resized.size().width);
        //putText(mRgb, mys, Point(20,400), FONT_HERSHEY_SIMPLEX, 4, Scalar(0,255,0),2,LINE_AA);
        mys = to_string(resized.channels());
        //putText(mRgb, mys, Point(20,600), FONT_HERSHEY_SIMPLEX, 4, Scalar(0,255,0),2,LINE_AA);

        //if (blob.cols == 28 && blob.rows == 28){
        //line(mRgb, Point(static_cast<int>(0), static_cast<int>(0)),
        //     Point(static_cast<int>(mRgb.cols), static_cast<int>(mRgb.rows)), Scalar(255,0,255), 1, CV_AA);
        //net.setInput(resized);
        //vector<Mat> outs;
        //Mat detections;
        //net.forward("dense_2/Softmax"); //"dense_2/Softmax"
        //}
        //}



    }
    return 1;
}

jint
Java_hu_marci_customsolutions_sudokusolveranddigitizer_MainActivity_imgProc(JNIEnv*, jobject,
        jlong inputMat, jlong mRgbaflipped, jlong imageGray, jlong flippedGray)//,
        //jlong myNetAddress)
        {
    Mat& mRgb = *(Mat*) inputMat;
    Mat& mRgbaFlipped = *(Mat*) mRgbaflipped;
    Mat& mGray = *(Mat*) imageGray;
    Mat& mFlippedGray = *(Mat*) flippedGray;
    //dnn::Net& myNet = *(dnn::Net*) myNetAddress;
    int success = toGray(mRgb, mRgbaFlipped, mGray, mFlippedGray);
    detectSusoku(/*myNet,*/ mRgbaFlipped, mFlippedGray);
    return (jint)success;

}

}