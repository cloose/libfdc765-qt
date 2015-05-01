#ifndef DISKIMAGEFILE_H
#define DISKIMAGEFILE_H

#include "dskformat.h"

#include <QByteArray>
#include <QVector>

class QIODevice;

class DiskImageFile
{
public:
    explicit DiskImageFile(QIODevice *device);
    ~DiskImageFile();

    bool open();

    qint64 size() const;
    bool isExtendedFormat() const;
    qint64 lookupTrack(quint8 cylinder, quint8 head) const;
    DiskInformationBlock *readDiskHeader() const;
    TrackInformationBlock *readTrackHeader(quint8 cylinder, quint8 head) const;
    SectorInfo *readSectorInfo(quint8 cylinder, quint8 head, quint8 sectorId) const;

private:
    int convertToTrack(quint8 cylinder, quint8 head) const;
    void precalculateTrackOffsets();

    QIODevice *m_device;
    DiskInformationBlock *m_diskInfo;
    QByteArray m_content;
    QVector<qint64> m_trackOffsets;
};

#endif // DISKIMAGEFILE_H
