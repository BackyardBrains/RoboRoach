package com.backyardbrains.roboroach;

import android.app.Activity;
import android.app.Fragment;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.content.Intent;
import android.graphics.Typeface;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.GestureDetector;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.SeekBar;
import android.widget.Toast;


public class RoboRoachSettingsActivity extends Activity implements RoboRoachManagerCallbacks {
    public static final String EXTRAS_DEVICE_ADDRESS = "BLE_DEVICE_ADDRESS";

    private final static String TAG = RoboRoachSettingsActivity.class.getSimpleName();

    private Handler mHandler = new Handler();
    private RoboRoachManager mRoboRoachManager = null;
    private ViewHolder viewHolder;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d(TAG, "onCreate()");

        setContentView(R.layout.roboroach_settings);

        mRoboRoachManager = new RoboRoachManager(this, this);

        // check if we have BT and BLE on board
        if(mRoboRoachManager.checkBleHardwareAvailable() == false) {
            bleMissing();
        }

    }


    @Override
    protected void onResume() {
        super.onResume();
        Log.d(TAG, "onResume()");

        if(!mRoboRoachManager.initialize()) {
            finish();
        }
        invalidateOptionsMenu();

    };


    @Override
    protected void onPause() {
        super.onPause();
        Log.d(TAG, "onPause()");



        if (mRoboRoachManager.isConnected()) {

            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    mRoboRoachManager.stopMonitoringRssiValue();
                    mRoboRoachManager.disconnect();
                    mRoboRoachManager.close();
                }
            });

        }

    };


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {

        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        return true;
    }

    /**
     * A placeholder fragment containing a simple view.
     */
    public static class PlaceholderFragment extends Fragment {

        public PlaceholderFragment() {
        }

        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container,
                Bundle savedInstanceState) {
            View rootView = inflater.inflate(R.layout.fragment_roboroach_main, container, false);
            return rootView;
        }
    }

    public void uiDeviceConnected(final BluetoothGatt gatt,
                                  final BluetoothDevice device)
    {
        Log.d(TAG, "uiDeviceConnected()");
    }

    public void uiDeviceDisconnected(final BluetoothGatt gatt,
                                     final BluetoothDevice device)
    {
        Log.d(TAG, "uiDeviceDisconnected()");

        invalidateOptionsMenu();
    }

    @Override
    public void uiServicesFound() {


        mRoboRoachManager.requestRoboRoachParameters();
    }

    @Override
    public void uiRoboRoachPropertiesUpdated() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {

            }
        });
    }

    @Override
    public void uiDeviceFound(BluetoothDevice device, int rssi, byte[] record) {

    }


    static class ViewHolder {
        ImageView stimImage;
        SeekBar sbFrequency;
        SeekBar sbGain;
        SeekBar sbPulseWidth;
        SeekBar sbDuration;

    }


    @Override
    public void uiLeftTurnSentSuccessfully(final int stimulusDuration) {

    }

    @Override
    public void uiRightTurnSentSuccessfully(final int stimulusDuration) {

    }


    private void btDisabled() {
        Toast.makeText(this, "Sorry, Your bluetooth needs to be turned ON for your RoboRoach to work!", Toast.LENGTH_LONG).show();
        finish();
    }

    private void bleMissing() {
        Toast.makeText(this, "BLE Hardware is required for your RoboRoach. Please try on another device.", Toast.LENGTH_LONG).show();
        finish();
    }


}


