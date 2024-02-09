#include <QCoreApplication>

void display(QByteArrayView &view)
{
    qInfo()<<view;\
    qInfo()<<"sizeof:"<<view.size();
    qInfo()<<"indexof:"<<view.indexOf('e');
    qInfo()<<"slice of"<<view.sliced(2,6);
    //qInfo()<<"remove:"<<view.remove(1); read only
    qInfo()<<"data:"<<view.data();
    qInfo()<<"contains: "<<view.contains('h');
    qInfo()<<"constdata:"<<view.constData();
    qInfo()<<"view at():"<<view.at(5);
    qInfo()<<"view[]:"<<view[20];

}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
QByteArray say="hi hello how are you!!";
QByteArrayView view(say);
display(view);
return a.exec();
}
