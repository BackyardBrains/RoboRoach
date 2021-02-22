package com.backyardbrains.roboroach.utils;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothManager;
import android.content.Context;
import android.content.pm.PackageManager;

import androidx.annotation.NonNull;

/**
 * @author Tihomir Leka <ticapeca at gmail.com.
 */
public class BluetoothUtils {

    /**
     * Checks if this device has necessary BT and BLE hardware available.
     */
    public static boolean checkBleHardwareAvailable(@NonNull Context context) {
        // First check general Bluetooth Hardware:
        // get BluetoothManager...
        final BluetoothManager manager = (BluetoothManager) context.getSystemService(Context.BLUETOOTH_SERVICE);
        if (manager == null) return false;
        // .. and then get adapter from manager
        final BluetoothAdapter adapter = manager.getAdapter();
        if (adapter == null) return false;

        // and then check if BT LE is also available
        return context.getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE);
    }

    /**
     * Check if BT is turned ON and enabled for us. This should be called in {@link Activity#onResume()} to always sure
     * that BT is ON when Your application is put into the foreground.
     */
    public static boolean isBtEnabled(@NonNull Context context) {
        final BluetoothManager manager = (BluetoothManager) context.getSystemService(Context.BLUETOOTH_SERVICE);
        if (manager == null) return false;

        final BluetoothAdapter adapter = manager.getAdapter();
        return adapter != null && adapter.isEnabled();
    }
}
