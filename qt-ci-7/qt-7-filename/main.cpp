#include <QCoreApplication>
#include<QSettings>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString filename=a.applicationDirPath()+"/setting.ini";
    qInfo()<<filename;
    QSettings settings(filename,QSettings::Format::IniFormat);
    settings.beginGroup("people");
    settings.setValue("gouthami",308);
    settings.endGroup();
    settings.sync();
    settings.beginGroup("people");

      qInfo() << "Tammy:" << settings.value("Tammy",QVariant("Person not found!"));
      qInfo() << "gouthami:" << settings.value("gouthami",QVariant("Person not found!"));
    settings.endGroup();


    return a.exec();
}
