package com.backyardbrains.roboroach;

import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;

public interface RoboRoachManagerCallbacks {

    void uiDeviceFound(final BluetoothDevice device, int rssi, byte[] record);

    void uiDeviceConnected(final BluetoothGatt gatt, final BluetoothDevice device);

    void uiDeviceDisconnected(final BluetoothGatt gatt, final BluetoothDevice device);

    void uiServicesFound();

    void uiRoboRoachPropertiesUpdated();

    void uiLeftTurnSentSuccessfully(final int stimulusDuration);

    void uiRightTurnSentSuccessfully(final int stimulusDuration);

    /**
     * Define Null Adapter class for that interface
     */
    class Null implements RoboRoachManagerCallbacks {

        @Override public void uiDeviceFound(BluetoothDevice device, int rssi, byte[] record) {
        }

        @Override public void uiDeviceConnected(BluetoothGatt gatt, BluetoothDevice device) {
        }

        @Override public void uiDeviceDisconnected(BluetoothGatt gatt, BluetoothDevice device) {
        }

        @Override public void uiServicesFound() {
        }

        @Override public void uiRoboRoachPropertiesUpdated() {
        }

        @Override public void uiLeftTurnSentSuccessfully(int stimulusDuration) {
        }

        @Override public void uiRightTurnSentSuccessfully(int stimulusDuration) {
        }
    }
}
