package hu.marci.customsolutions.sudokusolveranddigitizer;


import android.content.Context;
import android.content.Intent;
import android.content.res.AssetManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceView;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.TextView;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.JavaCameraView;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.Point;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.dnn.Dnn;
import org.opencv.dnn.Net;
import org.opencv.imgproc.Imgproc;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import hu.marci.customsolutions.sudokusolveranddigitizer.models.Classification;
import hu.marci.customsolutions.sudokusolveranddigitizer.models.Classifier;
import hu.marci.customsolutions.sudokusolveranddigitizer.models.TensorFlowClassifier;

public class MainActivity extends AppCompatActivity implements CameraBridgeViewBase.CvCameraViewListener2,
        View.OnTouchListener {

    private static final int PIXEL_WIDTH = 28;

    public static final String EXTRA_MESSAGE = "hu.marci.simplesolutions.sudokusolveranddigitizer.MESSAGE";
    private static String TAG = "SudokuSolverandDigitizer/MainActivity";

    Mat mRgba;
    Mat mRgbaFlipped;
    Mat mGray;
    Mat mflippedGray;

    private Net myNet;

    private Classifier mClassifier;

    TextView tv;

    //OpenCvJavaCameraView javaCameraView;
    JavaCameraView javaCameraView;
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

    int x_num = 8;
    int y_num = 1;
    float offset_ = (float) (0.0/10);

    public void startSolver(View view) {

        float w = (float) (((float) mflippedGray.size().width)/9.0);
        float h = (float) (((float) mflippedGray.size().height)/9.0);

        float[] pixels =  new float[28*28];

        float offset = offset_ *w;
        Rect rectCrop0 = new Rect((int)(x_num*w+offset), (int)(y_num*h+offset), (int)(w-2*offset), (int)(h-2*offset));
        Mat cropped0 = new Mat(mflippedGray,rectCrop0);

        Mat resized0 = new Mat();
        Size sz0 = new Size(28,28);
        Imgproc.resize( cropped0, resized0, sz0 );

        for(int k = 0; k < resized0.rows(); k++){
            for(int l= 0; l < resized0.cols(); l++){
                pixels[k*28+l] = (float) (( 255.0 - (float)resized0.get(k,l)[0])/255.0);
            }
        }

        String text = "";

        final Classification res0 = mClassifier.recognize(pixels);
        if (res0.getLabel() == null) {
            text += mClassifier.name() + ": ?\n";
        } else {
            //else output its name
            text += String.format("%s: %s, %f\n", mClassifier.name(), res0.getLabel(),
                    res0.getConf());
            tv.setText(String.format("Best choice: %s, with %s confidence.",
                    res0.getLabel(), res0.getConf()));
        }
        Log.i(TAG,text);


        if(3-2 == 1)
            return;

        for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++){
                // Crop region of interest.
                Rect rectCrop = new Rect((int)(i*w), (int)(j*h), (int)w, (int)h);
                Mat cropped = new Mat(mflippedGray,rectCrop);

                Mat resized = new Mat();
                Size sz = new Size(28,28);
                Imgproc.resize( cropped, resized, sz );

                for(int k = 0; k < resized.size().width; k++){
                    for(int l= 0; l < resized.size().height; l++){
                        pixels[k*9+1] = (float)resized.get(k,l)[0];
                    }
                }

                String text0 = "";

                final Classification res = mClassifier.recognize(pixels);
                //if it can't classify, output a question mark
                if (res.getLabel() == null) {
                    text += mClassifier.name() + ": ?\n";
                } else {
                    //else output its name
                    text += String.format("%s: %s, %f\n", mClassifier.name(), res.getLabel(),
                            res.getConf());

                }
                Log.i(TAG,text);
            }
        }


        //Intent intent = new Intent(this, SolverActivity.class);
        // we calc the numbers in native, and only pass through the array of numbers
        //intent.putExtra(EXTRA_MESSAGE, mflippedGray.getNativeObjAddr());
        //startActivity(intent);
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        //setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);

        setContentView(R.layout.activity_main);

        javaCameraView = findViewById(R.id.camera_view);
        javaCameraView.setVisibility(SurfaceView.VISIBLE);
        javaCameraView.setCvCameraViewListener(this);
        //javaCameraView.setFocusMode(getApplicationContext(), 0);

        // Example of a call to a native method
        tv = findViewById(R.id.tv);
        tv.setText(stringFromJNI());



        loadModel();
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
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (javaCameraView != null) {
            javaCameraView.disableView();
        }
    }


    //creates a model object in memory using the saved tensorflow protobuf model file
    //which contains all the learned weights
    private void loadModel() {
        //The Runnable interface is another way in which you can implement multi-threading other than extending the
        // //Thread class due to the fact that Java allows you to extend only one class. Runnable is just an interface,
        // //which provides the method run.
        // //Threads are implementations and use Runnable to call the method run().
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    //add 2 classifiers to our classifier arraylist
                    //the tensorflow classifier and the keras classifier
                    mClassifier =
                            TensorFlowClassifier.create(getAssets(), "Keras",
                                    "opt_mnist_convnet.pb", "labels.txt", PIXEL_WIDTH,
                                    "conv2d_1_input", "dense_2/Softmax", false);
                } catch (final Exception e) {
                    //if they aren't found, throw an error!
                    throw new RuntimeException("Error initializing classifiers!", e);
                }
            }
        }).start();
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


    public native int imgProc(long inputFrame, long mRgbaFlipped, long imageGray, long flipped);//,
                              //long myNetAddress);



    @Override
    public void onCameraViewStarted(int width, int height) {

        mRgba = new Mat(height, width, CvType.CV_8UC4);
        mRgbaFlipped = new Mat(width, height, CvType.CV_8UC4);
        mGray = new Mat(height, width, CvType.CV_8UC1);
        mflippedGray = new Mat(width, height, CvType.CV_8UC1);
        //String model = getPath("opt_mnist_convnet.pb", this);
        //String config = getPath("mnist_convnet_graph.pbtxt", this);
        //myNet = Dnn.readNetFromTensorflow(model, config);
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
        imgProc(mRgba.getNativeObjAddr(), mRgbaFlipped.getNativeObjAddr(), mGray.getNativeObjAddr(),
                mflippedGray.getNativeObjAddr()/*, myNet.getNativeObjAddr()*/);

        float w = (float) (((float) mflippedGray.size().width)/9.0);
        float h = (float) (((float) mflippedGray.size().height)/9.0);

        float offset = offset_ *w;
        Rect rect = new Rect((int)(x_num*w+offset), (int)(y_num*h+offset), (int)(w-2*offset), (int)(h-2*offset));
        Imgproc.rectangle(mflippedGray, new Point(rect.x, rect.y), new Point(rect.x + rect.width, rect.y + rect.height), new Scalar(255), 2);
        return mflippedGray;
    }


    @Override
    public boolean onTouch(View arg0, MotionEvent event) {

        double cols = mflippedGray.cols();
        double rows = mflippedGray.rows();

        double xOffset = (javaCameraView.getWidth() - cols) / 2;
        double yOffset = (javaCameraView.getHeight() - rows) / 2;

        double x = (((event).getX() - xOffset) /cols);
        double y = (((event).getY() - yOffset) / rows);

        x_num = (int) (9*x);
        y_num = (int) (9*y);

        startSolver(arg0);

        return false;// don't need subsequent touch events
    }

}
