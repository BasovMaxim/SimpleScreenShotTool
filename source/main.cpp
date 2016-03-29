#include <QApplication>
#include <QAbstractEventDispatcher>
#include <QTranslator>
#include <QSystemTrayIcon>


#include "aboutdialog.h"
#include "mainwidget.h"
#include "settings.h"
#include "settingsdialog.h"
#include "QDesktopWidget"

#ifdef HOTKEY_ENABLED

#include "hotkeyeventfilter.h"

#endif

int main(int argc, char *argv[])
{  
    QApplication a(argc, argv);
    Settings::load();

    QString trayToolTip = "Click icon or press Ctrl+PrtScr for making a custom screenshot\nDoubleclick for making a full screen screenshot";

    if(Settings::settings().language == "Russian")
    {
        QTranslator *translator = new QTranslator();
        translator->load(SC_RUSSIAN_LOC_FILE);
        a.installTranslator(translator);
        trayToolTip = "Щелчок по иконке или нажатие Ctrl+PrtScr - скриншот области экрана\nДвойной щелчок - скриншот всего экрана";
    }

    MainWidget w;
    SettingsDialog settingsDialog;

    if(a.arguments().size() > 1)
    {
        if(a.arguments().at(1) == "--screenshot")
        {
            w.show();
            w.setAttribute(Qt::WA_QuitOnClose, true);
            return a.exec();
        }
        else if(a.arguments().at(1) == "--settings")
        {
            settingsDialog.show();
            settingsDialog.setAttribute(Qt::WA_QuitOnClose, true);
            return a.exec();
        }
    }

    AboutDialog aboutDialog;
    QSystemTrayIcon trayIcon;
    QMenu trayMenu;

    trayIcon.setToolTip(trayToolTip);

    QAction *settingsAction = new QAction(QObject::tr("Settings"), &trayIcon);
    QAction *aboutAction = new QAction(QObject::tr("About"), &trayIcon);
    QAction *exitAction = new QAction(QObject::tr("Exit"), &trayIcon);

    trayIcon.setIcon(QIcon(":/ico_16px.png"));

    trayMenu.addAction(settingsAction);
    trayMenu.addSeparator();
    trayMenu.addAction(aboutAction);
    trayMenu.addSeparator();
    trayMenu.addAction(exitAction);

    QObject::connect(settingsAction, &QAction::triggered, &settingsDialog, &SettingsDialog::exec);
    QObject::connect(exitAction, &QAction::triggered, &a, &QApplication::quit);
    QObject::connect(aboutAction, &QAction::triggered, &aboutDialog, &AboutDialog::exec);
    QObject::connect(&trayIcon, &QSystemTrayIcon::activated, [&](QSystemTrayIcon::ActivationReason reason)
    {
        if(reason == QSystemTrayIcon::Trigger)
            w.setVisible(!w.isVisible());

        else if(reason == QSystemTrayIcon::DoubleClick)
        {
            QPixmap screen = QGuiApplication::primaryScreen()->grabWindow(0);

            if(!screen.save(Settings::absoluteFileName(), Settings::settings().imageFormat.toLocal8Bit().constData()))
                QMessageBox::warning(nullptr, "QScreenShotTool:", QObject::tr("Cannot save to file: ") + Settings::absoluteFileName());

            w.close();
        }
    });

    trayIcon.setContextMenu(&trayMenu);
    trayIcon.show();

#ifdef HOTKEY_ENABLED

    HotKeyEventFilter filter(&w);
    a.eventDispatcher()->instance()->installNativeEventFilter(&filter);

#endif

    return a.exec();
}
