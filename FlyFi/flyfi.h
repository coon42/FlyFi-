#ifndef FLYFI_H
#define FLYFI_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include <QList>
#include <QThread>
#include <vector>
#include "serial/serial.h"
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
    void FlyFi::showEvent(QShowEvent* event);

private:
  Ui::FlyFiClass ui;
  serial::Serial ser_;
  SerialRecvThread recvThread_;
  vector<serial::PortInfo> availSerPorts_ = serial::list_ports();

  // helpers
  void listSerialPorts();
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
};



#endif // FLYFI_H
