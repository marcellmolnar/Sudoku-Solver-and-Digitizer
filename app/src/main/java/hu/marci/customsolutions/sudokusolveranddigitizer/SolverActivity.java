package hu.marci.customsolutions.sudokusolveranddigitizer;

import android.animation.AnimatorInflater;
import android.animation.AnimatorSet;
import android.animation.ObjectAnimator;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.view.animation.AnimationUtils;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.TextView;

import org.opencv.core.Mat;

public class SolverActivity extends AppCompatActivity {

    Mat img;
    boolean[] showNumber;
    Context context;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.solver_layout);

        // Get the Intent that started this activity and extract the string
        Intent intent = getIntent();

        context = getApplicationContext();

        showNumber = new boolean[81];
        for (int i = 0; i < 81; i++){
            showNumber[i] = false;
        }

        // Capture the layout's TextView and set the string as its text
        //TextView textView = findViewById(R.id.textView);
        //textView.setText(message);

        //long addr = intent.getLongExtra("myImg", 0);
        //Mat tempImg = new Mat( addr );
        //img = tempImg.clone();
    }

    public void flipCard(View v) {
        // showing number to not showing
        if (showNumber[getIDfromView(v)]) {
            //v.startAnimation(AnimationUtils.loadAnimation(context, R.anim.flip_to_back));
            //ObjectAnimator anim = (ObjectAnimator) AnimatorInflater.loadAnimator(context, R.animator.flip_back);
            //anim.setTarget(v);
            FrameLayout fl = (FrameLayout) v;

            ObjectAnimator anim;
            ObjectAnimator anim2;
            ObjectAnimator anim3;
            AnimatorSet set = new AnimatorSet();
            for (int i = 0; i < fl.getChildCount(); i++) {
                View child = fl.getChildAt(i);
                if (child instanceof ImageView){
                    anim = ObjectAnimator.ofFloat(child, "rotationY", 0f, -180f);
                    anim.setDuration(500);
                    anim.start();
                }
                if (child instanceof TextView){
                    //anim2 = ObjectAnimator.ofFloat(child, "rotationY", 0f, -180f);
                    //anim2.setDuration(500);
                    //anim2.start();
                    anim3 = ObjectAnimator.ofFloat(child,"alpha",1.f);
                    anim3.setDuration(250);
                    anim3.start();
                }
            }
        }
        // not showing number to showing
        else{
            //v.startAnimation(AnimationUtils.loadAnimation(context, R.anim.flip_to_number));
            //ObjectAnimator anim = (ObjectAnimator) AnimatorInflater.loadAnimator(context, R.animator.flip_number);
            //anim.setTarget(v);
            FrameLayout fl = (FrameLayout) v;

            ObjectAnimator anim;
            ObjectAnimator anim2;
            ObjectAnimator anim3;
            for (int i = 0; i < fl.getChildCount(); i++) {
                View child = fl.getChildAt(i);
                if (child instanceof ImageView){
                    anim = ObjectAnimator.ofFloat(child, "rotationY", 180f, 0f);
                    anim.setDuration(500);
                    anim.start();
                }
                if (child instanceof TextView){
                    //anim2 = ObjectAnimator.ofFloat(child, "rotationY", 180f, 0f);
                    //anim2.setDuration(500);
                    //anim2.start();
                    anim3 = ObjectAnimator.ofFloat(child,"alpha",0.f);
                    anim3.setDuration(250);
                    anim3.start();
                }
            }
        }
        showNumber[getIDfromView(v)] = !showNumber[getIDfromView(v)];
    }


    private int getIDfromView(View v){
        switch (v.getId()){
            case R.id.frame0: case R.id.text0: {
                return 0;
            }
            case R.id.frame1: case R.id.text1: {
                return 1;
            }
            case R.id.frame2: case R.id.text2: {
                return 2;
            }
            case R.id.frame3: case R.id.text3: {
                return 3;
            }
            case R.id.frame4: case R.id.text4: {
                return 4;
            }
            case R.id.frame5: case R.id.text5: {
                return 5;
            }
            case R.id.frame6: case R.id.text6: {
                return 6;
            }
            case R.id.frame7: case R.id.text7: {
                return 7;
            }
            case R.id.frame8: case R.id.text8: {
                return 8;
            }
            case R.id.frame9: case R.id.text9: {
                return 9;
            }
            case R.id.frame10: case R.id.text10: {
                return 10;
            }
            case R.id.frame11: case R.id.text11: {
                return 11;
            }
            case R.id.frame12: case R.id.text12: {
                return 12;
            }
            case R.id.frame13: case R.id.text13: {
                return 13;
            }
            case R.id.frame14: case R.id.text14: {
                return 14;
            }
            case R.id.frame15: case R.id.text15: {
                return 15;
            }
            case R.id.frame16: case R.id.text16: {
                return 16;
            }
            case R.id.frame17: case R.id.text17: {
                return 17;
            }
            case R.id.frame18: case R.id.text18: {
                return 18;
            }
            case R.id.frame19: case R.id.text19: {
                return 19;
            }
            case R.id.frame20: case R.id.text20: {
                return 20;
            }
            case R.id.frame21: case R.id.text21: {
                return 21;
            }
            case R.id.frame22: case R.id.text22: {
                return 22;
            }
            case R.id.frame23: case R.id.text23: {
                return 23;
            }
            case R.id.frame24: case R.id.text24: {
                return 24;
            }
            case R.id.frame25: case R.id.text25: {
                return 25;
            }
            case R.id.frame26: case R.id.text26: {
                return 26;
            }
            case R.id.frame27: case R.id.text27: {
                return 27;
            }
            case R.id.frame28: case R.id.text28: {
                return 28;
            }
            case R.id.frame29: case R.id.text29: {
                return 29;
            }
            case R.id.frame30: case R.id.text30: {
                return 30;
            }
            case R.id.frame31: case R.id.text31: {
                return 31;
            }
            case R.id.frame32: case R.id.text32: {
                return 32;
            }
            case R.id.frame33: case R.id.text33: {
                return 33;
            }
            case R.id.frame34: case R.id.text34: {
                return 34;
            }
            case R.id.frame35: case R.id.text35: {
                return 35;
            }
            case R.id.frame36: case R.id.text36: {
                return 36;
            }
            case R.id.frame37: case R.id.text37: {
                return 37;
            }
            case R.id.frame38: case R.id.text38: {
                return 38;
            }
            case R.id.frame39: case R.id.text39: {
                return 39;
            }
            case R.id.frame40: case R.id.text40: {
                return 40;
            }
            case R.id.frame41: case R.id.text41: {
                return 41;
            }
            case R.id.frame42: case R.id.text42: {
                return 42;
            }
            case R.id.frame43: case R.id.text43: {
                return 43;
            }
            case R.id.frame44: case R.id.text44: {
                return 44;
            }
            case R.id.frame45: case R.id.text45: {
                return 45;
            }
            case R.id.frame46: case R.id.text46: {
                return 46;
            }
            case R.id.frame47: case R.id.text47: {
                return 47;
            }
            case R.id.frame48: case R.id.text48: {
                return 48;
            }
            case R.id.frame49: case R.id.text49: {
                return 49;
            }
            case R.id.frame50: case R.id.text50: {
                return 50;
            }
            case R.id.frame51: case R.id.text51: {
                return 51;
            }
            case R.id.frame52: case R.id.text52: {
                return 52;
            }
            case R.id.frame53: case R.id.text53: {
                return 53;
            }
            case R.id.frame54: case R.id.text54: {
                return 54;
            }
            case R.id.frame55: case R.id.text55: {
                return 55;
            }
            case R.id.frame56: case R.id.text56: {
                return 56;
            }
            case R.id.frame57: case R.id.text57: {
                return 57;
            }
            case R.id.frame58: case R.id.text58: {
                return 58;
            }
            case R.id.frame59: case R.id.text59: {
                return 59;
            }
            case R.id.frame60: case R.id.text60: {
                return 60;
            }
            case R.id.frame61: case R.id.text61: {
                return 61;
            }
            case R.id.frame62: case R.id.text62: {
                return 62;
            }
            case R.id.frame63: case R.id.text63: {
                return 63;
            }
            case R.id.frame64: case R.id.text64: {
                return 64;
            }
            case R.id.frame65: case R.id.text65: {
                return 65;
            }
            case R.id.frame66: case R.id.text66: {
                return 66;
            }
            case R.id.frame67: case R.id.text67: {
                return 67;
            }
            case R.id.frame68: case R.id.text68: {
                return 68;
            }
            case R.id.frame69: case R.id.text69: {
                return 69;
            }
            case R.id.frame70: case R.id.text70: {
                return 70;
            }
            case R.id.frame71: case R.id.text71: {
                return 71;
            }
            case R.id.frame72: case R.id.text72: {
                return 72;
            }
            case R.id.frame73: case R.id.text73: {
                return 73;
            }
            case R.id.frame74: case R.id.text74: {
                return 74;
            }
            case R.id.frame75: case R.id.text75: {
                return 75;
            }
            case R.id.frame76: case R.id.text76: {
                return 76;
            }
            case R.id.frame77: case R.id.text77: {
                return 77;
            }
            case R.id.frame78: case R.id.text78: {
                return 78;
            }
            case R.id.frame79: case R.id.text79: {
                return 79;
            }
            case R.id.frame80: case R.id.text80: {
                return 80;
            }
            default:{
                return -1;
            }
        }
    }

}
