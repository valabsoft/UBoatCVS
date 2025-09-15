#include "uboatmodel.h"

UBoatModel::UBoatModel()
{
    _cameraStatus = ConnectionStatus::OFF;
    _packetStatus = ConnectionStatus::OFF;
}

void UBoatModel::setCameraStatus(ConnectionStatus status)
{
    _cameraStatus = status;
}
void UBoatModel::setPacketStatus(ConnectionStatus status)
{
    _packetStatus = status;
}
ConnectionStatus UBoatModel::getCameraStatus()
{
    return _cameraStatus;
}
ConnectionStatus UBoatModel::getPacketStatus()
{
    return _packetStatus;
}
