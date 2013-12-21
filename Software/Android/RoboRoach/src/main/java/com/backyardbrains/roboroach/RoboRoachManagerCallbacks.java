package com.backyardbrains.roboroach;

import java.util.List;

import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;

public interface RoboRoachManagerCallbacks {

    public void uiDeviceFound(final BluetoothDevice device, int rssi, byte[] record);

    public void uiDeviceConnected(final BluetoothGatt gatt,
                                  final BluetoothDevice device);

    public void uiDeviceDisconnected(final BluetoothGatt gatt,
                                     final BluetoothDevice device);

    public void uiServicesFound();

    public void uiRoboRoachPropertiesUpdated();

    public void uiLeftTurnSentSuccessfully(final int stimulusDuration) ;

    public void uiRightTurnSentSuccessfully(final int stimulusDuration) ;

    /* define Null Adapter class for that interface */
    public static class Null implements RoboRoachManagerCallbacks {

        @Override
        public void uiDeviceFound(BluetoothDevice device, int rssi, byte[] record) {}

        @Override
        public void uiDeviceConnected(BluetoothGatt gatt, BluetoothDevice device) {}

        @Override
        public void uiDeviceDisconnected(BluetoothGatt gatt, BluetoothDevice device) {}

        @Override
        public void uiServicesFound() {}

        @Override
        public void uiRoboRoachPropertiesUpdated() {}

        @Override
        public void uiLeftTurnSentSuccessfully(int stimulusDuration) {}

        @Override
        public void uiRightTurnSentSuccessfully(int stimulusDuration) {}

    }
}
