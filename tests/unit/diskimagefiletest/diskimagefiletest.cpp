#include <QtTest/QtTest>

#include <QBuffer>
#include "diskimagefile.h"

class DiskImageFileTest : public QObject
{
    Q_OBJECT

private slots:
    void returnsSizeOfEmptyImage();
    void returnsSizeOfImage();
    void identifiesExtendedImages();
    void canOpenUnderlyingIoDevice();
    void looksupTrackOffset();
    void looksupTrackOffsetOfDoubleSidedDisk();
    void returnsErrorForLookupWithTooHighCylinderNumber();
    void returnsErrorForLookupWithTooHighHeadNumber();
    void readsTrackInfo();
    void readsDiskInfo();
    void readsSectorInfo();

private:
    QByteArray createDiskInformationBlock();
    QByteArray createTrackInformationBlock();
};

void DiskImageFileTest::returnsSizeOfEmptyImage()
{
    QBuffer buffer;
    DiskImageFile file(&buffer);

    QCOMPARE(file.size(), 0LL);
}

void DiskImageFileTest::returnsSizeOfImage()
{
    QByteArray ba = createDiskInformationBlock();
    QBuffer buffer(&ba);
    DiskImageFile file(&buffer);

    QCOMPARE(file.size(), buffer.size());
}

void DiskImageFileTest::identifiesExtendedImages()
{
    QByteArray ba = createDiskInformationBlock();
    QBuffer buffer(&ba);
    DiskImageFile file(&buffer);

    file.open();

    QCOMPARE(file.isExtendedFormat(), true);
}

void DiskImageFileTest::canOpenUnderlyingIoDevice()
{
    QByteArray ba = createDiskInformationBlock();
    QBuffer buffer(&ba);
    DiskImageFile file(&buffer);

    bool success = file.open();

    QCOMPARE(success, true);
    QCOMPARE(buffer.isOpen(), true);
}

void DiskImageFileTest::looksupTrackOffset()
{
    QByteArray ba = createDiskInformationBlock();
    QBuffer buffer(&ba);
    DiskImageFile file(&buffer);
    file.open();

    qint64 trackOffset = file.lookupTrack(2, 0);

    QCOMPARE(trackOffset, 0x2700LL);
}

void DiskImageFileTest::looksupTrackOffsetOfDoubleSidedDisk()
{
    QByteArray ba = createDiskInformationBlock();
    ba[0x30] = 0x03;
    ba[0x31] = 0x02;
    QBuffer buffer(&ba);
    DiskImageFile file(&buffer);
    file.open();

    qint64 trackOffset = file.lookupTrack(1, 1);

    QCOMPARE(trackOffset, 0x3900LL);
}

void DiskImageFileTest::returnsErrorForLookupWithTooHighCylinderNumber()
{
    QByteArray ba = createDiskInformationBlock();
    QBuffer buffer(&ba);
    DiskImageFile file(&buffer);
    file.open();

    qint64 trackOffset = file.lookupTrack(7, 0);

    QCOMPARE(trackOffset, -1LL);
}

void DiskImageFileTest::returnsErrorForLookupWithTooHighHeadNumber()
{
    QByteArray ba = createDiskInformationBlock();
    QBuffer buffer(&ba);
    DiskImageFile file(&buffer);
    file.open();

    qint64 trackOffset = file.lookupTrack(0, 1);

    QCOMPARE(trackOffset, -1LL);
}

void DiskImageFileTest::readsTrackInfo()
{
    QByteArray ba;
    ba.append(createDiskInformationBlock());
    ba.append(createTrackInformationBlock());

    QBuffer buffer(&ba);
    DiskImageFile file(&buffer);
    file.open();

    TrackInformationBlock *trackInfo = file.readTrackHeader(0, 0);

    QVERIFY(trackInfo != 0);
}

void DiskImageFileTest::readsDiskInfo()
{
    QByteArray ba;
    ba.append(createDiskInformationBlock());

    QBuffer buffer(&ba);
    DiskImageFile file(&buffer);
    file.open();

    DiskInformationBlock *diskInfo = file.readDiskHeader();

    QVERIFY(diskInfo != 0);
    QCOMPARE(QByteArray(diskInfo->tag, 34), QByteArray("EXTENDED CPC DSK File\r\nDisk-Info\r\n"));
    QCOMPARE(QByteArray(diskInfo->nameOfCreator, 14), QByteArray("NAMEOFCREATOR!"));
    QCOMPARE(diskInfo->numberOfTracks, (quint8)6);
    QCOMPARE(diskInfo->numberOfSides, (quint8)1);
    QCOMPARE(diskInfo->ExtendedFormat.trackSizeTable[0], (quint8)0x13);
    QCOMPARE(diskInfo->ExtendedFormat.trackSizeTable[1], (quint8)0x13);
    QCOMPARE(diskInfo->ExtendedFormat.trackSizeTable[2], (quint8)0x13);
    QCOMPARE(diskInfo->ExtendedFormat.trackSizeTable[3], (quint8)0x12);
    QCOMPARE(diskInfo->ExtendedFormat.trackSizeTable[4], (quint8)0x11);
    QCOMPARE(diskInfo->ExtendedFormat.trackSizeTable[5], (quint8)0x10);
    QCOMPARE(diskInfo->ExtendedFormat.trackSizeTable[6], (quint8)0x00);
}

void DiskImageFileTest::readsSectorInfo()
{
    QByteArray ba;
    ba.append(createDiskInformationBlock());
    ba.append(createTrackInformationBlock());

    QBuffer buffer(&ba);
    DiskImageFile file(&buffer);
    file.open();

    SectorInfo *sectorInfo = file.readSectorInfo(0, 0, 0x5f);

    QVERIFY(sectorInfo != 0);
    QCOMPARE(sectorInfo->track, (quint8)0);
    QCOMPARE(sectorInfo->side, (quint8)0);
    QCOMPARE(sectorInfo->sectorId, (quint8)0x5f);
    QCOMPARE(sectorInfo->sectorSize, (quint8)1);
    QCOMPARE(sectorInfo->st1, (quint8)0);
    QCOMPARE(sectorInfo->st2, (quint8)0);
    QCOMPARE(sectorInfo->dataLength, (quint16)256);
}

QByteArray DiskImageFileTest::createDiskInformationBlock()
{
    QByteArray ba;
    ba.append("EXTENDED CPC DSK File\r\nDisk-Info\r\n");
    ba.append("NAMEOFCREATOR!");
    ba.append("\x06\x01\x00\x00\x13\x13\x13\x12\x11\x10", 10);
    ba.append(QByteArray(198, '\x00'));
    return ba;
}

QByteArray DiskImageFileTest::createTrackInformationBlock()
{
    QByteArray ba;
    ba.append("Track-Info\r\n\x00\x00\x00\x00", 16);
    ba.append("\x00\x00\x00\x00\x04\x04\x4e\xe5", 8);
    ba.append("\x00\x00\x29\x04\x20\x20\x00\x08", 8);
    ba.append("\x00\x00\x44\x01\x00\x00\x00\x01", 8);
    ba.append("\x00\x00\x5f\x01\x00\x00\x00\x01", 8);
    ba.append("\x00\x00\x7a\x01\x00\x00\x00\x01", 8);
    ba.append(QByteArray(200, '\x00'));
    return ba;
}

QTEST_APPLESS_MAIN(DiskImageFileTest)
#include "diskimagefiletest.moc"
