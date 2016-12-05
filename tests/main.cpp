#include <QCoreApplication>
#include <QDebug>


#include "../src/qfingerprint.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    unsigned int length = 0;
    unsigned char *raw_data = NULL;

    QFingerPrint fingerPrint;
    length = fingerPrint.enroll(&raw_data);
    if (length <= 0)
        qDebug() << "ENROLL ERR: " << fingerPrint.lastErrorMessage();
    else
        qDebug() << "ENROLL OK: " << fingerPrint.lastSuccessfulMessage();

    if (!fingerPrint.verify(length, raw_data))
        qDebug() << "VERIFY ERR: " << fingerPrint.lastErrorMessage();
    else
        qDebug() << "VERIFY OK: " << fingerPrint.lastSuccessfulMessage();

    return 0;
}
