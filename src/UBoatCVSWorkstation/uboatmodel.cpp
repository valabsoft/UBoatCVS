#include "uboatmodel.h"

UBoatModel::UBoatModel()
{
    _cameraIsConnected = false;
    _packetIsConnected = false;
}

void UBoatModel::setCameraIsConnected(bool isconnected)
{
    _cameraIsConnected = isconnected;
}
void UBoatModel::setPacketIsConnected(bool isconnected)
{
    _packetIsConnected = isconnected;
}
bool UBoatModel::getCameraIsConnected()
{
    return _cameraIsConnected;
}
bool UBoatModel::getPacketIsConnected()
{
    return _packetIsConnected;
}
