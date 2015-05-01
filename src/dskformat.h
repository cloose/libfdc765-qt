#ifndef DSKFORMAT_H
#define DSKFORMAT_H

#include <QtGlobal>

struct DiskInformationBlock
{
    char tag[34];
    char nameOfCreator[14];
    quint8 numberOfTracks;
    quint8 numberOfSides;

    struct
    {
        quint8 unusedExt[2];
        quint8 trackSizeTable[204];
    } ExtendedFormat;
};

struct SectorInfo
{
    quint8 track;
    quint8 side;
    quint8 sectorId;
    quint8 sectorSize;
    quint8 st1;
    quint8 st2;
    quint16 dataLength;
};

struct TrackInformationBlock
{
    char   tag[13];
    quint8 unused[3];
    quint8 trackNumber;
    quint8 sideNumber;
    quint8 unused2[2];
    quint8 sectorSize;
    quint8 numberOfSectors;
    quint8 gap3Length;
    quint8 filler;
    SectorInfo sectorInfoList[29];    
};

#endif // DSKFORMAT_H

