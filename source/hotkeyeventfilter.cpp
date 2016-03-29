#include "hotkeyeventfilter.h"

HotKeyEventFilter::HotKeyEventFilter(MainWidget *parent) :
    m_parent(parent)
{
    RegisterHotKey(NULL, 1, MOD_CONTROL, VK_SNAPSHOT);
}

HotKeyEventFilter::~HotKeyEventFilter()
{

}

bool HotKeyEventFilter::nativeEventFilter(const QByteArray &, void *message, long *)
{
    MSG *msg = static_cast<MSG *>(message);
    if(msg->message == WM_HOTKEY)
    {
        m_parent->show();
        m_parent->raise();
        m_parent->activateWindow();
    }
    return false;
}
