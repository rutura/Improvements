#include "appwrapper.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QQmlContext>

AppWrapper::AppWrapper(QObject *parent) : QObject(parent) {}
bool AppWrapper::initialize()
{
  DataSource *ds = new DataSource(this);
  mJokeModel.setDatasource(ds);
  mEngine.rootContext()->setContextProperty("myModel", &mJokeModel);
  mEngine.rootContext()->setContextProperty("myDatasource", ds);
  mEngine.load(QUrl(QStringLiteral("qrc:/main.qml")));
  if (mEngine.rootObjects().isEmpty()) {
    return false;
  } else {
    return true;
  }
}
