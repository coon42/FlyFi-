#include "flyfi.h"
#include <Windows.h>

using std::vector;

FlyFi::FlyFi(QWidget *parent) : QMainWindow(parent) {
  pMidiIn = new RtMidiIn();
  qRegisterMetaType<MidiMsg_t>();

  bool ok = QObject::connect(this, SIGNAL(dispatchMidiMsg(MidiMsg_t, int)), this, 
      SLOT(on_dispatchMidiMsg(MidiMsg_t, int)));

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

  MidiMsg_t msg;
  msg.midiEvent.deltatime = deltatime;
  int dataSize = pMessage->size();

  switch (dataSize) {
    case 3: msg.midiEvent.byte2 = pMessage->at(2);
    case 2: msg.midiEvent.byte1 = pMessage->at(1);
    case 1: msg.midiEvent.byte0 = pMessage->at(0);
  }

  emit pFlyFi->dispatchMidiMsg(msg, dataSize - 1);
}

// MIDI events. TODO: move from gui class to somewhere else!?
void FlyFi::onNoteOff(NoteOff_t noteOff) {
  dbg("Note off");
  playTone(noteOff.channel, 0);
}

void FlyFi::onNoteOn(NoteOn_t noteOn) {
  dbg("Note on");
  auto midiNoteToFrequency = [](int note) -> float {
    return 8.17575 * pow(2.0, note / 12.0);
  };

  playTone(noteOn.channel, midiNoteToFrequency(noteOn.note));
}

void FlyFi::onNoteKeyPressure(NoteKeyPressure_t noteKeyPressure) {
  dbg("Note Key Pressure [not implemented yet!]");
  // TODO: implement if needed.
}

void FlyFi::onSetParameter(SetParameter_t setParameter) {
  dbg("Set Parameter [not implemented yet!]");
  // TODO: implement if needed.
}

void FlyFi::onSetProgram(SetProgram_t setProgram) {
  dbg("Set Program [not implemented yet!]");
  // TODO: implement if needed.
}

void FlyFi::onChangePressure(ChangePressure_t changePressure) {
  dbg("Change Pressure [not implemented yet!]");
  // TODO: implement if needed.
}

void FlyFi::onSetPitchWheel(SetPitchWheel_t setPitchWheel) {
  dbg("Set Pitch Wheel [not implemented yet!]");
  // TODO: Implement!
}

void FlyFi::onSysEx(SysEx_t sysEx, int dataSize) {
  dbg("System Exclusive [not implemented yet!]");
}

// TODO: implement SysEx and Meta events?

// end of move

#define MAX_CHANNELS 16
void FlyFi::playTone(int channel, float frequency) {
  if (channel < 1 || channel > MAX_CHANNELS) {
    dbg("channel '%d' out of range. it has to be between 1 - %d", channel, MAX_CHANNELS);
    return;
  }
  
  int prescaler = 8;
  int crystalClock = 20000000; // 20 Mhz

  if (frequency > 440)
    return;

  // if (frequency > 0)
  //   dbg("Frequency: %f Hz", frequency);

  auto round = [](int num) -> int  {
    return 0.5 + num;
  };

  int ticks = frequency > 0 ? round(crystalClock / (2.0f * prescaler * frequency)) : 0;
  uint8_t data[5] { 0x55, 0xAA, channel, ticks >> 8, ticks && 0xFF };

  if (ser_.isOpen()) {
    ser_.write(reinterpret_cast<uint8_t*>(&data), sizeof(data));
  }
  else
    dbg("serial port error on play tone for midi_channel: %d", channel);
}

void FlyFi::on_dispatchMidiMsg(MidiMsg_t msg, int dataSize) {
  int eventType =  msg.midiEvent.byte0 & 0xF0;
  int channel   = (msg.midiEvent.byte0 & 0x0F) + 1;
  msg.midiEvent.byte0 = channel;

  switch (eventType) {
    case msgNoteOff:          onNoteOff(msg.noteOff);                 break;
    case msgNoteOn:           onNoteOn(msg.noteOn);                   break;
    case msgNoteKeyPressure:  onNoteKeyPressure(msg.noteKeyPressure); break;
    case msgSetParameter:     onSetParameter(msg.setParameter);       break;
    case msgSetProgram:       onSetProgram(msg.setProgram);           break;
    case msgChangePressure:   onChangePressure(msg.changePressure);   break;
    case msgSetPitchWheel:    onSetPitchWheel(msg.setPitchWheel);     break;
    case msgMetaEvent:                                                break; // TODO: Implement !?

    // Fallthrough intended
    case msgSysEx1:
    case msgSysEx2:
      onSysEx(msg.sysEx, dataSize);
      break;

    default:    
      dbgErr("Unknown MIDI Pattern: 0x%02X, 0x%02X, 0x%02X", msg.midiEvent.byte0, msg.midiEvent.byte1, 
          msg.midiEvent.byte2);

      break;
  }
}

void FlyFi::setFloatNum(float float_num) {
  char freq[256];
  sprintf(freq, "%.2f Hz", float_num / 100.0f);
  ui.lblFreq->setText(freq);
}

void FlyFi::on_btnRefreshPorts_clicked() {
  ui.cmbSerialPorts->clear();
  ui.cmbMidiPorts->clear();
  availSerPorts_.clear();
  availMidiPorts_.clear();

  listSerialPorts();
  listMidiInPorts();
}

void FlyFi::on_cmbMidiPorts_currentIndexChanged(int index) {
  if (pMidiIn->isPortOpen())
    pMidiIn->closePort();

  if (index != -1) {    
    pMidiIn->openPort(index);
    pMidiIn->setCallback(onMidiEvent, this);
    pMidiIn->ignoreTypes(false, false, false); // Don't ignore sysex, timing, or active sensing messages.

    if (pMidiIn->isPortOpen())
      dbg("Midi Port '%s' opened.", pMidiIn->getPortName(index).c_str());
    else
      dbg("Error on opening Port '%s'!", pMidiIn->getPortName(index).c_str());
  }
}

void FlyFi::on_btnOpenSerial_clicked() {
  int baudRate = ui.cmbBaudrate->currentText().toInt();
  string portName = availSerPorts_[ui.cmbSerialPorts->currentIndex()].port;

  try {
    ser_.close();
    ser_.setPort(portName);
    ser_.setBaudrate(baudRate);
    ser_.open();

    if (ser_.isOpen())
      dbg("Connected to serial port '%s'.", ser_.getPort().c_str());
  }
  catch (serial::IOException e) {
    dbgErr("Failed opening serial port '%s'. Is it already opened by another application?", 
        ser_.getPort().c_str());
  }
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
