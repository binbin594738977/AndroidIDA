package com.example.androidida;

import android.os.Bundle;
import android.os.PersistableBundle;
import android.util.Log;
import android.view.View;

import java.lang.reflect.Method;
import java.util.ArrayList;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "xxxMainActivity";

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState, @Nullable PersistableBundle persistentState) {
        super.onCreate(savedInstanceState, persistentState);
    }

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        findViewById(R.id.btn1).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String str = JniMethods.callNativeMethod("hello jni");
                Log.i(TAG, "我输入的是 -> " + str);
            }
        });

        findViewById(R.id.btn2).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    ArrayList<Method> methodList = new ArrayList<>();
                    Method callNativeMethod = JniMethods.class.getDeclaredMethod("callNativeMethod", String.class);
                    callNativeMethod.setAccessible(true);
                    methodList.add(callNativeMethod);
                    JniHook.hookNativeMethods(methodList.toArray());
                    Log.i(TAG, "------------------------------------ ");
                } catch (Exception e) {
                    Log.i(TAG, "onClick: ", e);
                }
            }
        });
    }


}
