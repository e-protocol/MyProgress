#ifndef MSGBOX_H
#define MSGBOX_H

#pragma once
#include <QMessageBox>

class MsgBox : public QMessageBox
{
public:
    MsgBox(const QString& title, const QString &text, int numberOfBtns);
    ~MsgBox();

private:
    const double windowOpacity = 0.9;
};

#endif // MSGBOX_H
