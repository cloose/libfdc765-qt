#include "diskimagefile.h"

#include <QBuffer>
#include <QIODevice>

DiskImageFile::DiskImageFile(QIODevice *device) :
    m_device(device),
    m_diskInfo(0)
{
}

DiskImageFile::~DiskImageFile()
{
}

bool DiskImageFile::open()
{
    if (!m_device->open(QIODevice::ReadOnly)) {
        return false;
    }
    
    m_content = m_device->readAll();
    m_diskInfo = (DiskInformationBlock*)m_content.constData();
    precalculateTrackOffsets();

    return true;
}

qint64 DiskImageFile::size() const
{
    return m_device->size();
}

bool DiskImageFile::isExtendedFormat() const
{
    return m_content.startsWith("EXTENDED");
}

qint64 DiskImageFile::lookupTrack(quint8 cylinder, quint8 head) const
{
    if (cylinder >  m_diskInfo->numberOfTracks) return -1;
    if (head >= m_diskInfo->numberOfSides) return -1;

    int track = convertToTrack(cylinder, head);
    return m_trackOffsets[track];
}

DiskInformationBlock *DiskImageFile::readDiskHeader() const
{
    return m_diskInfo;
}

#include <QDebug>
TrackInformationBlock *DiskImageFile::readTrackHeader(quint8 cylinder, quint8 head) const
{
    qint64 offset = lookupTrack(cylinder, head);
    if (offset < 0) {
        qWarning("seek fail");
        return 0;
    }

    TrackInformationBlock *trackInfo = (TrackInformationBlock*)(m_content.constData() + offset);
    return trackInfo;
}

SectorInfo *DiskImageFile::readSectorInfo(quint8 cylinder, quint8 head, quint8 sectorId) const
{
    TrackInformationBlock *trackInfo = readTrackHeader(cylinder, head);
    if (!trackInfo) {
        qWarning("seek fail");
        return 0;
    }

    for (int i = 0; i < trackInfo->numberOfSectors; ++i) {
        if (trackInfo->sectorInfoList[i].sectorId == sectorId) {
            return &trackInfo->sectorInfoList[i];
        }
    }
}

int DiskImageFile::convertToTrack(quint8 cylinder, quint8 head) const
{
    int track = cylinder;
    if (m_diskInfo->numberOfSides > 1)
        track *= 2;
    track += head;

    return track;
}

void DiskImageFile::precalculateTrackOffsets()
{
    const int totalNumberOfTracks = m_diskInfo->numberOfTracks*m_diskInfo->numberOfSides;
    m_trackOffsets.resize(totalNumberOfTracks);

    m_trackOffsets[0] = sizeof(DiskInformationBlock);
    for (int i = 1; i < totalNumberOfTracks; ++i) {
        quint8 highByte = m_diskInfo->ExtendedFormat.trackSizeTable[i];
        m_trackOffsets[i] = (256 * highByte) + m_trackOffsets[i-1];
    }
}
