#include "flyfi.h"
#include <Windows.h>

using std::vector;

FlyFi::FlyFi(QWidget *parent) : QMainWindow(parent) {
  pMidiIn = new RtMidiIn();

  QObject::connect(this, SIGNAL(dispatchMidiMsg(double, int, unsigned char, unsigned char, unsigned char)),
       this, SLOT(on_dispatchMidiMsg(double, int, unsigned char, unsigned char, unsigned char)));

  ui.setupUi(this);
  setFixedSize(width(), height());
}

FlyFi::~FlyFi() {
  if (pMidiIn) {
    pMidiIn->closePort();
    delete pMidiIn;
  }
}

void FlyFi::showEvent(QShowEvent* event) {
  QWidget::showEvent(event);
  ui.centralWidget->setFixedSize(ui.centralWidget->width(), ui.centralWidget->height());

  listSerialPorts();
  listMidiInPorts();
  addBaudRates();
}

void FlyFi::onMidiEvent(double deltatime, vector<unsigned char>* pMessage, void* pArg) {
  FlyFi* pFlyFi = reinterpret_cast<FlyFi*>(pArg);

  unsigned char byte[3] = { 0, 0, 0 };
 
  switch (pMessage->size()) {
    case 3: byte[2] = pMessage->at(2);
    case 2: byte[1] = pMessage->at(1);
    case 1: byte[0] = pMessage->at(0);
  }

  emit pFlyFi->dispatchMidiMsg(deltatime, pMessage->size(), byte[0], byte[1], byte[2]);
}

#define MAX_CHANNELS 16
void FlyFi::playTone(int channel, float frequency) {
  if (channel < 1 || channel > MAX_CHANNELS) {
    dbg("channel '%d' out of range. it has to be between 1 - %d", channel, MAX_CHANNELS);
    return;
  }

  dbg("frequency: %d Hz", frequency);
    
  int prescaler = 8;
  int crystalClock = 20000000; // 20 Mhz

  if (frequency > 440)
    return;

  auto round = [](int num) -> int  {
    return 0.5 + num;
  };

  int ticks = frequency > 0 ? round(crystalClock / (2.0f * prescaler * frequency)) : 0; 

  uint8_t data[5] { 0x55, 0xAA, channel, ticks >> 8, ticks && 0xFF };

  if (ser_.isOpen()) {
    ser_.write(reinterpret_cast<uint8_t*>(&data), sizeof(data));
    dbg("Playing tone: %f", frequency);
  }
  else
    dbg("serial port error on play tone for midi_channel: %d", channel);
}

void FlyFi::on_dispatchMidiMsg(double deltatime, int msgSize, unsigned char byte0, unsigned char byte1,   
    unsigned char byte2) {

  const char* noteStr;
  int eventType = byte0 & 0xF0;

  switch (eventType) {
  case msgNoteOff:
    struct {
      uint8_t channel;
      uint8_t note;
      uint8_t velocity;
    } noteOff;

    noteOff.channel = byte0 & 0x0F;
    noteOff.note = byte1;
    noteOff.velocity = byte2;
    playTone(noteOff.channel + 1, 0);

    noteStr = "Note off";
    break;

  case msgNoteOn: {
      struct {
        uint8_t channel;
        uint8_t note;
        uint8_t velocity;
      } noteOn;

      noteOn.channel = byte0 & 0x0F;
      noteOn.note = byte1;
      noteOn.velocity = byte2;

      auto midiNoteToFrequency = [](int note) -> float {
        return 8.17575 * pow(2.0, note / 12.0);
      };

      playTone(noteOn.channel + 1, midiNoteToFrequency(noteOn.note));
      noteStr = "Note on";
      }
    break;

  case msgNoteKeyPressure:
    noteStr = "Note Key Pressure";
    break;

  case msgSetParameter:
    noteStr = "Set Parameter";
    break;

  case msgSetProgram:
    noteStr = "SetProgram";
    break;

  case msgChangePressure:
    noteStr = "Change Pressure";
    break;

  case msgSetPitchWheel :
    noteStr = "SetPitchWheel";
    break;

  case msgMetaEvent:
    noteStr = "Meta Event";
    break;

  case msgSysEx1:
    noteStr = "SysEx1";
    break;

  case msgSysEx2:
    noteStr = "SysEx2";
    break;

  default:
    noteStr = "##### Unknown ######";
    dbgErr("Unknown MIDI Pattern: 0x%02X, 0x%02X, 0x%02X", byte0, byte1, byte2);
    return;
  }

  char pBuf[1024];
  sprintf(pBuf, "[%.3f] %s: ", deltatime, noteStr);
  unsigned char msg[3] = { byte0, byte1, byte2 };

  for (int i = 0; i < msgSize; i++)
    sprintf(pBuf, "%s 0x%02X ", pBuf, msg[i]);
  
  sprintf(pBuf, "%s (%d)", pBuf, msgSize);
  dbg(pBuf);
}

