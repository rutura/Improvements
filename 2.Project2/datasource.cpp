#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include "datasource.h"

// #define USE_ICNDB_JOKE_SOURCE // uncomment to enable icndb joke source

DataSource::DataSource(QObject *parent) : QObject(parent),
    mNetManager(new QNetworkAccessManager(this)),
    mNetReply(nullptr),
    mDataBuffer(new QByteArray)
{
    connect(this, &DataSource::requestCompleted, this, &DataSource::makeRequest);
}

void DataSource::fetchJokes(int number)
{
    switch(mRequestState){
    case REQUESTS_ONGOING:
        mRequestsToMake += number;
        break;
    case REQUESTS_COMPLETED:
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
    Joke *joke = new Joke("Test Joke Added First",this);
    addJoke(joke);
}

void DataSource::addJoke(const QString &jokeParam)
{
    Joke *joke = new Joke(jokeParam,this);
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
    if ( !mJokes.isEmpty()){
        removeJoke(mJokes.size()-1);
    }
}

QList<Joke *> DataSource::dataItems()
{
    return mJokes;
}

void DataSource::dataReadyRead()
{
     mDataBuffer->append(mNetReply->readAll());
}

void DataSource::dataReadFinished()
{
    //Parse the JSON
    if( mNetReply->error()){
        qDebug() << "There was some error : " << mNetReply->errorString();
    }else{
        //
        //Turn the data into a json document
        QJsonDocument doc = QJsonDocument::fromJson(*mDataBuffer);
        //Grab the value array
        QJsonObject data = doc.object();
#if !defined(USE_ICNDB_JOKE_SOURCE)
        QString joke = data["value"].toString();
        addJoke(joke);
#else
        QJsonArray array = data["value"].toArray();

        qDebug() << "Number of jokes " << array.size();

        for ( int i = 0; i < array.size() ; i++){

            QJsonObject object = array.at(i).toObject();

            QString joke = object["joke"].toString();

            addJoke(joke);
        }
        mRequestsToMake = 0;
#endif
        //Clear the buffer
        mDataBuffer->clear();
        if(mRequestsToMake > 0){
            --mRequestsToMake;
            mRequestState = REQUESTS_ONGOING;
            emit requestCompleted();
        }else{
            mRequestState = REQUESTS_COMPLETED;
        }
    }
}

void DataSource::makeRequest()
{
#if defined(USE_ICNDB_JOKE_SOURCE)
    //Initialize our API data
    const QUrl API_ENDPOINT("http://api.icndb.com/jokes/random/"+QString::number(mRequestsToMake));
#else
    const QUrl API_ENDPOINT("https://api.chucknorris.io/jokes/random");
#endif
    QNetworkRequest request;
    request.setUrl(API_ENDPOINT);

    mNetReply = mNetManager->get(request);
    connect(mNetReply,&QIODevice::readyRead,this,&DataSource::dataReadyRead);
    connect(mNetReply,&QNetworkReply::finished,this,&DataSource::dataReadFinished);
}
