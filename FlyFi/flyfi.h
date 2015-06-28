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

/*
** MIDI Messages [ consist of message, and optional bytes ]
**				a 'msg' has two nibbles: message type & channel
*/
typedef enum {
  msgNoteOff                                            = 0x80,	  /* [ pitch, volume ] */
  msgNoteOn                                             = 0x90,	  /* [ pitch, volume ] */
  msgNoteKeyPressure                                    = 0xa0,		/* [ pitch, pressure (after touch) ] */
  msgSetParameter                                       = 0xb0,		/* [ param number (CC), setting ] */
  msgSetProgram                                         = 0xc0,		/* [ program ] */
  msgChangePressure                                     = 0xd0,		/* [ pressure (after touch) ] */
  msgSetPitchWheel                                      = 0xe0,		/* [ LSB,  MSG (two 7 bit values) ] */

  msgMetaEvent                                          = 0xff,
  msgSysEx1                                             = 0xf0,
  msgSysEx2                                             = 0xf7,

  /* Alternative names */
  msgPatchChange = msgSetProgram,
  msgControlChange = msgSetParameter,

  msgSysMask = 0xf0,
} tMIDI_MSG;

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
  void dbgErr(string format, ...);
  
public slots:
  void on_btnRefreshPorts_clicked();
  void on_btnOpenMidi_clicked();
  void on_btnOpenSerial_clicked();
  void on_sldFreq_valueChanged(int val);
  void on_btnPlay_clicked();
  void on_btnStop_clicked();
  void on_action_close_triggered();
  void on_dispatchMidiMsg(double deltatime, int msgSize, unsigned char byte0, unsigned char byte1, 
      unsigned char byte2);
  
signals:
  void dispatchMidiMsg(double deltatime, int msgSize, unsigned char byte0, unsigned char byte1, unsigned char byte2);
};



#endif // FLYFI_H
