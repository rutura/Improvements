#include "datasource.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

DataSource::DataSource(QObject *parent)
  : QObject(parent), mNetManager(new QNetworkAccessManager(this)), mNetReply(nullptr), mDataBuffer(new QByteArray)
{
  connect(this, &DataSource::requestCompleted, this, &DataSource::makeRequest);
}

void DataSource::fetchJokes(int number)
{
  switch (mRequestState) {
  case RequestsState::REQUESTS_ONGOING:
    mRequestsToMake += number;
    break;
  case RequestsState::REQUESTS_COMPLETED:
    mRequestsToMake = number - 1;// -1 accounts for the request we just made
    makeRequest();
    break;
  }
}

void DataSource::addJoke(Joke *joke)
{
  emit preItemAdded();
  mJokes.append(joke);
  emit postItemAdded();
}

void DataSource::addJoke()
{
  Joke *joke = new Joke("Test Joke Added First", this);
  addJoke(joke);
}

void DataSource::addJoke(const QString &jokeParam)
{
  Joke *joke = new Joke(jokeParam, this);
  addJoke(joke);
}

void DataSource::removeJoke(int index)
{
  emit preItemRemoved(index);
  mJokes.removeAt(index);
  emit postItemRemoved();
}

void DataSource::removeLastJoke()
{
  if (!mJokes.isEmpty()) { removeJoke(mJokes.size() - 1); }
}

QList<Joke *> DataSource::dataItems() { return mJokes; }

void DataSource::dataReadyRead() { mDataBuffer->append(mNetReply->readAll()); }

void DataSource::dataReadFinished()
{
  // Parse the JSON
  if (mNetReply->error()) {
    qDebug() << "There was some error : " << mNetReply->errorString();
  } else {
    //
    // Turn the data into a json document
    QJsonDocument doc = QJsonDocument::fromJson(*mDataBuffer);
    // Grab the value array
    QJsonObject data = doc.object();
    QString joke = data["value"].toString();
    addJoke(joke);

    // Clear the buffer
    mDataBuffer->clear();
    if (mRequestsToMake > 0) {
      --mRequestsToMake;
      mRequestState = RequestsState::REQUESTS_ONGOING;
      emit requestCompleted();
    } else {
      mRequestState = RequestsState::REQUESTS_COMPLETED;
    }
  }
}

void DataSource::makeRequest()
{
  const QUrl API_ENDPOINT("https://api.chucknorris.io/jokes/random");
  QNetworkRequest request;
  request.setUrl(API_ENDPOINT);

  mNetReply = mNetManager->get(request);
  connect(mNetReply, &QIODevice::readyRead, this, &DataSource::dataReadyRead);
  connect(mNetReply, &QNetworkReply::finished, this, &DataSource::dataReadFinished);
}
