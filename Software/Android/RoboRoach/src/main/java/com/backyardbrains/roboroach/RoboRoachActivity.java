package com.backyardbrains.roboroach;

import android.Manifest;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.GestureDetector;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ViewFlipper;
import com.backyardbrains.roboroach.utils.BluetoothUtils;
import java.util.List;
import pub.devrel.easypermissions.AfterPermissionGranted;
import pub.devrel.easypermissions.AppSettingsDialog;
import pub.devrel.easypermissions.EasyPermissions;

import static com.backyardbrains.roboroach.utils.LogUtils.LOGD;
import static com.backyardbrains.roboroach.utils.LogUtils.LOGE;
import static com.backyardbrains.roboroach.utils.LogUtils.makeLogTag;

public class RoboRoachActivity extends AppCompatActivity
    implements RoboRoachManagerCallbacks, EasyPermissions.PermissionCallbacks {

    final static String TAG = makeLogTag(RoboRoachActivity.class);

    private static final int REQUEST_CODE_ENABLE_BT = 120;
    private static final int REQUEST_CODE_SETTINGS_SCREEN = 121;
    private static final int REQUEST_CODE_ACCESS_COARSE_LOCATION_PERM = 122;

    private static final long SCANNING_TIMEOUT = 4 * 1000; /* 4 seconds */

    private static final String RR_DEVICE_NAME = "RoboRoach";

    boolean mScanning = false;
    boolean mTurning = false;
    boolean mOnSettingsScreen = false;

    String mDeviceAddress;

    Handler mHandler = new Handler();
    RoboRoachManager mRoboRoachManager = null;
    ViewHolder viewHolder;
    Runnable mGATTUpdate;
    int mGATTFreq = 0;

    private GestureDetector gestureDetector;

    @Override protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        LOGD(TAG, "onCreate()");

        setContentView(R.layout.roboroach_main);

        mRoboRoachManager = new RoboRoachManager(this, this);

        // check if we have BT and BLE on board
        if (!BluetoothUtils.checkBleHardwareAvailable(this)) bleMissing();

        viewHolder = new ViewHolder();
        viewHolder.bind(this);

        // set toolbar as actionbar
        setSupportActionBar(viewHolder.toolbar);
        if (getSupportActionBar() != null) getSupportActionBar().setDisplayShowTitleEnabled(false);

        viewHolder.roachImage.setVisibility(View.VISIBLE);
        viewHolder.backpackImage.setVisibility(View.INVISIBLE);
        viewHolder.goLeftText.setVisibility(View.INVISIBLE);
        viewHolder.goRightText.setVisibility(View.INVISIBLE);

        viewHolder.Frequecy.setMax(150);
        viewHolder.Frequecy.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
            }

            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            public void onStopTrackingTouch(SeekBar seekBar) {
                mGATTFreq = seekBar.getProgress();
                if (mGATTFreq < 1) mGATTFreq = 1;

                //If the new freq it's greater than 1/2 the
                if ((float) mRoboRoachManager.getRoboRoachPulseWidth() > (float) 500 / mGATTFreq) {
                    mGATTUpdate = new Runnable() {
                        @Override public void run() {
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
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
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
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
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
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
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

        viewHolder.configText.setText("");

        final Button button = findViewById(R.id.btnSaveSettings);
        button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                LOGD(TAG, "Updating RoboRoach Settings");
                mRoboRoachManager.updateFrequency(viewHolder.Frequecy.getProgress());

                // Perform action on click
                ViewFlipper vf = findViewById(R.id.viewFlipper);
                vf.showNext();
                mOnSettingsScreen = false;
            }
        });

        gestureDetector = new GestureDetector(this.getBaseContext(), new SwipeGestureDetector());
    }

    @Override protected void onResume() {
        super.onResume();
        LOGD(TAG, "onResume()");

        // on every resume check if BT is enabled (user could turn it off while app was in background etc.)
        if (!BluetoothUtils.isBtEnabled(this)) {
            // BT is not turned on - ask user to make it enabled
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_CODE_ENABLE_BT);
            // see onActivityResult to check what is the status of our request
        } else {
            // check if we have location permission
            checkLocation();
        }

        // if RoboRoachManager cannot be initialized we should leave
        if (!mRoboRoachManager.initialize()) finish();

        invalidateOptionsMenu();
    }

    @Override protected void onPause() {
        super.onPause();
        LOGD(TAG, "onPause()");

        if (mRoboRoachManager.isConnected()) {
            runOnUiThread(new Runnable() {
                @Override public void run() {
                    mRoboRoachManager.stopMonitoringRssiValue();
                    mRoboRoachManager.disconnect();
                    mRoboRoachManager.close();
                    invalidateOptionsMenu();
                }
            });
        } else if (mScanning) {
            stopLeScan();
        }
    }

    @Override protected void onDestroy() {
        if (mRoboRoachManager != null) mRoboRoachManager.close();
        super.onDestroy();
    }

    @Override protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        // check if user agreed to enable BT.
        if (requestCode == REQUEST_CODE_ENABLE_BT) {
            if (resultCode == Activity.RESULT_CANCELED) {
                // user didn't want to turn on BT
                btDisabled();
                return;
            } else if (resultCode == Activity.RESULT_OK) {
                // check if we have location permission
                checkLocation();
            }
        }

        super.onActivityResult(requestCode, resultCode, data);
    }

    @Override public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.main_menu, menu);
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
                menu.findItem(R.id.menu_refresh).setActionView(R.layout.actionbar_progress);
            } else {
                menu.findItem(R.id.menu_stop).setVisible(false);
                menu.findItem(R.id.menu_scan).setVisible(true);
                menu.findItem(R.id.menu_refresh).setActionView(null);
            }
        }

        return true;
    }

    @Override public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        switch (item.getItemId()) {
            case R.id.menu_scan:
                // start LE scan
                startLeScan();
                break;
            case R.id.menu_stop:
                // stop LE scan
                stopLeScan();
                break;
            case R.id.menu_disconnect:
                disconnect();
                break;
            case R.id.menu_settings:
                if (!mOnSettingsScreen) {
                    ViewFlipper vf = findViewById(R.id.viewFlipper);
                    vf.showNext();
                    mOnSettingsScreen = true;
                }
                break;
        }
        return true;
    }

    @Override public boolean onTouchEvent(MotionEvent event) {
        return gestureDetector.onTouchEvent(event) || super.onTouchEvent(event);
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

    @Override public void uiDeviceConnected(final BluetoothGatt gatt, final BluetoothDevice device) {
        LOGD(TAG, "uiDeviceConnected()");
        runOnUiThread(new Runnable() {
            @Override public void run() {
                //mDeviceStatus.setText("connected");
                viewHolder.backpackImage.setImageAlpha(150);
                viewHolder.backpackImage.setVisibility(View.VISIBLE);
                invalidateOptionsMenu();
            }
        });
    }

    @Override public void uiDeviceDisconnected(final BluetoothGatt gatt, final BluetoothDevice device) {
        LOGD(TAG, "uiDeviceDisconnected()");

        // let's reset strongest signal and last connected device address
        mStrongestSignal = Integer.MIN_VALUE;
        mDeviceAddress = null;

        runOnUiThread(new Runnable() {
            @Override public void run() {
                viewHolder.configText.setText("");
                viewHolder.backpackImage.setVisibility(View.INVISIBLE);
            }
        });

        invalidateOptionsMenu();
    }

    @Override public void uiServicesFound() {
        mRoboRoachManager.requestRoboRoachParameters();
    }

    @Override public void uiRoboRoachPropertiesUpdated() {
        runOnUiThread(new Runnable() {
            @Override public void run() {
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

    private int mStrongestSignal = Integer.MIN_VALUE;

    @Override public void uiDeviceFound(BluetoothDevice device, int rssi, byte[] record) {
        //LOGD(TAG, "uiDeviceFound()");

        if (device == null || device.getAddress() == null || !RR_DEVICE_NAME.equals(device.getName())) return;

        if (rssi > mStrongestSignal) {
            LOGD(TAG, "uiDeviceFound() ... Found nearest RoboRoach: " + rssi);
            mStrongestSignal = rssi;
            mDeviceAddress = device.getAddress();
        }
    }

    void connectToNearestBtDevice() {
        if (mDeviceAddress != null) {
            LOGD(TAG, "connectToNearestBtDevice() ... Found a RoboRoach!");

            // adding to the UI have to happen in UI thread
            runOnUiThread(new Runnable() {
                @Override public void run() {
                    viewHolder.backpackImage.setImageAlpha(60);  //slowly builds up until connection
                    viewHolder.backpackImage.setVisibility(View.VISIBLE);

                    LOGD(TAG, "connectToNearestBtDevice() ... mDeviceAddress = " + mDeviceAddress);

                    if (mScanning) {
                        mScanning = false;
                        invalidateOptionsMenu();
                        mRoboRoachManager.stopScanning();
                        LOGD(TAG, "connectToNearestBtDevice() ... mRoboRoachManager.stopScanning()");
                    }

                    LOGD(TAG, "connectToNearestBtDevice() ... about to call mRoboRoachManager.connect()");
                    mRoboRoachManager.connect(mDeviceAddress);
                    LOGD(TAG, "connectToNearestBtDevice() ... finished calling mRoboRoachManager.connect()");
                }
            });
        } else {
            LOGD(TAG, "connectToNearestBtDevice() ... Couldn't find a RoboRoach! Continue to scan.");
            mHandler.postDelayed(new Runnable() {
                @Override public void run() {
                    connectToNearestBtDevice();
                }
            }, SCANNING_TIMEOUT);
        }
    }

    //@Override public void uiDeviceFound(BluetoothDevice device, int rssi, byte[] record) {
    //    LOGD(TAG, "uiDeviceFound()");
    //
    //    if (mHandler != null) mHandler.removeCallbacks(scanTimeout);
    //
    //    if (device == null || device.getAddress() == null || device.getName() == null) return;
    //
    //    mDeviceAddress = device.getAddress();
    //
    //    if (device.getName().equals("RoboRoach")) {
    //        LOGD(TAG, "uiDeviceFound() ...Found a RoboRoach!");
    //
    //        // adding to the UI have to happen in UI thread
    //        runOnUiThread(new Runnable() {
    //            @Override public void run() {
    //                viewHolder.backpackImage.setImageAlpha(60);  //slowly builds up until connection
    //                viewHolder.backpackImage.setVisibility(View.VISIBLE);
    //
    //                LOGD(TAG, "uiDeviceFound() ... mDeviceAddress = " + mDeviceAddress);
    //
    //                if (mScanning) {
    //                    mScanning = false;
    //                    invalidateOptionsMenu();
    //                    mRoboRoachManager.stopScanning();
    //                    LOGD(TAG, "uiDeviceFound() ... mRoboRoachManager.stopScanning()");
    //                }
    //
    //                LOGD(TAG, "uiDeviceFound() ... about to call mRoboRoachManager.connect()");
    //                mRoboRoachManager.connect(mDeviceAddress);
    //                LOGD(TAG, "uiDeviceFound() ... finished calling mRoboRoachManager.connect()");
    //            }
    //        });
    //    } else {
    //        LOGD(TAG, "uiDeviceFound() ... Found a non-RoboRoach :( !");
    //    }
    //}

    @Override public void uiLeftTurnSentSuccessfully(final int stimulusDuration) {
        runOnUiThread(new Runnable() {
            @Override public void run() {
                viewHolder.goLeftText.setVisibility(View.VISIBLE);
                mTurning = true;
                addTurnCommandTimeout(stimulusDuration);
            }
        });
    }

    @Override public void uiRightTurnSentSuccessfully(final int stimulusDuration) {
        runOnUiThread(new Runnable() {
            @Override public void run() {
                viewHolder.goRightText.setVisibility(View.VISIBLE);
                mTurning = true;
                addTurnCommandTimeout(stimulusDuration);
            }
        });
    }

    void onLeftSwipe() {
        LOGD(TAG, "onLeftSwipe()");
        if (mRoboRoachManager.isConnected() && !mOnSettingsScreen) {
            if (!mTurning) mRoboRoachManager.turnLeft();
        }
    }

    void onRightSwipe() {
        LOGD(TAG, "onRightSwipe()");
        if (mRoboRoachManager.isConnected() && !mOnSettingsScreen) {
            if (!mTurning) mRoboRoachManager.turnRight();
        }
    }

    private void startLeScan() {
        mHandler.postDelayed(new Runnable() {
            @Override public void run() {
                connectToNearestBtDevice();
            }
        }, SCANNING_TIMEOUT);
        mScanning = true;
        mRoboRoachManager.startScanning();
        invalidateOptionsMenu();
    }

    private void stopLeScan() {
        mScanning = false;
        mRoboRoachManager.stopScanning();
        invalidateOptionsMenu();
    }

    private void disconnect() {
        mRoboRoachManager.disconnect();
        //mRoboRoachManager.close();
        if (mOnSettingsScreen) {
            ViewFlipper vf = findViewById(R.id.viewFlipper);
            vf.showNext();
            mOnSettingsScreen = false;
        }
    }

    /* make sure that potential scanning will take no longer
 * than <SCANNING_TIMEOUT> seconds from now on */
    void addTurnCommandTimeout(int timeoutInMS) {
        Runnable timeout = new Runnable() {
            @Override public void run() {
                viewHolder.goRightText.setVisibility(View.INVISIBLE);
                viewHolder.goLeftText.setVisibility(View.INVISIBLE);
                mTurning = false;
            }
        };
        mHandler.postDelayed(timeout, timeoutInMS);
    }

    private void btDisabled() {
        Toast.makeText(this, getString(R.string.message_turn_on_bt), Toast.LENGTH_LONG).show();
        finish();
    }

    private void bleMissing() {
        Toast.makeText(this, getString(R.string.message_ble_hardware_required), Toast.LENGTH_LONG).show();
        finish();
    }

    //==============================================
    // ACCESS_COARSE_LOCATION PERMISSION
    //==============================================

    @Override public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions,
        @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        EasyPermissions.onRequestPermissionsResult(requestCode, permissions, grantResults, this);
    }

    @Override public void onPermissionsGranted(int requestCode, @NonNull List<String> perms) {
        LOGD(TAG, "onPermissionsGranted:" + requestCode + ":" + perms.size());
    }

    @Override public void onPermissionsDenied(int requestCode, @NonNull List<String> perms) {
        LOGD(TAG, "onPermissionsDenied:" + requestCode + ":" + perms.size());
        if (EasyPermissions.somePermissionPermanentlyDenied(this, perms)) {
            new AppSettingsDialog.Builder(this).setRationale(R.string.rationale_ask_again)
                .setTitle(R.string.title_settings_dialog)
                .setPositiveButton(R.string.action_setting)
                .setNegativeButton(R.string.action_cancel)
                .setRequestCode(REQUEST_CODE_SETTINGS_SCREEN)
                .build()
                .show();
        }
    }

    @AfterPermissionGranted(REQUEST_CODE_ACCESS_COARSE_LOCATION_PERM) void checkLocation() {
        if (!EasyPermissions.hasPermissions(this, Manifest.permission.ACCESS_COARSE_LOCATION)) {
            // Request the permission
            EasyPermissions.requestPermissions(this, getString(R.string.rationale_access_coarse_location),
                REQUEST_CODE_ACCESS_COARSE_LOCATION_PERM, Manifest.permission.ACCESS_COARSE_LOCATION);
        }
    }

    /**
     *
     */
    static class ViewHolder {
        Toolbar toolbar;

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

        // Binds UI elements to local variables
        void bind(@NonNull Activity activity) {
            toolbar = activity.findViewById(R.id.toolbar);
            roachImage = activity.findViewById(R.id.imageRoach);
            backpackImage = activity.findViewById(R.id.imageBackpack);
            goLeftText = activity.findViewById(R.id.textGoLeft);
            goRightText = activity.findViewById(R.id.textGoRight);
            configText = activity.findViewById(R.id.textConfig);
            Duration = activity.findViewById(R.id.sbDuration);
            Gain = activity.findViewById(R.id.sbGain);
            Frequecy = activity.findViewById(R.id.sbFrequency);
            PulseWidth = activity.findViewById(R.id.sbPulseWidth);
            RandomMode = activity.findViewById(R.id.swRandomMode);
        }
    }

    // Private class for gestures
    private class SwipeGestureDetector extends SimpleOnGestureListener {
        // Swipe properties, you can change it to make the swipe
        // longer or shorter and speed
        private static final int SWIPE_MIN_DISTANCE = 60;
        private static final int SWIPE_MAX_OFF_PATH = 200;
        private static final int SWIPE_THRESHOLD_VELOCITY = 200;

        @Override public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
            try {
                float diffAbs = Math.abs(e1.getY() - e2.getY());
                float diff = e1.getX() - e2.getX();

                if (diffAbs > SWIPE_MAX_OFF_PATH) return false;

                // Left swipe
                if (diff > SWIPE_MIN_DISTANCE && Math.abs(velocityX) > SWIPE_THRESHOLD_VELOCITY) {
                    onLeftSwipe();

                    // Right swipe
                } else if (-diff > SWIPE_MIN_DISTANCE && Math.abs(velocityX) > SWIPE_THRESHOLD_VELOCITY) {
                    onRightSwipe();
                }
            } catch (Exception e) {
                LOGE(TAG, "Error on gestures");
            }
            return false;
        }
    }
}


