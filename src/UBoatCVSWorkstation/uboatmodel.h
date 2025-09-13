#ifndef UBOATMODEL_H
#define UBOATMODEL_H

class UBoatModel
{
public:
    UBoatModel();
    void setCameraIsConnected(bool isconnected);
    void setPacketIsConnected(bool isconnected);
    bool getCameraIsConnected();
    bool getPacketIsConnected();
private:
    bool _cameraIsConnected;
    bool _packetIsConnected;
};

#endif // UBOATMODEL_H
