package com.backyardbrains.roboroach;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.Locale;
import java.util.UUID;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Handler;
import android.util.Log;
import java.nio.ByteBuffer;



public class RoboRoachManager {

    public static final UUID DEVICE_INFORMATION = new UUID((0x180AL << 32) | 0x1000, GattUtils.leastSigBits);
    public static final UUID BATTERY_SERVICE_1_1 = new UUID((0x180FL << 32) | 0x1000, GattUtils.leastSigBits);
    public static final UUID BATTERY_LEVEL = new UUID((0x2A19L << 32) | 0x1000, GattUtils.leastSigBits);
    public static final UUID FIRMWARE_REVISION_STRING = new UUID((0x2A26L << 32) | 0x1000, GattUtils.leastSigBits);
    public static final UUID HARDWARE_REVISION_STRING = new UUID((0x2A27L << 32) | 0x1000, GattUtils.leastSigBits);
    public static final UUID BYB_ROBOROACH_SERVICE = new UUID((0xB2B0L << 32) | 0x1000, GattUtils.leastSigBits);
    public static final UUID ROBOROACH_FREQUENCY = new UUID((0xB2B1L << 32) | 0x1000, GattUtils.leastSigBits);
    public static final UUID ROBOROACH_PULSE_WIDTH = new UUID((0xB2B2L << 32) | 0x1000, GattUtils.leastSigBits);
    public static final UUID ROBOROACH_DURATION_IN_5MS = new UUID((0xB2B3L << 32) | 0x1000, GattUtils.leastSigBits);
    public static final UUID ROBOROACH_RANDOM_MODE = new UUID((0xB2B4L << 32) | 0x1000, GattUtils.leastSigBits);
    public static final UUID ROBOROACH_STIMULATE_LEFT = new UUID((0xB2B5L << 32) | 0x1000, GattUtils.leastSigBits);
    public static final UUID ROBOROACH_STIMULATE_RIGHT = new UUID((0xB2B6L << 32) | 0x1000, GattUtils.leastSigBits);
    public static final UUID ROBOROACH_GAIN = new UUID((0xB2B7L << 32) | 0x1000, GattUtils.leastSigBits);
    public static final UUID ROBOROACH_FREQ_MIN = new UUID((0xB2B8L << 32) | 0x1000, GattUtils.leastSigBits);
    public static final UUID ROBOROACH_FREQ_MAX = new UUID((0xB2B9L << 32) | 0x1000, GattUtils.leastSigBits);
    public static final UUID ROBOROACH_PW_MIN = new UUID((0xB2BAL << 32) | 0x1000, GattUtils.leastSigBits);
    public static final UUID ROBOROACH_PW_MAX = new UUID((0xB2BBL << 32) | 0x1000, GattUtils.leastSigBits);
    public static final UUID ROBOROACH_GAIN_MIN = new UUID((0xB2BCL << 32) | 0x1000, GattUtils.leastSigBits);
    public static final UUID ROBOROACH_GAIN_MAX = new UUID((0xB2BDL << 32) | 0x1000, GattUtils.leastSigBits);

    private final static String TAG = RoboRoachManager.class.getSimpleName();

    /* defines (in milliseconds) how often RSSI should be updated */
    private static final int RSSI_UPDATE_TIME_INTERVAL = 1500; // 1.5 seconds

    /* callback object through which we are returning results to the caller */
    private RoboRoachManagerCallbacks mUiCallback = null;
    /* define NULL object for UI callbacks */
    private static final RoboRoachManagerCallbacks NULL_CALLBACK = new RoboRoachManagerCallbacks.Null();

    /* creates BleWrapper object, set its parent activity and callback object */
    public RoboRoachManager(Activity parent, RoboRoachManagerCallbacks callback) {
        this.mParent = parent;
        mUiCallback = callback;
        if(mUiCallback == null) mUiCallback = NULL_CALLBACK;
    }

    public BluetoothManager           getManager() { return mBluetoothManager; }
    public BluetoothAdapter           getAdapter() { return mBluetoothAdapter; }
    public BluetoothDevice            getDevice()  { return mBluetoothDevice; }
    public BluetoothGatt              getGatt()    { return mBluetoothGatt; }
    public BluetoothGattService       getCachedService() { return mBluetoothSelectedService; }
    public List<BluetoothGattService> getCachedServices() { return mBluetoothGattServices; }
    public boolean                    isConnected() { return mConnected; }
    public int                        getRoboRoachFrequency() { return rrFrequency; }
    public int                        getRoboRoachGain() { return rrGain; }
    public int                        getRoboRoachPulseWidth() { return rrPulseWidth; }
    public int                        getRoboRoachDuration() { return rrDuration; }
    public boolean                    getRoboRoachRandomMode()  { return rrRandomMode; }

