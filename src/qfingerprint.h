#ifndef QFINGERPRINT
#define QFINGERPRINT

#include <QObject>
#include <libfprint/fprint.h>


class QFingerPrint : public QObject
{
    Q_OBJECT

public:
    QFingerPrint(QObject *parent = 0);

    unsigned int enroll(unsigned char **raw_data);
    bool verify(const unsigned int length, unsigned char *raw_data);

    QString lastErrorMessage() const;
    QString lastSuccessfulMessage() const;

signals:
    void errorMessage();

private:
    struct fp_dev *setupDevice();
    struct fp_dscv_dev *discover_device(struct fp_dscv_dev **discovered_devs);

    QString m_lastErrorMessage;
    QString m_lastSuccessfulMessage;
};

#endif // QFINGERPRINT
