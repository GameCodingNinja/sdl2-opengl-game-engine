package com.hugheswhoproductions.hugheswhoslots;

import org.libsdl.app.SDLActivity;
import android.content.pm.ActivityInfo;
import android.os.Bundle;

public class hugheswhoslotsActivity extends SDLActivity
{
    // Setup
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
    }
}
