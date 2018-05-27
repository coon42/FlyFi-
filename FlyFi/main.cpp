#include "flyfi.h"
#include <QtWidgets/QApplication>

#pragma comment (lib, "winmm.lib")

extern "C" int eMidi_printf(const char* pFormat, ...) {
  va_list args;

  va_start(args, pFormat);
  int ret = vprintf(pFormat, args);
  va_end(args);

  return ret;
}

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  FlyFi w;
  w.show();

  return a.exec();
}
