#include "flyfi.h"
#include <Windows.h>

using std::vector;

FlyFi::FlyFi(QWidget *parent) : QMainWindow(parent) {
  pMidiIn = new RtMidiIn();

  QObject::connect(this, SIGNAL(doRecvMidiMsg(double, unsigned char, unsigned char, unsigned char)),
       this, SLOT(on_MidiMsgReceived(double, unsigned char, unsigned char, unsigned char)));

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

  emit pFlyFi->doRecvMidiMsg(deltatime, byte[0], byte[1], byte[2]);
}

void FlyFi::on_MidiMsgReceived(double deltatime, unsigned char byte0, unsigned char byte1, unsigned char byte2) {
  dbg("[%.3f]: 0x%02X, 0x%02X, 0x%02X", deltatime, byte0, byte1, byte2);
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
  if (ser_.isOpen()) {
    ser_.write("Play");
    dbg("Play");
  }
  else
    dbg("Ser Port is not open!");

  //recvThread_.start();
}

void FlyFi::on_btnStop_clicked() {
  if (ser_.isOpen()) {
    ser_.write("Stop");
    dbg("Stop");
  }
  else
    dbg("Ser Port is not open!");
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
