#include "settings.h"


Settings::Values &Settings::settings()
{
    return Settings::instance().m_values;
}

void Settings::load()
{
    Settings::Values &values = Settings::instance().settings();

    values.lenseScaleFactor     = Settings::instance().value(S_LENSE_SCALE_FACTOR, 4).toInt();
    values.lenseSize            = Settings::instance().value(S_LENSE_SIZE, 100).toInt();

    values.fileDirectory        = Settings::instance().value(S_FILE_DIRECTORY, SC_DEFAULT_FILE_DIRECTORY).toString();
    if(values.fileDirectory.isEmpty()) values.fileDirectory = SC_DEFAULT_FILE_DIRECTORY;

    values.fileNameFormat       = Settings::instance().value(S_FILE_NAME_FORMAT, SC_DEFAULT_FILE_NAME_FORMAT).toString();
    if(values.fileNameFormat.isEmpty()) values.fileNameFormat = SC_DEFAULT_FILE_NAME_FORMAT;

    values.imageFormat          = Settings::instance().value(S_IMAGE_FORMAT, "PNG").toString();
    if(values.imageFormat.isEmpty()) values.imageFormat = "PNG";

    values.fillBackgroind       = Settings::instance().value(S_FILL_BACKGROUND, false).toBool();
    values.closeAfterSave       = Settings::instance().value(S_CLOSE_AFTER_SAVE, true).toBool();
    values.fillColor            = Settings::instance().value(S_FILL_CLOLOR, QColor(0, 0, 0, 127)).value<QColor>();
    values.markerWidth          = Settings::instance().value(S_MARKER_WIDTH, 3).toInt();
    values.saveSelectionFrame   = Settings::instance().value(S_SAVE_SELECTION_FRAME, false).toBool();
    values.saveAfterSelection   = Settings::instance().value(S_SAVE_AFTER_SELECTION, false).toBool();
    values.closeAfterCopy       = Settings::instance().value(S_CLOSE_AFTER_COPY, true).toBool();
    values.copyAfterSelection   = Settings::instance().value(S_COPY_AFTER_SELECTION, false).toBool();
    values.language             = Settings::instance().value(S_LANGUAGE, "English").toString();
}

void Settings::save()
{
    Settings::Values &values = Settings::instance().settings();

    Settings::instance().setValue(S_LENSE_SCALE_FACTOR, values.lenseScaleFactor);
    Settings::instance().setValue(S_LENSE_SIZE, values.lenseSize);
    Settings::instance().setValue(S_FILE_DIRECTORY, values.fileDirectory);
    Settings::instance().setValue(S_FILE_NAME_FORMAT, values.fileNameFormat);
    Settings::instance().setValue(S_IMAGE_FORMAT, values.imageFormat);
    Settings::instance().setValue(S_FILL_BACKGROUND, values.fillBackgroind);
    Settings::instance().setValue(S_CLOSE_AFTER_SAVE, values.closeAfterSave);
    Settings::instance().setValue(S_FILL_CLOLOR, values.fillColor);
    Settings::instance().setValue(S_MARKER_WIDTH, values.markerWidth);
    Settings::instance().setValue(S_SAVE_SELECTION_FRAME, values.saveSelectionFrame);
    Settings::instance().setValue(S_SAVE_AFTER_SELECTION, values.saveAfterSelection);
    Settings::instance().setValue(S_CLOSE_AFTER_COPY, values.closeAfterCopy);
    Settings::instance().setValue(S_COPY_AFTER_SELECTION, values.copyAfterSelection);
    Settings::instance().setValue(S_LANGUAGE, values.language);
}

void Settings::restore()
{
    Settings::instance().clear();
    Settings::load();
}

QString Settings::formatFileName(const QString &format)
{
    return QTime::currentTime().toString(QDate::currentDate().toString(format)) + "." + Settings::settings().imageFormat.toLower();
}

QString Settings::absoluteFileName()
{
    QString fileDirectory = Settings::settings().fileDirectory;
    QString fileName = Settings::formatFileName(Settings::settings().fileNameFormat);
    return  fileDirectory + "/" + fileName;
}

Settings &Settings::instance()
{
    static Settings instance = Settings();
    return instance;
}

Settings::Settings() :
    QSettings(SC_SETTINGS_FILE_NAME, QSettings::IniFormat)
{
}

