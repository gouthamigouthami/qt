#include "test.h"

Test::Test(QObject *parent)
    : QObject{parent}
{
    qDebug()<<this<<"constructed";
}

void Test::testing()
{
    QString noise="Wake Up!!";
    emit alarm(noise);
}

Test::~Test()
{
    qDebug()<<this <<" destructor";
}

void Test::wake(QString noise)
{
    qDebug()<<"from"<<sender()<<noise;
}
