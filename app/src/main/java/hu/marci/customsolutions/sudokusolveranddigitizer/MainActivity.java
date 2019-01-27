package hu.marci.customsolutions.sudokusolveranddigitizer;


import android.Manifest;
import android.app.ActivityOptions;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.graphics.Rect;
import android.hardware.Camera;
import android.os.Build;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceView;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ProgressBar;
import android.widget.SeekBar;
import android.widget.TextView;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.JavaCameraView;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.dnn.Dnn;
import org.opencv.dnn.Net;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;


public class MainActivity extends AppCompatActivity implements CameraBridgeViewBase.CvCameraViewListener2,
        View.OnTouchListener {

    public static final String EXTRA_MESSAGE = "hu.marci.simplesolutions.sudokusolveranddigitizer.MESSAGE";
    private static String TAG = "SudokuSolverandDigitizer/MainActivity";

    Mat mRgba;
    Mat mRgbaFlipped;
    Mat mGray;
    Mat mflippedGray;

    private Net myNet;

    private TextView tv;
    private ProgressBar progressBar;

    Camera mCamera;

    //OpenCvJavaCameraView javaCameraView;
    OpenCvJavaCameraView javaCameraView;
    BaseLoaderCallback mBaseLoaderCallBack = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            super.onManagerConnected(status);
            switch (status){
                case BaseLoaderCallback.SUCCESS: {
                    javaCameraView.enableView();
                    javaCameraView.setOnTouchListener(MainActivity.this);
                    break;
                }
                default:{
                    super.onManagerConnected(status);
                    break;
                }
            }
        }
    };

    int numbers[] = new int[]{
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0 };


    public void startSolver(View view) {
        //progressBar.setVisibility(View.VISIBLE);
        Intent intent = new Intent(this, SolverActivity.class);
        // we calc the numbers in native, and only pass through the array of numbers
        intent.putExtra("numbers", numbers);
        Log.d("this is my array", "arr: " + Arrays.toString(numbers));
        ActivityOptions options =
                ActivityOptions.makeCustomAnimation(getApplicationContext(), R.anim.slide_in_from_right, R.anim.slide_out_to_left);
        startActivity(intent, options.toBundle());
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        //setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);

        setContentView(R.layout.activity_main);


        // https://developer.android.com/training/permissions/requesting
        //Check if permission is already granted
        //thisActivity is your activity. (e.g.: MainActivity.this)
        if (ContextCompat.checkSelfPermission(this,
                Manifest.permission.CAMERA)
                != PackageManager.PERMISSION_GRANTED) {

            // Give first an explanation, if needed.
            if (ActivityCompat.shouldShowRequestPermissionRationale(this,
                    Manifest.permission.CAMERA)) {

                // Show an explanation to the user *asynchronously* -- don't block
                // this thread waiting for the user's response! After the user
                // sees the explanation, try again to request the permission.

            } else {

                // No explanation needed, we can request the permission.

                ActivityCompat.requestPermissions(this,
                        new String[]{Manifest.permission.CAMERA},
                        1);
            }
        }



        javaCameraView = findViewById(R.id.camera_view);
        javaCameraView.setVisibility(SurfaceView.VISIBLE);
        javaCameraView.setCvCameraViewListener(this);
        //javaCameraView.setFocusMode(getApplicationContext(), 0);


        // Example of a call to a native method
        tv = findViewById(R.id.tv);
        tv.setText(stringFromJNI());

        progressBar = findViewById(R.id.progressBar);
        progressBar.setVisibility(View.INVISIBLE);

    }

    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           String permissions[], int[] grantResults) {
        switch (requestCode) {
            case 1: {
                // If request is cancelled, the result arrays are empty.
                if (grantResults.length > 0
                        && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    // permission was granted, yay! Do the
                    // contacts-related task you need to do.
                } else {
                    // permission denied, boo! Disable the
                    // functionality that depends on this permission.
                }
                return;
            }

            // other 'case' lines to check for other
            // permissions this app might request.
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (OpenCVLoader.initDebug()){
            Log.i(TAG, "OpenCV loaded successfully");
            mBaseLoaderCallBack.onManagerConnected(LoaderCallbackInterface.SUCCESS);
        }
        else{
            Log.i(TAG, "OpenCV not loaded");
            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_4_0, this, mBaseLoaderCallBack);
            mBaseLoaderCallBack.onManagerConnected(LoaderCallbackInterface.SUCCESS);
        }
    }

    @Override
    protected void onPause()
    {
        super.onPause();
        if (javaCameraView != null) {
            javaCameraView.disableView();
        }
        progressBar.setVisibility(View.INVISIBLE);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (javaCameraView != null) {
            javaCameraView.disableView();
        }
        if (mCamera != null){
            mCamera.release();
        }
    }


    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }


    public native boolean imgProc(long inputFrame, long mRgbaFlipped, long imageGray, long flipped,
                              long myNetAddress, int[]numbers);



    @Override
    public void onCameraViewStarted(int width, int height) {

        mRgba = new Mat(height, width, CvType.CV_8UC4);
        mRgbaFlipped = new Mat(width, height, CvType.CV_8UC4);
        mGray = new Mat(height, width, CvType.CV_8UC1);
        mflippedGray = new Mat(width, height, CvType.CV_8UC1);
        String model = getPath("opt_my_convnet.pb", this);
        String config = getPath("new_my_convnet_model.pbtxt", this);
        myNet = Dnn.readNetFromTensorflow(model, config);
        Log.i(TAG, "DNN read successfully");
    }

    private static String getPath(String file, Context context) {
        AssetManager assetManager = context.getAssets();
        BufferedInputStream inputStream = null;
        try {
            // Read data from assets.
            inputStream = new BufferedInputStream(assetManager.open(file));
            byte[] data = new byte[inputStream.available()];
            inputStream.read(data);
            inputStream.close();
            // Create copy file in storage.
            File outFile = new File(context.getFilesDir(), file);
            FileOutputStream os = new FileOutputStream(outFile);
            os.write(data);
            os.close();
            // Return a path to file which may be read in common way.
            return outFile.getAbsolutePath();
        } catch (IOException ex) {
            Log.i(TAG, "Failed to upload a file");
        }
        return "";
    }

    @Override
    public void onCameraViewStopped() {
        mRgba.release();
    }

    @Override
    public Mat onCameraFrame(CameraBridgeViewBase.CvCameraViewFrame inputFrame) {
        mRgba = inputFrame.rgba();
        boolean detectedASudoku = imgProc(mRgba.getNativeObjAddr(), mRgbaFlipped.getNativeObjAddr(), mGray.getNativeObjAddr(),
                mflippedGray.getNativeObjAddr(), myNet.getNativeObjAddr(), numbers);

        if (detectedASudoku){
            return mflippedGray;
        }

        return mRgbaFlipped;
    }


    @Override
    public boolean onTouch(View arg0, MotionEvent event) {

        double cols = mflippedGray.cols();
        double rows = mflippedGray.rows();

        double xOffset = (javaCameraView.getWidth() - cols) / 2;
        double yOffset = (javaCameraView.getHeight() - rows) / 2;

        double x = (((event).getX() - xOffset) /cols);
        double y = (((event).getY() - yOffset) / rows);

        //startSolver(arg0);

        //javaCameraView.focusOnArea();

        return false;
    }

}
