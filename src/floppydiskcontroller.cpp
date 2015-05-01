#include "floppydiskcontroller.h"

#include <QDebug>

enum FdcCommands
{
    ReadDiagnosticCommand       = 0x02,
    SpecifyCommand              = 0x03,
    SenseDriveStatusCommand     = 0x04,
    WriteDataCommand            = 0x05,
    ReadDataCommand             = 0x06,
    RecalibrateCommand          = 0x07,
    SenseInterruptStatusCommand = 0x08,
    WriteDeletedDataCommand     = 0x09,
    ReadIdCommand               = 0x0a,
    ReadDeletedDataCommand      = 0x0c,
    WriteIdCommand              = 0x0d,
    SeekCommand                 = 0x0f,
    ScanEqualCommand            = 0x11,
    ScanLowOrEqualCommand       = 0x19,
    ScanHighOrEqualCommand      = 0x1e
};

static const int BytesInCommand[4] =
{
    -1,
    -1,
    9, // READ DIAGNOSTIC
    3  // SPECIFY
};

FloppyDiskController::FloppyDiskController() :
    m_currentDataRegister(0),
    m_headLoadTime(0x01),
    m_headUnloadTime(0x01),
    m_stepRateTime(0x0f)   
{
    m_dataRegisters.resize(9);
}

quint8 FloppyDiskController::readDataRegister()
{
    return 0;
}

void FloppyDiskController::writeDataRegister(quint8 value)
{
    m_dataRegisters[m_currentDataRegister] = value;
    m_currentDataRegister++;
    qDebug() << m_dataRegisters;
    if (m_currentDataRegister == BytesInCommand[m_dataRegisters[0]]) {
        qDebug() << "[FDC] command complete";
        transitToExecutionPhase();
    }
}

quint8 FloppyDiskController::headLoadTime() const
{
    return m_headLoadTime;
}

quint8 FloppyDiskController::headUnloadTime() const
{
    return m_headUnloadTime;
}

quint8 FloppyDiskController::stepRateTime() const
{
    return m_stepRateTime;
}

bool FloppyDiskController::dmaModeSelected() const
{
    return m_dmaModeSelected;
}

void FloppyDiskController::transitToExecutionPhase()
{
    executeCommand();
}

void FloppyDiskController::executeCommand()
{
    switch (m_dataRegisters[0])
    {
        case SpecifyCommand:
            specify();
            break;
    }
}

void FloppyDiskController::specify()
{
    m_stepRateTime = (m_dataRegisters[1] & 0xf0) >> 4;
    m_headUnloadTime = m_dataRegisters[1] & 0x0f;
    m_headLoadTime = (m_dataRegisters[2] & 0xfe) >> 1;
    m_dmaModeSelected = (m_dataRegisters[2] & 0x01) == 0;
    qDebug() << "[FDC] SPECIFY" 
             << hex 
             << m_stepRateTime << m_headUnloadTime 
             << m_headLoadTime << m_dmaModeSelected;
}
