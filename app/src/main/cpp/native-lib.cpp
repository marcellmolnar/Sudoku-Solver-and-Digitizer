#include <jni.h>
#include <string>
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

extern "C"{
jstring
Java_hu_marci_customsolutions_sudokusolveranddigitizer_MainActivity_stringFromJNI(JNIEnv* env, jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

int toGray(Mat img, Mat& gray, Mat& flipped){
    cvtColor(img, gray, CV_RGB2GRAY);
    transpose(gray, flipped);
    flip(flipped, flipped, +1);
    if (gray.rows == img.rows && gray.cols == img.cols){
        return 1;
    }
    else{
        return 0;
    }
}

int detectSusoku(Mat& sudoku){
    GaussianBlur(sudoku, sudoku, Size(11,11), 0);
    adaptiveThreshold(sudoku, sudoku, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 2);
    bitwise_not(sudoku, sudoku);
    //float kdata[] = {0, 1, 0, 1, 1, 1, 0, 1, 0};
    //Mat kernel(3,3,CV_8UC1, kdata);
    //dilate(sudoku, sudoku, kernel);
    return 1;
}

jint
Java_hu_marci_customsolutions_sudokusolveranddigitizer_MainActivity_imgProc(JNIEnv* env, jclass,
        jlong inputMat, jlong imageGray, jlong flipped) {
    Mat& rgb = *(Mat*) inputMat;
    Mat& gray = *(Mat*) imageGray;
    Mat& mFlipped = *(Mat*) flipped;
    int success = toGray(rgb, gray, mFlipped);
    detectSusoku(mFlipped);
    return (jint)success;

}

}