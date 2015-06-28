#include "flyfi.h"
#include <QtWidgets/QApplication>

#pragma comment (lib, "winmm.lib")

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  FlyFi w;
  w.show();

  return a.exec();
}
