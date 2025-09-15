#ifndef UBOATMODEL_H
#define UBOATMODEL_H

#include "applicationcommon.h"

class UBoatModel
{
public:
    UBoatModel();
    void setCameraStatus(ConnectionStatus isconnected);
    void setPacketStatus(ConnectionStatus isconnected);
    ConnectionStatus getCameraStatus();
    ConnectionStatus getPacketStatus();
private:
    ConnectionStatus _cameraStatus;
    ConnectionStatus _packetStatus;
};

#endif // UBOATMODEL_H
