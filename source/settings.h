#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDate>
#include <QDir>
#include <QColor>
#include <QSettings>

const int SC_DEFAULTIMAGEFORMAT             = 0;
const QString SC_SETTINGS_FILE_NAME         = "Settings.ini";
const QString SC_DEFAULT_FILE_NAME_FORMAT   = "'Screen_'hh.mm.ss_dd.MM.yy";
const QString SC_DEFAULT_FILE_DIRECTORY     = QDir::homePath() + "/Desktop";
const QString SC_RUSSIAN_LOC_FILE           = ":/sst_ru.qm";
const Qt::KeyboardModifier SC_LENSE_MODIFER = Qt::ShiftModifier;

const QString S_FRAME_PEN               = "general/framepen";
const QString S_LENSE_SIZE              = "general/lensesize";
const QString S_LENSE_SCALE_FACTOR      = "general/lensescalefactor";
const QString S_FILE_DIRECTORY          = "general/filedirectory";
const QString S_FILE_NAME_FORMAT        = "general/filenameformat";
const QString S_IMAGE_FORMAT            = "general/fimageforamt";
const QString S_FILL_BACKGROUND         = "general/fillbackground";
const QString S_CLOSE_AFTER_SAVE        = "general/closeaftersave";
const QString S_FILL_CLOLOR             = "general/fullcolor";
const QString S_MARKER_WIDTH            = "general/markerwidth";
const QString S_SAVE_SELECTION_FRAME    = "general/saveselectionframe";
const QString S_SAVE_AFTER_SELECTION    = "general/saveafterselection";
const QString S_CLOSE_AFTER_COPY        = "general/closeaftercopy";
const QString S_COPY_AFTER_SELECTION    = "general/copyafterselection";
const QString S_LANGUAGE                = "general/lang";

class Settings : public QSettings //The global class-singleton provide the app's settings.
{
    Q_OBJECT

public:
    //Structure-container for fields of settings.
    struct Values {
        bool fillBackgroind;
        bool closeAfterSave;
        bool saveSelectionFrame;
        bool saveAfterSelection;
        bool closeAfterCopy;
        bool copyAfterSelection;
        int lenseSize;
        int lenseScaleFactor;
        int markerWidth;
        QColor fillColor;
        QString fileDirectory;
        QString fileNameFormat;
        QString imageFormat;
        QString language;
    };

    static Settings::Values &settings(); //Return values

    static void load();
    static void save();
    static void restore();

    static QString formatFileName(const QString &format); //Format a file name acording with mask.
    static QString absoluteFileName(); //Return an absolute default file name

private:
    Values m_values;

    static Settings &instance(); //Singletone instance

    Settings();
    Settings(const Settings &root);
    Settings& operator=(const Settings &);
};

#endif // SETTINGS_H
