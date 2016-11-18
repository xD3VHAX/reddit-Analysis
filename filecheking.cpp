#include "FileCheking.h"
#include <QtCore>

QString FileName;
void delay(int wait);

#ifdef DEBUG
QString DEBUG_EXT = "d";
#else
QString DEBUG_EXT = "";
#endif

QString settings;

FileCheking::FileCheking(QObject *parent) :
    QThread(parent)
{
}


void  FileCheking::run()
{
    QFile file;
    file.setFileName("data/"+FileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    settings = file.readAll();

    file.close();


    QJsonDocument doc = QJsonDocument::fromJson(settings.toUtf8());
    QJsonObject obj = doc.object();
    QJsonArray jsonArray = obj["messages"].toArray();



    QJsonDocument doc2(obj);
    QString strJson2(doc2.toJson(QJsonDocument::Compact));
    Q_EMIT FileSize(strJson2.size());

    foreach (const QJsonValue & value, jsonArray) {

        delay(1);
        QJsonArray jsonArray2 = value.toArray();
        QJsonObject obj = value.toObject();
        Q_EMIT CheckingThisFile(obj);

    }


     Q_EMIT loadFinished(true);
     settings.clear(); // Release memory

   //


}
