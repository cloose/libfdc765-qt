#ifndef FLOPPYDISKCONTROLLER_H
#define FLOPPYDISKCONTROLLER_H

#include <QVector>

class FloppyDiskController
{
public:
    FloppyDiskController();
    
    quint8 readDataRegister();
    void writeDataRegister(quint8 value);
    
    quint8 headLoadTime() const;
    quint8 headUnloadTime() const;
    quint8 stepRateTime() const;
    bool dmaModeSelected() const;
    
private:
    void transitToExecutionPhase();
    void executeCommand();
    
    void specify();
    
    QVector<quint8> m_dataRegisters;
    quint8 m_currentDataRegister;
    
    quint8 m_headLoadTime;
    quint8 m_headUnloadTime;
    quint8 m_stepRateTime;
    bool m_dmaModeSelected;
};

#endif // FLOPPYDISKCONTROLLER_H
