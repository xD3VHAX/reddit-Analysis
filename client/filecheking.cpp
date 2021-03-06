#include "FileCheking.h"
#include <QtCore>
#include <QApplication>
#include <list.h>
#include <utils.h>
QString FileName;
int NumberOfLine=0;
float loadedData;
void delay(int wait);

QString settings;

FileCheking::FileCheking(QObject *parent) :
    QThread(parent)
{
}


// On ouvre le fichié séléctionné et on le parse tout en remplissant notre liste chainée
void  FileCheking::run()
{

    QFile file;
    file.setFileName("data/"+FileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    settings = file.readAll();
    file.close();
    int count=0;

    QJsonDocument doc = QJsonDocument::fromJson(settings.toUtf8());
    QJsonObject obj = doc.object();
    QJsonArray jsonArray = obj["messages"].toArray();

    QJsonDocument doc2(obj);
    QString strJson2(doc2.toJson(QJsonDocument::Compact));
    Q_EMIT FileSize(strJson2.size());

    foreach (const QJsonValue & value, jsonArray) {
        NumberOfLine++;
        QCoreApplication::processEvents();
        delay(0.1);
        QJsonArray jsonArray2 = value.toArray();
        QJsonObject obj = value.toObject();
        QJsonDocument doc(obj);
        QString strJson(doc.toJson(QJsonDocument::Compact));
        loadedData +=  strJson.length()+3;

        Q_EMIT CheckingThisFile( loadedData);

        list->addAtFront(obj);
        debug->print_msg(obj["name"].toString());
        count++;
        // this->msleep(1);
        // delay(1);
    }


    Q_EMIT loadFinished(true);
    settings.clear();

}