    private BluetoothGattService    mInfoService;
    private BluetoothGattService    mRoboRoachService;
    private BluetoothGattService    mBatteryService;

    private static int rrFrequency;
    private static int rrPulseWidth;
    private static int rrDuration;
    private static int rrGain;
    private static boolean rrRandomMode = false;
    private static int rrBatteryLevel = 0;

    public int getRoboRoachBatteryLevel() { return rrBatteryLevel; }
    public String getRoboRoachConfigurationString() {
        if (rrRandomMode) {
            return "Randomized Stimulus. " + rrGain +"%";
        } else {
            return rrFrequency + "Hz, " + rrPulseWidth + "ms Pulse, for " + rrDuration + "ms. " + rrGain +"%";
        }
    }

    /* run test and check if this device has BT and BLE hardware available */
    public boolean checkBleHardwareAvailable() {
        // First check general Bluetooth Hardware:
        // get BluetoothManager...
        final BluetoothManager manager = (BluetoothManager) mParent.getSystemService(Context.BLUETOOTH_SERVICE);
        if(manager == null) return false;
        // .. and then get adapter from manager
        final BluetoothAdapter adapter = manager.getAdapter();
        if(adapter == null) return false;

        // and then check if BT LE is also available
        boolean hasBle = mParent.getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE);
        return hasBle;
    }

    public void requestRoboRoachParameters() {
        if (mRoboRoachService == null) return;

        requestCharacteristicValue(mRoboRoachService.getCharacteristic(ROBOROACH_FREQUENCY));

    }

    /* before any action check if BT is turned ON and enabled for us
     * call this in onResume to be always sure that BT is ON when Your
     * application is put into the foreground */
    public boolean isBtEnabled() {
        final BluetoothManager manager = (BluetoothManager) mParent.getSystemService(Context.BLUETOOTH_SERVICE);
        if(manager == null) return false;

        final BluetoothAdapter adapter = manager.getAdapter();
        if(adapter == null) return false;

        return adapter.isEnabled();
    }

    /* set new value for turn right */
    public void turnRight() {
        if (mBluetoothAdapter == null || mBluetoothGatt == null || mRoboRoachService == null) return;

        final byte[] dataToWrite = new byte[] { (byte)0x01 };
        final BluetoothGattCharacteristic ch = mRoboRoachService.getCharacteristic(ROBOROACH_STIMULATE_RIGHT);

        // first set it locally....
        ch.setValue(dataToWrite);
        // ... and then "commit" changes to the peripheral
        mBluetoothGatt.writeCharacteristic(ch);
    }

    /* set new value for turn left */
    public void turnLeft() {
        if (mBluetoothAdapter == null || mBluetoothGatt == null || mRoboRoachService == null) return;

        final byte[] dataToWrite = new byte[] { (byte)0x01 };
        final BluetoothGattCharacteristic ch = mRoboRoachService.getCharacteristic(ROBOROACH_STIMULATE_LEFT);

        // first set it locally....
        ch.setValue(dataToWrite);
        // ... and then "commit" changes to the peripheral
        mBluetoothGatt.writeCharacteristic(ch);
    }


    public void updateGain( int gain ){

        final byte[] dataToWrite = new byte[] { (byte)gain };
        final BluetoothGattCharacteristic ch = mRoboRoachService.getCharacteristic(ROBOROACH_GAIN);
        // first set it locally....
        ch.setValue(dataToWrite);
        // ... and then "commit" changes to the peripheral
        mBluetoothGatt.writeCharacteristic(ch);
        rrGain = gain;

    }

    public void updateFrequency( int freq ){

        final byte[] dataToWrite = new byte[] { (byte)freq };
        final BluetoothGattCharacteristic ch = mRoboRoachService.getCharacteristic(ROBOROACH_FREQUENCY);
        ch.setValue(dataToWrite);
        mBluetoothGatt.writeCharacteristic(ch);
        rrFrequency = freq;
    }


    public void updateRandomMode( boolean randomMode ){

        final byte[] dataToWrite;
        if ( randomMode ) {
            dataToWrite = new byte[] { (byte)0x01 };
        } else {
            dataToWrite = new byte[] { (byte)0x00 };
        }
        final BluetoothGattCharacteristic ch = mRoboRoachService.getCharacteristic(ROBOROACH_RANDOM_MODE);
        ch.setValue(dataToWrite);
        mBluetoothGatt.writeCharacteristic(ch);
        rrRandomMode = randomMode;
    }

    public void updateDuration( int dur ){

        final byte[] dataToWrite = new byte[] { (byte) (dur/5) };
        final BluetoothGattCharacteristic ch = mRoboRoachService.getCharacteristic(ROBOROACH_DURATION_IN_5MS);
        // first set it locally....
        ch.setValue(dataToWrite);
        // ... and then "commit" changes to the peripheral
        mBluetoothGatt.writeCharacteristic(ch);

        rrDuration = dur;
    }

    public void updatePulseWidth( int pw ){

        final byte[] dataToWrite = new byte[] { (byte)pw };
        final BluetoothGattCharacteristic ch = mRoboRoachService.getCharacteristic(ROBOROACH_PULSE_WIDTH);
        // first set it locally....
        ch.setValue(dataToWrite);
        // ... and then "commit" changes to the peripheral
        mBluetoothGatt.writeCharacteristic(ch);

        rrPulseWidth = pw;
    }

    /* start scanning for BT LE devices around */
    public void startScanning() {
        Log.e(TAG, "startScanning()");
        mBluetoothAdapter.startLeScan(mDeviceFoundCallback);
    }

    /* stops current scanning */
    public void stopScanning() {
        Log.e(TAG, "stopScanning()");
        mBluetoothAdapter.stopLeScan(mDeviceFoundCallback);
    }

    /* initialize BLE and get BT Manager & Adapter */
    public boolean initialize() {
        Log.d(TAG, "initialize()");

        if (mBluetoothManager == null) {
            mBluetoothManager = (BluetoothManager) mParent.getSystemService(Context.BLUETOOTH_SERVICE);
            if (mBluetoothManager == null) {
                return false;
            }
        }

        if(mBluetoothAdapter == null) mBluetoothAdapter = mBluetoothManager.getAdapter();
        if (mBluetoothAdapter == null) {
            return false;
        }
        return true;
    }

    /* connect to the device with specified address */
    public boolean connect(final String deviceAddress) {
        if (mBluetoothAdapter == null || deviceAddress == null) return false;
        mDeviceAddress = deviceAddress;

        Log.d(TAG, "connect()");

        // check if we need to connect from scratch or just reconnect to previous device
        if(mBluetoothGatt != null && mBluetoothGatt.getDevice().getAddress().equals(deviceAddress)) {
            // just reconnect
            return mBluetoothGatt.connect();
        }
        else {
            // connect from scratch
            // get BluetoothDevice object for specified address
            mBluetoothDevice = mBluetoothAdapter.getRemoteDevice(mDeviceAddress);
            if (mBluetoothDevice == null) {
                // we got wrong address - that device is not available!
                return false;
            }
            // connect with remote device
            mBluetoothGatt = mBluetoothDevice.connectGatt(mParent, false, mBleCallback);
        }
        if (mBluetoothGatt == null) Log.e( TAG, "mBluetoothGatt is null!" );
        return true;
    }

    /* disconnect the device. It is still possible to reconnect to it later with this Gatt client */
    public void disconnect() {
        Log.d(TAG, "disconnect()");

        if(mBluetoothGatt != null) mBluetoothGatt.disconnect();
        mUiCallback.uiDeviceDisconnected(mBluetoothGatt, mBluetoothDevice);
    }

    /* close GATT client completely */
    public void close() {
        Log.d(TAG, "close()" );
        if(mBluetoothGatt != null) mBluetoothGatt.close();
        mBluetoothGatt = null;
    }

    /* request new RSSi value for the connection*/
    public void readPeriodicalyRssiValue(final boolean repeat) {
        mTimerEnabled = repeat;
        // check if we should stop checking RSSI value
        if(mConnected == false || mBluetoothGatt == null || mTimerEnabled == false) {
            mTimerEnabled = false;
            return;
        }

        mTimerHandler.postDelayed(new Runnable() {
            @Override
            public void run() {
                if(mBluetoothGatt == null ||
                        mBluetoothAdapter == null ||
                        mConnected == false)
                {
                    mTimerEnabled = false;
                    return;
                }

                // request RSSI value
                mBluetoothGatt.readRemoteRssi();
                // add call it once more in the future
                readPeriodicalyRssiValue(mTimerEnabled);
            }
        }, RSSI_UPDATE_TIME_INTERVAL);
    }

    /* starts monitoring RSSI value */
    public void startMonitoringRssiValue() {
        readPeriodicalyRssiValue(true);
    }

    /* stops monitoring of RSSI value */
    public void stopMonitoringRssiValue() {
        readPeriodicalyRssiValue(false);
    }

    /* request to discover all services available on the remote devices
     * results are delivered through callback object */
    public void startServicesDiscovery() {
        Log.d(TAG, "startServicesDiscovery()" );
        if(mBluetoothGatt != null) mBluetoothGatt.discoverServices();
    }

    /* gets services and calls UI callback to handle them
     * before calling getServices() make sure service discovery is finished! */
    public void getSupportedServices() {
        Log.d(TAG, "getSupportedServices()" );

        if(mBluetoothGattServices != null && mBluetoothGattServices.size() > 0) mBluetoothGattServices.clear();
        // keep reference to all services in local array:
        //if(mBluetoothGatt != null) mBluetoothGattServices = mBluetoothGatt.getServices();

        mRoboRoachService = mBluetoothGatt.getService(BYB_ROBOROACH_SERVICE);
        mBatteryService = mBluetoothGatt.getService(BATTERY_SERVICE_1_1);
        mInfoService = mBluetoothGatt.getService(DEVICE_INFORMATION);

        if(mRoboRoachService != null) mRoboRoachService.getCharacteristics();
        if(mBatteryService != null)  mBatteryService.getCharacteristics();
        if(mInfoService != null) mInfoService.getCharacteristics();

        mUiCallback.uiServicesFound();
    }

    /* get all characteristic for particular service and pass them to the UI callback */
    public void getCharacteristicsForService(final BluetoothGattService service) {
        if(service == null) return;
        List<BluetoothGattCharacteristic> chars = null;

        chars = service.getCharacteristics();
        //mUiCallback.uiCharacteristicForService(mBluetoothGatt, mBluetoothDevice, service, chars);
        // keep reference to the last selected service
        mBluetoothSelectedService = service;


    }



    /* request to fetch newest value stored on the remote device for particular characteristic */
    public void requestCharacteristicValue(BluetoothGattCharacteristic ch) {
        if (mBluetoothAdapter == null || mBluetoothGatt == null) return;

        mBluetoothGatt.readCharacteristic(ch);

        Log.d(TAG, "requestCharacteristicValue()" );
        // new value available will be notified in Callback Object
    }

    /* get characteristic's value (and parse it for some types of characteristics)
     * before calling this You should always update the value by calling requestCharacteristicValue() */
    public void getCharacteristicValue(BluetoothGattCharacteristic ch) {
        if (mBluetoothAdapter == null || mBluetoothGatt == null || ch == null) return;

        if (ch.getUuid().equals(ROBOROACH_FREQUENCY)) rrFrequency = ch.getIntValue(BluetoothGattCharacteristic.FORMAT_UINT8, 0);
        if (ch.getUuid().equals(ROBOROACH_PULSE_WIDTH)) rrPulseWidth = ch.getIntValue(BluetoothGattCharacteristic.FORMAT_UINT8, 0);
        if (ch.getUuid().equals(ROBOROACH_DURATION_IN_5MS)) rrDuration = ch.getIntValue(BluetoothGattCharacteristic.FORMAT_UINT8, 0) * 5;
        if (ch.getUuid().equals(ROBOROACH_GAIN)) rrGain = ch.getIntValue(BluetoothGattCharacteristic.FORMAT_UINT8, 0);
        if (ch.getUuid().equals(ROBOROACH_RANDOM_MODE)) rrRandomMode = ch.getIntValue(BluetoothGattCharacteristic.FORMAT_UINT8, 0) == 1;
        if (ch.getUuid().equals(BATTERY_LEVEL)) rrBatteryLevel = ch.getIntValue(BluetoothGattCharacteristic.FORMAT_UINT8, 0);

        Log.d(TAG, "F=[" + rrFrequency + "]PW=[" + rrPulseWidth + "]" );


         //mUiCallback.uiNewValueForCharacteristic(mBluetoothGatt,
        //        mBluetoothDevice,
        //        mBluetoothSelectedService,
        //        ch,
        //        strValue,
        //        intValue,
        //       rawValue,
        //        timestamp);
    }

    /* reads and return what what FORMAT is indicated by characteristic's properties
     * seems that value makes no sense in most cases */
    public int getValueFormat(BluetoothGattCharacteristic ch) {
        int properties = ch.getProperties();

        if((BluetoothGattCharacteristic.FORMAT_FLOAT & properties) != 0) return BluetoothGattCharacteristic.FORMAT_FLOAT;
        if((BluetoothGattCharacteristic.FORMAT_SFLOAT & properties) != 0) return BluetoothGattCharacteristic.FORMAT_SFLOAT;
        if((BluetoothGattCharacteristic.FORMAT_SINT16 & properties) != 0) return BluetoothGattCharacteristic.FORMAT_SINT16;
        if((BluetoothGattCharacteristic.FORMAT_SINT32 & properties) != 0) return BluetoothGattCharacteristic.FORMAT_SINT32;
        if((BluetoothGattCharacteristic.FORMAT_SINT8 & properties) != 0) return BluetoothGattCharacteristic.FORMAT_SINT8;
        if((BluetoothGattCharacteristic.FORMAT_UINT16 & properties) != 0) return BluetoothGattCharacteristic.FORMAT_UINT16;
        if((BluetoothGattCharacteristic.FORMAT_UINT32 & properties) != 0) return BluetoothGattCharacteristic.FORMAT_UINT32;
        if((BluetoothGattCharacteristic.FORMAT_UINT8 & properties) != 0) return BluetoothGattCharacteristic.FORMAT_UINT8;

        return 0;
    }

    /* set new value for particular characteristic */
    public void writeDataToCharacteristic(final BluetoothGattCharacteristic ch, final byte[] dataToWrite) {
        if (mBluetoothAdapter == null || mBluetoothGatt == null || ch == null) return;

        // first set it locally....
        ch.setValue(dataToWrite);
        // ... and then "commit" changes to the peripheral
        mBluetoothGatt.writeCharacteristic(ch);
    }

    /* enables/disables notification for characteristic */
    public void setNotificationForCharacteristic(BluetoothGattCharacteristic ch, boolean enabled) {
        if (mBluetoothAdapter == null || mBluetoothGatt == null) return;

        boolean success = mBluetoothGatt.setCharacteristicNotification(ch, enabled);
        if(!success) {
            Log.e("------", "Seting proper notification status for characteristic failed!");
        }

        Log.d(TAG,ch.toString());

        // This is also sometimes required (e.g. for heart rate monitors) to enable notifications/indications
        // see: https://developer.bluetooth.org/gatt/descriptors/Pages/DescriptorViewer.aspx?u=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
        BluetoothGattDescriptor descriptor = ch.getDescriptor(UUID.fromString("00002902-0000-1000-8000-00805f9b34fb"));
        if(descriptor != null) {
            byte[] val = enabled ? BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE : BluetoothGattDescriptor.DISABLE_NOTIFICATION_VALUE;
            descriptor.setValue(val);
            mBluetoothGatt.writeDescriptor(descriptor);
        }
    }

    /* defines callback for scanning results */
    private BluetoothAdapter.LeScanCallback mDeviceFoundCallback = new BluetoothAdapter.LeScanCallback() {
        @Override
        public void onLeScan(final BluetoothDevice device, final int rssi, final byte[] scanRecord) {
            Log.d(TAG, "mDeviceFoundCallback()" );
            mUiCallback.uiDeviceFound(device, rssi, scanRecord);
        }
    };

    /* callbacks called for any action on particular Ble Device */
    private final BluetoothGattCallback mBleCallback = new BluetoothGattCallback() {
        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            if (newState == BluetoothProfile.STATE_CONNECTED) {
                mConnected = true;
                mUiCallback.uiDeviceConnected(mBluetoothGatt, mBluetoothDevice);

                // now we can start talking with the device, e.g.
                mBluetoothGatt.readRemoteRssi();
                // response will be delivered to callback object!

                // in our case we would also like automatically to call for services discovery
                startServicesDiscovery();

                Log.d(TAG, "onConnectionStateChange()");
                Log.d(TAG, BYB_ROBOROACH_SERVICE.toString());
                // and we also want to get RSSI value to be updated periodically
                //startMonitoringRssiValue();
            }
            else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
                mConnected = false;
                mUiCallback.uiDeviceDisconnected(mBluetoothGatt, mBluetoothDevice);
            }
        }

        @Override
        public void onServicesDiscovered(BluetoothGatt gatt, int status) {
            if (status == BluetoothGatt.GATT_SUCCESS) {
                // now, when services discovery is finished, we can call getServices() for Gatt
                getSupportedServices();
            }
        }



        @Override
        public void onCharacteristicRead(BluetoothGatt gatt,
                                         BluetoothGattCharacteristic characteristic,
                                         int status)
        {
            Log.d(TAG, "onCharacteristicRead()");

            // we got response regarding our request to fetch characteristic value
            if (status == BluetoothGatt.GATT_SUCCESS) {
                // and it success, so we can get the value
                getCharacteristicValue(characteristic);

                //Hack.   Walk through the values.
                if (characteristic.getUuid().equals( ROBOROACH_FREQUENCY)) requestCharacteristicValue(mRoboRoachService.getCharacteristic(ROBOROACH_PULSE_WIDTH));
                if (characteristic.getUuid().equals( ROBOROACH_PULSE_WIDTH)) requestCharacteristicValue(mRoboRoachService.getCharacteristic(ROBOROACH_DURATION_IN_5MS));
                if (characteristic.getUuid().equals( ROBOROACH_DURATION_IN_5MS)) requestCharacteristicValue(mRoboRoachService.getCharacteristic( ROBOROACH_RANDOM_MODE));
                if (characteristic.getUuid().equals( ROBOROACH_RANDOM_MODE)) requestCharacteristicValue(mRoboRoachService.getCharacteristic(ROBOROACH_GAIN));
                if (characteristic.getUuid().equals( ROBOROACH_GAIN)) requestCharacteristicValue(mBatteryService.getCharacteristic(BATTERY_LEVEL));
                if (characteristic.getUuid().equals( BATTERY_LEVEL)){
                    mUiCallback.uiRoboRoachPropertiesUpdated();
                }

            }
        }

        @Override
        public void onCharacteristicChanged(BluetoothGatt gatt,
                                            BluetoothGattCharacteristic characteristic)
        {
            Log.d(TAG, "onCharacteristicChanged()");
            // characteristic's value was updated due to enabled notification, lets get this value
            // the value itself will be reported to the UI inside getCharacteristicValue
            getCharacteristicValue(characteristic);
            // also, notify UI that notification are enabled for particular characteristic
            //mUiCallback.uiGotNotification(mBluetoothGatt, mBluetoothDevice, mBluetoothSelectedService, characteristic);
        }

        @Override
        public void onCharacteristicWrite(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
            Log.d( TAG, "onCharacteristicWrite(" + status +")" );
            // we got response regarding our request to write new value to the characteristic
            // let see if it failed or not
            if(status == BluetoothGatt.GATT_SUCCESS) {
                if (characteristic.getUuid().equals(ROBOROACH_STIMULATE_LEFT) ) {
                    mUiCallback.uiLeftTurnSentSuccessfully( rrDuration );
                }
                if (characteristic.getUuid().equals(ROBOROACH_STIMULATE_RIGHT) ) {
                    mUiCallback.uiRightTurnSentSuccessfully( rrDuration );
                }
            }
        }

        @Override
        public void onReadRemoteRssi(BluetoothGatt gatt, int rssi, int status) {
            if(status == BluetoothGatt.GATT_SUCCESS) {
                // we got new value of RSSI of the connection, pass it to the UI
                //mUiCallback.uiNewRssiAvailable(mBluetoothGatt, mBluetoothDevice, rssi);
            }
        }
    };

    private Activity mParent = null;
    private boolean mConnected = false;
    private String mDeviceAddress = "";

    private BluetoothManager mBluetoothManager = null;
    private BluetoothAdapter mBluetoothAdapter = null;
    private BluetoothDevice  mBluetoothDevice = null;
    private BluetoothGatt    mBluetoothGatt = null;
    private BluetoothGattService mBluetoothSelectedService = null;
    private List<BluetoothGattService> mBluetoothGattServices = null;

    private Handler mTimerHandler = new Handler();
    private boolean mTimerEnabled = false;
}
