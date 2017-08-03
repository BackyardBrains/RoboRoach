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
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ViewFlipper;


public class RoboRoachActivity extends Activity implements RoboRoachManagerCallbacks {
    private static final long SCANNING_TIMEOUT = 5 * 1000; /* 5 seconds */
    private static final int ENABLE_BT_REQUEST_ID = 1;

    private final static String TAG = RoboRoachActivity.class.getSimpleName();
    private boolean mScanning = false;
    private boolean mTurning = false;
    private boolean mOnSettingsScreen = false;


    private String mDeviceAddress;

    private Handler mHandler = new Handler();
    private RoboRoachManager mRoboRoachManager = null;
    private ViewHolder viewHolder;
    private Runnable mGATTUpdate;
    private int mGATTFreq = 0;

    private GestureDetector gestureDetector;

    Runnable scanTimeout = new Runnable() {
        @Override
        public void run() {
            if (mRoboRoachManager == null) return;
            mScanning = false;
            mRoboRoachManager.stopScanning();
            invalidateOptionsMenu();
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d(TAG, "onCreate()");


        setContentView(R.layout.roboroach_main);

        mRoboRoachManager = new RoboRoachManager(this, this);

        // check if we have BT and BLE on board
        if (mRoboRoachManager.checkBleHardwareAvailable() == false) {
            bleMissing();
        }

        viewHolder = new ViewHolder();
        viewHolder.roachImage = (ImageView) findViewById(R.id.imageRoach);
        viewHolder.backpackImage = (ImageView) findViewById(R.id.imageBackpack);
        viewHolder.goLeftText = (TextView) findViewById(R.id.textGoLeft);
        viewHolder.goRightText = (TextView) findViewById(R.id.textGoRight);
        viewHolder.configText = (TextView) findViewById(R.id.textConfig);
        viewHolder.Duration = (SeekBar) findViewById(R.id.sbDuration);
        viewHolder.Gain = (SeekBar) findViewById(R.id.sbGain);
        viewHolder.Frequecy = (SeekBar) findViewById(R.id.sbFrequency);
        viewHolder.PulseWidth = (SeekBar) findViewById(R.id.sbPulseWidth);
        viewHolder.RandomMode = (Switch) findViewById(R.id.swRandomMode);

        viewHolder.roachImage.setVisibility(View.VISIBLE);
        viewHolder.backpackImage.setVisibility(View.INVISIBLE);
        viewHolder.goLeftText.setVisibility(View.INVISIBLE);
        viewHolder.goRightText.setVisibility(View.INVISIBLE);


        viewHolder.Frequecy.setMax(150);
        viewHolder.Frequecy.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            public void onProgressChanged(SeekBar seekBar, int progress,
                                          boolean fromUser) {
            }

            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            public void onStopTrackingTouch(SeekBar seekBar) {

                mGATTFreq = seekBar.getProgress();
                if (mGATTFreq < 1) mGATTFreq = 1;

                //If the new freq it's greater than 1/2 the
                if ((float) mRoboRoachManager.getRoboRoachPulseWidth() > (float) 500 / mGATTFreq) {
                    mGATTUpdate = new Runnable() {
                        @Override
                        public void run() {

                            float newFreq = (float) (1000 / mGATTFreq);
                            int newPW = (int) newFreq / 2;
                            mRoboRoachManager.updatePulseWidth(newPW);
                            viewHolder.configText.setText(mRoboRoachManager.getRoboRoachConfigurationString());

                        }
                    };
                    mHandler.postDelayed(mGATTUpdate, 500);
                }

                viewHolder.PulseWidth.setMax(1000 / mGATTFreq);
                mRoboRoachManager.updateFrequency(mGATTFreq);
                viewHolder.configText.setText(mRoboRoachManager.getRoboRoachConfigurationString());


            }
        });
        viewHolder.Gain.setMax(100);
        viewHolder.Gain.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            public void onProgressChanged(SeekBar seekBar, int progress,
                                          boolean fromUser) {
            }

            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            public void onStopTrackingTouch(SeekBar seekBar) {

                float roundedGain = Math.round((float) seekBar.getProgress() / 5.0f) * 5.0f;
                mRoboRoachManager.updateGain((int) roundedGain);
                viewHolder.configText.setText(mRoboRoachManager.getRoboRoachConfigurationString());
            }
        });

        viewHolder.RandomMode.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (mRoboRoachManager.isConnected()) {

                    mRoboRoachManager.updateRandomMode(isChecked);
                    viewHolder.PulseWidth.setEnabled(!viewHolder.RandomMode.isChecked());
                    viewHolder.Frequecy.setEnabled(!viewHolder.RandomMode.isChecked());
                    viewHolder.configText.setText(mRoboRoachManager.getRoboRoachConfigurationString());
                }
            }
        });

        viewHolder.PulseWidth.setMax(50);
        viewHolder.PulseWidth.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            public void onProgressChanged(SeekBar seekBar, int progress,
                                          boolean fromUser) {
            }

            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            public void onStopTrackingTouch(SeekBar seekBar) {
                mRoboRoachManager.updatePulseWidth(seekBar.getProgress());
                viewHolder.configText.setText(mRoboRoachManager.getRoboRoachConfigurationString());
            }
        });


        viewHolder.Duration.setMax(1000);
        viewHolder.Duration.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            public void onProgressChanged(SeekBar seekBar, int progress,
                                          boolean fromUser) {
            }

            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            public void onStopTrackingTouch(SeekBar seekBar) {
                float roundedDuration = Math.round((float) seekBar.getProgress() / 10.0f) * 10.0f;
                //if ( roundedDuration < 10 ) roundedDuration = 10;

                mRoboRoachManager.updateDuration((int) roundedDuration);
                viewHolder.configText.setText(mRoboRoachManager.getRoboRoachConfigurationString());
            }
        });

        Typeface typeFace = Typeface.createFromAsset(getAssets(), "fonts/comicbook.ttf");
        viewHolder.goLeftText.setTypeface(typeFace);
        viewHolder.goRightText.setTypeface(typeFace);
        viewHolder.configText.setTypeface(typeFace);

        viewHolder.configText.setText("");

        final Button button = (Button) findViewById(R.id.btnSaveSettings);
        button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Log.d(TAG, "Updating RoboRoach Settings");
                mRoboRoachManager.updateFrequency(viewHolder.Frequecy.getProgress());


                // Perform action on click
                ViewFlipper vf = (ViewFlipper) findViewById(R.id.viewFlipper);
                vf.showNext();
                mOnSettingsScreen = false;
            }
        });


        gestureDetector = new GestureDetector(this.getBaseContext(),
                new SwipeGestureDetector());

        if (savedInstanceState == null) {
            getFragmentManager().beginTransaction()
                    .add(R.id.container, new PlaceholderFragment())
                    .commit();
        }

    }


    public void updateSettingConstraints() {


        //if ( self.roboRoach.randomMode.boolValue ){
        //    [freqSlider setEnabled:NO];
        //    [pulseWidthSlider setEnabled:NO];
        //}else{
        //    [freqSlider setEnabled:YES];
        //    [pulseWidthSlider setEnabled:YES];
        //}

    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.d(TAG, "onResume()");

        // on every Resume check if BT is enabled (user could turn it off while app was in background etc.)
        if (mRoboRoachManager.isBtEnabled() == false) {
            // BT is not turned on - ask user to make it enabled
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, ENABLE_BT_REQUEST_ID);
            // see onActivityResult to check what is the status of our request
        }

        if (!mRoboRoachManager.initialize()) {
            finish();
        }
        invalidateOptionsMenu();

    }

    ;


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

        } else if (mScanning) {
            mScanning = false;
            mRoboRoachManager.stopScanning();
        }

        invalidateOptionsMenu();

    }

    ;


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.main, menu);
        if (mRoboRoachManager.isConnected()) {
            menu.findItem(R.id.menu_stop).setVisible(false);
            menu.findItem(R.id.menu_scan).setVisible(false);
            menu.findItem(R.id.menu_refresh).setActionView(null);
            menu.findItem(R.id.menu_disconnect).setVisible(true);
            menu.findItem(R.id.menu_settings).setVisible(true);

        } else {
            menu.findItem(R.id.menu_disconnect).setVisible(false);
            menu.findItem(R.id.menu_settings).setVisible(false);

            if (mScanning) {
                menu.findItem(R.id.menu_stop).setVisible(true);
                menu.findItem(R.id.menu_scan).setVisible(false);
                menu.findItem(R.id.menu_refresh).setActionView(
                        R.layout.actionbar_progress);
            } else {
                menu.findItem(R.id.menu_stop).setVisible(false);
                menu.findItem(R.id.menu_scan).setVisible(true);
                menu.findItem(R.id.menu_refresh).setActionView(null);
            }
        }
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        switch (item.getItemId()) {
            case R.id.menu_scan:
                mScanning = true;
                invalidateOptionsMenu();
                mRoboRoachManager.startScanning();
                break;
            case R.id.menu_stop:
                mScanning = false;
                mRoboRoachManager.stopScanning();
                invalidateOptionsMenu();
                break;
            case R.id.menu_disconnect:
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        mRoboRoachManager.disconnect();
                        mRoboRoachManager.close();
                        if (mOnSettingsScreen) {
                            ViewFlipper vf = (ViewFlipper) findViewById(R.id.viewFlipper);
                            vf.showNext();
                            mOnSettingsScreen = false;
                        }
                        invalidateOptionsMenu();
                    }
                });
                break;
            case R.id.menu_settings:
                if (!mOnSettingsScreen) {
                    ViewFlipper vf = (ViewFlipper) findViewById(R.id.viewFlipper);
                    vf.showNext();
                    mOnSettingsScreen = true;
                }
                break;

        }
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
                                  final BluetoothDevice device) {
        Log.d(TAG, "uiDeviceConnected()");
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                //mDeviceStatus.setText("connected");
                viewHolder.backpackImage.setImageAlpha(150);
                viewHolder.backpackImage.setVisibility(View.VISIBLE);
                invalidateOptionsMenu();
            }
        });
    }

    public void uiDeviceDisconnected(final BluetoothGatt gatt,
                                     final BluetoothDevice device) {
        Log.d(TAG, "uiDeviceDisconnected()");
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                viewHolder.configText.setText("");
                viewHolder.backpackImage.setVisibility(View.INVISIBLE);
            }
        });

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

                viewHolder.configText.setText(mRoboRoachManager.getRoboRoachConfigurationString());
                viewHolder.backpackImage.setImageAlpha(255);
                viewHolder.backpackImage.setVisibility(View.VISIBLE);

                viewHolder.Frequecy.setProgress(mRoboRoachManager.getRoboRoachFrequency());
                viewHolder.Gain.setProgress(mRoboRoachManager.getRoboRoachGain());
                viewHolder.PulseWidth.setProgress(mRoboRoachManager.getRoboRoachPulseWidth());
                viewHolder.Duration.setProgress(mRoboRoachManager.getRoboRoachDuration());
                viewHolder.RandomMode.setChecked(mRoboRoachManager.getRoboRoachRandomMode());

                viewHolder.PulseWidth.setEnabled(!viewHolder.RandomMode.isChecked());
                viewHolder.Frequecy.setEnabled(!viewHolder.RandomMode.isChecked());

                invalidateOptionsMenu();
            }
        });
    }

    @Override
    public void uiDeviceFound(BluetoothDevice device, int rssi, byte[] record) {
        Log.d(TAG, "uiDeviceFound()");

        if (mHandler != null) {
            mHandler.removeCallbacks(scanTimeout);
        }

        if (device == null || device.getAddress() == null || device.getName() == null) return;

        mDeviceAddress = device.getAddress();

        if (device.getName().equals("RoboRoach")) {

            Log.d(TAG, "uiDeviceFound() ...Found a RoboRoach!");

            // adding to the UI have to happen in UI thread
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    viewHolder.backpackImage.setImageAlpha(60);  //slowly builds up until connection
                    viewHolder.backpackImage.setVisibility(View.VISIBLE);

                    Log.d(TAG, "uiDeviceFound() ... mDeviceAddress = " + mDeviceAddress);

                    if (mScanning) {
                        mScanning = false;
                        invalidateOptionsMenu();
                        mRoboRoachManager.stopScanning();
                        Log.d(TAG, "uiDeviceFound() ... mRoboRoachManager.stopScanning()");

                    }

                    Log.d(TAG, "uiDeviceFound() ... about to call mRoboRoachManager.connect()");
                    mRoboRoachManager.connect(mDeviceAddress);
                    Log.d(TAG, "uiDeviceFound() ... finished calling mRoboRoachManager.connect()");
                }
            });


        } else {

            Log.d(TAG, "uiDeviceFound() ... Found a non-RoboRoach :( !");

        }
    }


    static class ViewHolder {
        TextView goLeftText;
        TextView goRightText;
        TextView configText;
        ImageView roachImage;
        ImageView backpackImage;

        SeekBar Frequecy;
        SeekBar Duration;
        SeekBar PulseWidth;
        SeekBar Gain;
        Switch RandomMode;

    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (gestureDetector.onTouchEvent(event)) {
            return true;
        }
        return super.onTouchEvent(event);
    }


    private void onLeftSwipe() {
        // Do something
        Log.d(TAG, "onLeftSwipe()");
        if (mRoboRoachManager.isConnected() && !mOnSettingsScreen) {
            if (!mTurning) mRoboRoachManager.turnLeft();
        }
    }

    private void onRightSwipe() {
        // Do something
        Log.d(TAG, "onRightSwipe()");
        if (mRoboRoachManager.isConnected() && !mOnSettingsScreen) {
            if (!mTurning) mRoboRoachManager.turnRight();
        }

    }

    @Override
    public void uiLeftTurnSentSuccessfully(final int stimulusDuration) {

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                viewHolder.goLeftText.setVisibility(View.VISIBLE);
                mTurning = true;
                addTurnCommandTimeout(stimulusDuration);
            }
        });
    }

    @Override
    public void uiRightTurnSentSuccessfully(final int stimulusDuration) {

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                viewHolder.goRightText.setVisibility(View.VISIBLE);
                mTurning = true;
                addTurnCommandTimeout(stimulusDuration);
            }
        });
    }


    /* make sure that potential scanning will take no longer
 * than <SCANNING_TIMEOUT> seconds from now on */
    private void addTurnCommandTimeout(int timeoutInMS) {
        Runnable timeout = new Runnable() {
            @Override
            public void run() {
                viewHolder.goRightText.setVisibility(View.INVISIBLE);
                viewHolder.goLeftText.setVisibility(View.INVISIBLE);
                mTurning = false;
            }
        };
        mHandler.postDelayed(timeout, timeoutInMS);
    }

    /* make sure that potential scanning will take no longer
* than <SCANNING_TIMEOUT> seconds from now on */
    private void addScanningTimeout() {

        mHandler.postDelayed(scanTimeout, SCANNING_TIMEOUT);
    }


    /* check if user agreed to enable BT */
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        // user didn't want to turn on BT
        if (requestCode == ENABLE_BT_REQUEST_ID) {
            if (resultCode == Activity.RESULT_CANCELED) {
                btDisabled();
                return;
            }
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

    private void btDisabled() {
        Toast.makeText(this, "Sorry, Your bluetooth needs to be turned ON for your RoboRoach to work!", Toast.LENGTH_LONG).show();
        finish();
    }

    private void bleMissing() {
        Toast.makeText(this, "BLE Hardware is required for your RoboRoach. Please try on another device.", Toast.LENGTH_LONG).show();
        finish();
    }

    // Private class for gestures
    private class SwipeGestureDetector
            extends SimpleOnGestureListener {
        // Swipe properties, you can change it to make the swipe
        // longer or shorter and speed
        private static final int SWIPE_MIN_DISTANCE = 60;
        private static final int SWIPE_MAX_OFF_PATH = 200;
        private static final int SWIPE_THRESHOLD_VELOCITY = 200;

        @Override
        public boolean onFling(MotionEvent e1, MotionEvent e2,
                               float velocityX, float velocityY) {
            try {
                float diffAbs = Math.abs(e1.getY() - e2.getY());
                float diff = e1.getX() - e2.getX();

                if (diffAbs > SWIPE_MAX_OFF_PATH)
                    return false;

                // Left swipe
                if (diff > SWIPE_MIN_DISTANCE
                        && Math.abs(velocityX) > SWIPE_THRESHOLD_VELOCITY) {
                    RoboRoachActivity.this.onLeftSwipe();

                    // Right swipe
                } else if (-diff > SWIPE_MIN_DISTANCE
                        && Math.abs(velocityX) > SWIPE_THRESHOLD_VELOCITY) {
                    RoboRoachActivity.this.onRightSwipe();
                }
            } catch (Exception e) {
                Log.e("RoboRoachActivity", "Error on gestures");
            }
            return false;
        }
    }

}