void FlyFi::setFloatNum(float float_num) {
  char freq[256];
  sprintf(freq, "%.2f Hz", float_num / 100.0f);
  ui.lblFreq->setText(freq);
}

void FlyFi::on_btnRefreshPorts_clicked() {
  ui.cmbSerialPorts->clear();
  availSerPorts_.clear();

  listSerialPorts();
}

void FlyFi::on_btnOpenMidi_clicked() {
  if (pMidiIn->isPortOpen())
    pMidiIn->closePort();

  int portNumber = ui.cmbMidiPorts->currentIndex();
  pMidiIn->openPort(portNumber);
  pMidiIn->setCallback(onMidiEvent, this);
  pMidiIn->ignoreTypes(false, false, false); // Don't ignore sysex, timing, or active sensing messages.

  if (pMidiIn->isPortOpen())
    dbg("Midi Port '%s' opened", pMidiIn->getPortName(portNumber).c_str());
  else
    dbg("Error on opening Port '%s'!", pMidiIn->getPortName(portNumber).c_str());
}

void FlyFi::on_btnOpenSerial_clicked() {
  int baudRate = ui.cmbBaudrate->currentText().toInt();
  string portName = availSerPorts_[ui.cmbSerialPorts->currentIndex()].port;

  ser_.close();
  ser_.setPort(portName);
  ser_.setBaudrate(baudRate);
  ser_.open();

  if (ser_.isOpen())
    dbg("connected to serial port %s", ser_.getPort().c_str());
}

void FlyFi::on_sldFreq_valueChanged(int val) {
  setFloatNum(val);
}

void FlyFi::on_btnPlay_clicked() {
  float frequency = ui.sldFreq->value() / 100;

  if (ser_.isOpen()) {
    playTone(1, frequency);
  }
}

void FlyFi::on_btnStop_clicked() {
  if (ser_.isOpen()) {
    playTone(1, 0);
    dbg("Stop");
  }
}

// ------------------------------------------------------------------------------------------------------------
// Menu bar
// ------------------------------------------------------------------------------------------------------------

void FlyFi::on_action_close_triggered() {
  this->close();
}

// ------------------------------------------------------------------------------------------------------------
// helpers
// ------------------------------------------------------------------------------------------------------------

void FlyFi::dbg(string format, ...) {
  char formattedText[512];

  va_list args;
  va_start(args, format);
  vsnprintf_s(formattedText, sizeof(formattedText), sizeof(formattedText), format.c_str(), args);
  va_end(args);

  ui.edtDebug->append(formattedText);
}

void FlyFi::dbgErr(string format, ...) {
  char formattedText[512];

  va_list args;
  va_start(args, format);
  vsnprintf_s(formattedText, sizeof(formattedText), sizeof(formattedText), format.c_str(), args);
  va_end(args);

  ui.edtError->append(formattedText);
}

void FlyFi::listMidiInPorts() {
  ui.cmbMidiPorts->clear();

  unsigned int nPorts = pMidiIn->getPortCount();
  if (nPorts == 0) {
    ui.cmbMidiPorts->setDisabled(true);
    ui.cmbMidiPorts->addItem("(No MIDI input devices available)");
  }
  else {
    ui.cmbMidiPorts->setDisabled(false);

    for (unsigned int i = 0; i < nPorts; i++)
      ui.cmbMidiPorts->addItem(pMidiIn->getPortName(i).c_str());
  }
}

void FlyFi::listSerialPorts() {
  availSerPorts_ = serial::list_ports();

  if (availSerPorts_.empty()) {
    ui.cmbSerialPorts->setDisabled(true);
    ui.cmbSerialPorts->addItem("(No Serial Ports available)");
  }
  else {
    ui.cmbSerialPorts->setDisabled(false);

    char deviceName[128];
    for(auto pDevice = availSerPorts_.begin(); pDevice != availSerPorts_.end(); pDevice++) {
      sprintf_s(deviceName, sizeof(deviceName), "%s - %s", pDevice->port.c_str(), pDevice->description.c_str());
      ui.cmbSerialPorts->addItem(deviceName);
    }
  }
}

void FlyFi::addBaudRates() {
  ui.cmbBaudrate->addItem("300");
  ui.cmbBaudrate->addItem("1200");
  ui.cmbBaudrate->addItem("2400");
  ui.cmbBaudrate->addItem("4800");
  ui.cmbBaudrate->addItem("9600");
  ui.cmbBaudrate->addItem("14400");
  ui.cmbBaudrate->addItem("19200");
  ui.cmbBaudrate->addItem("28800");
  ui.cmbBaudrate->addItem("38400");
  ui.cmbBaudrate->addItem("57600");
  ui.cmbBaudrate->addItem("115200");
  ui.cmbBaudrate->addItem("230400");

  ui.cmbBaudrate->setCurrentIndex(4);
}
