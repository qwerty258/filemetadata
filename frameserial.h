#ifndef FRAMESERIAL_H
#define FRAMESERIAL_H

#include <QFrame>

#include "datastructdefines.h"

namespace Ui {
class FrameSerial;
}

class FrameSerial : public QFrame
{
    Q_OBJECT

public:
    explicit FrameSerial(QWidget *parent = nullptr);
    ~FrameSerial();
    void set_data(serial_metadata_t &serial_metadata);
    void get_data(serial_metadata_t &serial_metadata);

private:
    Ui::FrameSerial *ui;
};

#endif // FRAMESERIAL_H
