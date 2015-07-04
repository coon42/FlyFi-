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

typedef struct {
  double deltatime;
  uint8_t byte0;
  uint8_t byte1;
  uint8_t byte2;
} MidiEvent_t;

typedef struct {
  double deltatime;
  uint8_t channel;
  uint8_t note;
  uint8_t velocity;
} NoteOff_t;

typedef struct {
  double deltatime;
  uint8_t channel;
  uint8_t note; 
  uint8_t velocity;
} NoteOn_t;

typedef struct {
  double deltatime;
  int8_t channel;
  int8_t note;
  int8_t pressure;
} NoteKeyPressure_t;

typedef struct {
  double deltatime;
  int8_t channel;
  int8_t control;
  int8_t parameter;
} SetParameter_t;

typedef struct {
  double deltatime;
  int8_t channel;
  int8_t program;
} SetProgram_t;

typedef struct {
  double deltatime;
  int8_t channel;
  int8_t pressure;
} ChangePressure_t;

typedef struct {
  double deltatime;
  int8_t channel;
  int16_t pitch;
} SetPitchWheel_t;

typedef struct {
  double deltatime;
  void* data;
} SysEx_t;

typedef union {
  MidiEvent_t midiEvent;
  NoteOff_t noteOff;
  NoteOn_t noteOn;
  NoteKeyPressure_t noteKeyPressure;
  SetParameter_t setParameter;
  SetProgram_t setProgram;
  ChangePressure_t changePressure;
  SetPitchWheel_t setPitchWheel;
  SysEx_t sysEx;
} MidiMsg_t;
Q_DECLARE_METATYPE(MidiMsg_t);

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

  // MIDI events. TODO: move from gui class to somewhere else!?
  void onNoteOff(NoteOff_t noteOff);
  void onNoteOn(NoteOn_t noteOn);
  void onNoteKeyPressure(NoteKeyPressure_t noteKeyPressure);
  void onSetParameter(SetParameter_t setParameter);
  void onSetProgram(SetProgram_t setProgram);
  void onChangePressure(ChangePressure_t changePressure);
  void onSetPitchWheel(SetPitchWheel_t setPitchWheel);
  void onSysEx(SysEx_t sysEx, int dataSize);
  // TODO: implement SysEx and Meta events?
  // end of lay out

  // helpers
  void listSerialPorts();
  void listMidiInPorts();
  void addBaudRates();
  void setFloatNum(float float_num);
  void dbg(string format, ...);
  void dbgErr(string format, ...);
  void playTone(int channel, float frequency);

public slots:
  void on_btnRefreshPorts_clicked();
  void on_btnOpenSerial_clicked();
  void on_sldFreq_valueChanged(int val);
  void on_cmbMidiPorts_currentIndexChanged(int index);
  void on_btnPlay_clicked();
  void on_btnStop_clicked();
  void on_action_close_triggered();
  
private slots:
  void on_dispatchMidiMsg(MidiMsg_t msg, int dataSize);

signals:
  void dispatchMidiMsg(MidiMsg_t msg, int dataSize);
};



#endif // FLYFI_H
