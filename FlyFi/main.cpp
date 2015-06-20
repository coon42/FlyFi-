#include "flyfi.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  FlyFi w;
  w.show();

  return a.exec();
}
