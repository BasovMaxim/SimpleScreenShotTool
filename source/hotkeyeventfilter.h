#ifndef HOTKEYEVENTFILTER_H
#define HOTKEYEVENTFILTER_H

#include <QAbstractNativeEventFilter>

#include "windows.h"
#include "mainwidget.h"

class HotKeyEventFilter : public QAbstractNativeEventFilter
        //The class provide a interception of a global systen hotkeys as a native system messange
        //Captures Ctrl+PRINTSCREEN compination
        //Works only under windows systems.
{
public:
    HotKeyEventFilter(MainWidget *parent = 0);
    ~HotKeyEventFilter();

private:
    MainWidget *m_parent;
    bool nativeEventFilter(const QByteArray &, void *message, long *);

};

#endif // CUSTOMGUIAPPLICATION_H
