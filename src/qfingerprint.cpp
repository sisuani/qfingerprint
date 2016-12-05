/*
 *
 *
 *
 */

#include "qfingerprint.h"

QFingerPrint::QFingerPrint(QObject *parent)
    : QObject(parent)
{
}

struct fp_dscv_dev *QFingerPrint::discover_device(struct fp_dscv_dev **discovered_devs)
{
    struct fp_dscv_dev *ddev = discovered_devs[0];
    struct fp_driver *drv;
    if (!ddev)
        return NULL;

    drv = fp_dscv_dev_get_driver(ddev);
    return ddev;
}

struct fp_dev *QFingerPrint::setupDevice()
{
    struct fp_dev *dev;
    struct fp_dscv_dev *ddev;
    struct fp_dscv_dev **discovered_devs;

    int r = fp_init();
    if (r < 0) {
        m_lastErrorMessage = trUtf8("Error inicializando");
        return NULL;
    }

    discovered_devs = fp_discover_devs();
    if (!discovered_devs) {
        m_lastErrorMessage = trUtf8("No se puede buscar el dispositivo");
        fp_exit();
        return NULL;
    }

    ddev = discover_device(discovered_devs);
    if (!ddev) {
        m_lastErrorMessage = trUtf8("No se encontró el dispositivo");
        fp_exit();
        return NULL;
    }

    dev = fp_dev_open(ddev);
    fp_dscv_devs_free(discovered_devs);
    if (!dev) {
        m_lastErrorMessage = trUtf8("No es posible abrir el dispositivo");
        fp_exit();
        return NULL;
    }

    return dev;
}

unsigned int QFingerPrint::enroll(unsigned char **raw_data)
{
    struct fp_dev *dev = setupDevice();
    if (!dev)
        return 0;

    struct fp_print_data *data = NULL;
    int r = -1;

    do {
        r = fp_enroll_finger(dev, &data);
        if (r < 0) {
            m_lastErrorMessage = trUtf8("No se puede leer la huella digital");
            emit errorMessage();
            break;
        }

        switch (r) {
            case FP_ENROLL_COMPLETE:
                m_lastSuccessfulMessage = trUtf8("Lectura correcta!");
                emit errorMessage();
                break;
            case FP_ENROLL_FAIL:
                m_lastErrorMessage = trUtf8("Lectura incorrecta");
                emit errorMessage();
                data = NULL;
            case FP_ENROLL_PASS:
                break;
            case FP_ENROLL_RETRY:
                m_lastErrorMessage = trUtf8("Lectura incorrecta");
                emit errorMessage();
                break;
            case FP_ENROLL_RETRY_TOO_SHORT:
                m_lastErrorMessage = trUtf8("La lectura fue muy corta");
                emit errorMessage();
                break;
            case FP_ENROLL_RETRY_CENTER_FINGER:
                m_lastErrorMessage = trUtf8("Centrá el dedo en el dispositivo");
                emit errorMessage();
                break;
            case FP_ENROLL_RETRY_REMOVE_FINGER:
                m_lastErrorMessage = trUtf8("Move el dedo e intentá nuevamente");
                emit errorMessage();
                break;
        }
    } while (r != FP_ENROLL_COMPLETE);

    fp_dev_close(dev);

    r = fp_print_data_save(data, RIGHT_INDEX);
    if (r < 0) {
        m_lastErrorMessage = trUtf8("No es posible guardar los datos");
        fp_print_data_free(data);
        return 0;
    }

    unsigned int length = fp_print_data_get_data(data, raw_data);
    fp_print_data_free(data);

    return length;
}

bool QFingerPrint::verify(const unsigned int length, unsigned char *raw_data)
{
    struct fp_dev *dev = setupDevice();
    if (!dev)
        return false;

    struct fp_print_data *data = fp_print_data_from_data(raw_data, length);
    int r = -1;

    r = fp_verify_finger(dev, data);
    fp_dev_close(dev);
    if (r < 0) {
        m_lastErrorMessage = trUtf8("No se pudo verificar la huella, intente nuevamente");
        return false;
    }

    switch (r) {
        case FP_VERIFY_NO_MATCH:
            m_lastErrorMessage = trUtf8("La huella no coincide, intente nuevamente");
            return false;
        case FP_VERIFY_MATCH:
            return true;
        case FP_VERIFY_RETRY:
            m_lastErrorMessage = trUtf8("La lectura incorrecta, intente nuevamente");
            return false;
        case FP_VERIFY_RETRY_TOO_SHORT:
            m_lastErrorMessage = trUtf8("La lectura fue muy corta");
            return false;
        case FP_VERIFY_RETRY_CENTER_FINGER:
            m_lastErrorMessage = trUtf8("Centrá el dedo en el dispositivo");
            return false;
        case FP_VERIFY_RETRY_REMOVE_FINGER:
            m_lastErrorMessage = trUtf8("Move el dedo e intentá nuevamente");
            return false;
    }

    m_lastErrorMessage = trUtf8("Error desconocido");
    return false;
}

QString QFingerPrint::lastErrorMessage() const
{
    return m_lastErrorMessage;
}

QString QFingerPrint::lastSuccessfulMessage() const
{
    return m_lastSuccessfulMessage;
}
