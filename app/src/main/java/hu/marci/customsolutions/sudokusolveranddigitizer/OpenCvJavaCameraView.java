package hu.marci.customsolutions.sudokusolveranddigitizer;

import android.content.Context;
import android.graphics.Rect;
import android.hardware.Camera;
import android.util.AttributeSet;
import android.util.Log;
import android.widget.Toast;

import org.opencv.android.JavaCameraView;

import java.util.ArrayList;
import java.util.List;

public class OpenCvJavaCameraView extends JavaCameraView {

    private static String TAG = "SudokuSolverandDigitizer/JavaCameraView";

    public OpenCvJavaCameraView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }


    public void focusOnArea(){
        // set Camera parameters
        Camera.Parameters params = mCamera.getParameters();

        if (params.getMaxNumMeteringAreas() > 0){ // check that metering areas are supported
            List<Camera.Area> meteringAreas = new ArrayList<Camera.Area>();

            Rect areaRect1 = new Rect(-100, -100, 100, 100);    // specify an area in center of image
            meteringAreas.add(new Camera.Area(areaRect1, 600)); // set weight to 60%

            params.setMeteringAreas(meteringAreas);
        }

        mCamera.setParameters(params);
        Log.i(TAG, "focused");
    }


    /*public void setFocusMode(Context item, int type) {

        Camera mCamera = Camera.open();
        Camera.Parameters params = mCamera.getParameters();
        mCamera.cancelAutoFocus();
        mCamera.autoFocus(new Camera.AutoFocusCallback() {
            @Override
            public void onAutoFocus(boolean b, Camera camera) {
            }
        });

        List<String> FocusModes = params.getSupportedFocusModes();

        switch (type) {
            case 0:
                if (FocusModes.contains(Camera.Parameters.FOCUS_MODE_AUTO))
                    params.setFocusMode(Camera.Parameters.FOCUS_MODE_AUTO);
                else
                    Toast.makeText(item, "Auto Mode is not supported", Toast.LENGTH_SHORT).show();
                break;
            case 1:
                if (FocusModes.contains(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO))
                    params.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO);
                else
                    Toast.makeText(item, "Continuous Mode is not supported", Toast.LENGTH_SHORT).show();
                break;
            case 2:
                if (FocusModes.contains(Camera.Parameters.FOCUS_MODE_EDOF))
                    params.setFocusMode(Camera.Parameters.FOCUS_MODE_EDOF);
                else
                    Toast.makeText(item, "EDOF Mode is not supported", Toast.LENGTH_SHORT).show();
                break;
            case 3:
                if (FocusModes.contains(Camera.Parameters.FOCUS_MODE_FIXED))
                    params.setFocusMode(Camera.Parameters.FOCUS_MODE_FIXED);
                else
                    Toast.makeText(item, "Fixed Mode is not supported", Toast.LENGTH_SHORT).show();
                break;
            case 4:
                if (FocusModes.contains(Camera.Parameters.FOCUS_MODE_INFINITY))
                    params.setFocusMode(Camera.Parameters.FOCUS_MODE_INFINITY);
                else
                    Toast.makeText(item, "Infinity Mode is not supported", Toast.LENGTH_SHORT).show();
                break;
            case 5:
                if (FocusModes.contains(Camera.Parameters.FOCUS_MODE_MACRO))
                    params.setFocusMode(Camera.Parameters.FOCUS_MODE_MACRO);
                else
                    Toast.makeText(item, "Macro Mode is not supported", Toast.LENGTH_SHORT).show();
                break;
        }

        mCamera.setParameters(params);
    }*/

}
