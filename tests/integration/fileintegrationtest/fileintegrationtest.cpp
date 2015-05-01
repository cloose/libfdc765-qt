#include <QtTest/QtTest>

#include <QDataStream>
#include <QFile>

#include "diskimagefile.h"

class FileIntegrationTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void returnsSizeOfEmptyImage();
    void returnsSizeOfImage();
    void identifiesExtendedImages();
    void canOpenUnderlyingIoDevice();
    void looksupTrackOffset();
};

void FileIntegrationTest::initTestCase()
{
    QFile emptyfile("emptyfile.dsk");
    emptyfile.open(QIODevice::WriteOnly);
    emptyfile.close();

    QFile file("testfile.dsk");
    file.open(QIODevice::WriteOnly);

    file.write("EXTENDED CPC DSK File\r\nDisk-Info\r\n");
    file.write("NAMEOFCREATOR!");
    file.write("\x28\x01\x00\x00\x13\x13\x13\x12\x11\x10", 10);

    file.close();
}

void FileIntegrationTest::cleanupTestCase()
{
    QFile::remove("emptyfile.dsk");
    QFile::remove("testfile.dsk");
}

void FileIntegrationTest::returnsSizeOfEmptyImage()
{
    QFile file("emptyfile.dsk");
    DiskImageFile imageFile(&file);

    QCOMPARE(imageFile.size(), 0LL);
}

void FileIntegrationTest::returnsSizeOfImage()
{
    QFile file("testfile.dsk");
    DiskImageFile imageFile(&file);

    QCOMPARE(imageFile.size(), QFile("testfile.dsk").size());
}

void FileIntegrationTest::identifiesExtendedImages()
{
    QFile file("testfile.dsk");
    DiskImageFile imageFile(&file);

    imageFile.open();

    QCOMPARE(imageFile.isExtendedFormat(), true);
}

void FileIntegrationTest::canOpenUnderlyingIoDevice()
{
    QFile file("testfile.dsk");
    DiskImageFile imageFile(&file);

    bool success = imageFile.open();

    QCOMPARE(success, true);
    QCOMPARE(file.isOpen(), true);
}

void FileIntegrationTest::looksupTrackOffset()
{
    QFile file("testfile.dsk");
    DiskImageFile imageFile(&file);
    imageFile.open();

    qint64 trackOffset = imageFile.lookupTrack(2, 0);

    QCOMPARE(trackOffset, 0x2700LL);
}

QTEST_APPLESS_MAIN(FileIntegrationTest)
#include "fileintegrationtest.moc"

