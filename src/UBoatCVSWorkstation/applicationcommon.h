#ifndef APPLICATIONCOMMON_H
#define APPLICATIONCOMMON_H

enum Theme {
    WHITE,
    BLACK
};

enum ConnectionStatus {
    ON,
    OFF,
    UNKNOWN
};

struct BoundingBox
{
    float x, y, w, h;
    float confidence;
    int track_id;
    bool selected;
};

#endif // APPLICATIONCOMMON_H
