#include <QtTest/QtTest>

#include "floppydiskcontroller.h"

class FloppyDiskControllerTest : public QObject
{
    Q_OBJECT

private slots:
    void handlesSpecifyCommand();
};

void FloppyDiskControllerTest::handlesSpecifyCommand()
{
    FloppyDiskController fdc;
    
    fdc.writeDataRegister(0x03);
    fdc.writeDataRegister(0xa1);
    fdc.writeDataRegister(0x03);
    
    QCOMPARE((int)fdc.stepRateTime(), 0x0a);
    QCOMPARE((int)fdc.headUnloadTime(), 0x01);
    QCOMPARE((int)fdc.headLoadTime(), 0x01);
    QCOMPARE(fdc.dmaModeSelected(), false);
}

QTEST_APPLESS_MAIN(FloppyDiskControllerTest)
#include "floppydiskcontrollertest.moc"
