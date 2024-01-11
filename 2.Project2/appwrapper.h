#ifndef APPWRAPPER_H
#define APPWRAPPER_H
#include <QObject>
#include <QQmlApplicationEngine>
// #include "datasource.h"
#include "jokemodel.h"

class AppWrapper : public QObject
{
  Q_OBJECT
public:
  explicit AppWrapper(QObject *parent = nullptr);
  bool initialize();

private:
  QQmlApplicationEngine mEngine;
  JokeModel mJokeModel;
};

#endif// APPWRAPPER_H
