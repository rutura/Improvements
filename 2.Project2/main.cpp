#include "appwrapper.h"
#include <QGuiApplication>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
  QGuiApplication app(argc, argv);
  AppWrapper wrapper;

  if (!wrapper.initialize()) return -1;

  return app.exec();
}
