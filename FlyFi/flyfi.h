#ifndef FLYFI_H
#define FLYFI_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include <QList>
#include <QThread>
#include <vector>
#include "serial/serial.h"
#include "rtmidi/RtMidi.h"
#include "ui_flyfi.h"

using namespace std;

class SerialRecvThread : public QThread {
private:
  void run(Ui::FlyFiClass* ui, serial::Serial* pSer) {
    while (pSer->isOpen()) {
      pSer->waitReadable();
      std::string data = pSer->read();      
      ui->edtDebug->append(QString::fromStdString(data));
    }
  }
};

class FlyFi : public QMainWindow {
    Q_OBJECT

public:
    FlyFi(QWidget *parent = 0);
    ~FlyFi();
    static void onMidiEvent(double deltatime, vector<unsigned char>* pMessage, void* pArg);
    void FlyFi::showEvent(QShowEvent* event);

private:
  Ui::FlyFiClass ui;
  serial::Serial ser_;
  SerialRecvThread recvThread_;
  RtMidiIn* pMidiIn;
  vector<serial::PortInfo> availMidiPorts_;
  vector<serial::PortInfo> availSerPorts_;

  // helpers
  void listSerialPorts();
  void listMidiInPorts();
  void addBaudRates();
  void setFloatNum(float float_num);
  void dbg(string format, ...);
  
public slots:
  void on_btnRefreshPorts_clicked();
  void on_btnOpenMidi_clicked();
  void on_btnOpenSerial_clicked();
  void on_sldFreq_valueChanged(int val);
  void on_btnPlay_clicked();
  void on_btnStop_clicked();
  void on_action_close_triggered();
  void on_MidiMsgReceived(double deltatime, unsigned char byte0, unsigned char byte1, unsigned char byte2);
  
signals:
  void doRecvMidiMsg(double deltatime, unsigned char byte0, unsigned char byte1, unsigned char byte2);
};



#endif // FLYFI_H
