#include "msgBox.h"

MsgBox::MsgBox(const QString &title, const QString &text, int numberOfBtns)
{
    setWindowFlag(Qt::FramelessWindowHint);
    setWindowOpacity(windowOpacity);
    setTextFormat(Qt::RichText);
    setText(title + "<br><br><br>" + text + "<br>");

    switch(numberOfBtns)
    {
        case(1):
        {
            addButton("OK", QMessageBox::YesRole);
            break;
        }
        case(2):
        {
            addButton(QMessageBox::Yes);
            addButton(QMessageBox::No);
            break;
        }
        default: break;
    }
}

MsgBox::~MsgBox()
{

}
