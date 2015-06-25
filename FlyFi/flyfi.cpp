#include "flyfi.h"

FlyFi::FlyFi(QWidget *parent) : QMainWindow(parent) {
  this->setFixedSize(this->width(), this->height());

  ui.setupUi(this);
}

FlyFi::~FlyFi() {

}

void FlyFi::showEvent(QShowEvent* event) {
  QWidget::showEvent(event);

  listSerialPorts();
  addBaudRates();
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

void FlyFi::on_btnOpen_clicked() {
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

void FlyFi::listSerialPorts() {
  availSerPorts_ = serial::list_ports();

  char deviceName[128];
  for(auto pDevice = availSerPorts_.begin(); pDevice != availSerPorts_.end(); pDevice++) {
    sprintf_s(deviceName, sizeof(deviceName), "%s - %s", pDevice->port.c_str(), pDevice->description.c_str());
    ui.cmbSerialPorts->addItem(deviceName);
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
