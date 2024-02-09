#include <QCoreApplication>
#include "test.h"
void lifeCycle()
{
    Test mom;
    Test child;
    mom.setObjectName("mom");
    child.setObjectName("child");

    QObject::connect(&mom,&Test::alarm,&child,&Test::wake);
    mom.testing();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    lifeCycle();
    return a.exec();
}
