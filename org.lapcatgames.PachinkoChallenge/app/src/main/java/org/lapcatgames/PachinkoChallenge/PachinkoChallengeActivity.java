package org.lapcatgames.PachinkoChallenge;

import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.widget.RelativeLayout;
import android.view.View;

import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.MobileAds;

import org.libsdl.app.SDLActivity;

public class PachinkoChallengeActivity extends SDLActivity
{
    // Setup
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        this.setRequestedOrientation( ActivityInfo.SCREEN_ORIENTATION_PORTRAIT );

        // Add AdMob support
        String adId = "ca-app-pub-3940256099942544/6300978111";
        MobileAds.initialize(this, adId);
        AdView mAdView = new AdView(this);
        mAdView.setAdSize(AdSize.SMART_BANNER);
        mAdView.setAdUnitId(adId);
        AdRequest adRequest = new AdRequest.Builder()
                //.addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
                .build();
        mAdView.loadAd(adRequest);

        // Add the layout of the AdMob banner
        RelativeLayout.LayoutParams params = new RelativeLayout
                .LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
        params.addRule(RelativeLayout.CENTER_HORIZONTAL);
        params.addRule(RelativeLayout.ALIGN_PARENT_TOP);
        mAdView.setLayoutParams(params);

        mLayout.addView(mAdView);
    }
}
