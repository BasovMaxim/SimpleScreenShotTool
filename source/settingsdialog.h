#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QAbstractButton>
#include <QColorDialog>
#include <QFileDialog>
#include <QPainter>
#include <QValidator>

#include "settings.h"

namespace Ui {
class SettingsDialog;
}

class FileNameFormatValidator : public QValidator //The validator of a file name format
{
public:
    FileNameFormatValidator(QObject *parent = 0) : QValidator(parent) {}
    ~FileNameFormatValidator() {}

private:
    QValidator::State validate(QString &input, int &) const override
    {
        if(input.contains('\\') || input.contains('/')  || input.contains(':')  ||
           input.contains('*')  || input.contains('?')  || input.contains('\"') ||
           input.contains('<')  || input.contains('>')  || input.contains('|')  || input.isEmpty())
            return QValidator::Invalid;

        return QValidator::Acceptable;

    }

    void fixup(QString &input) const override
    {
        if(input.isEmpty())
        {
            input.append(SC_DEFAULT_FILE_NAME_FORMAT);
            return;
        }

        input.remove('\\');
        input.remove('/');
        input.remove(':');
        input.remove('*');
        input.remove('?');
        input.remove('\"');
        input.remove('<');
        input.remove('>');
        input.remove('|');
    }
};


class ColorWidget : public QWidget //The class provide a simple way to chouse color with a color dialog.
{
    Q_OBJECT

public:
    ColorWidget(QWidget *parent = 0) : QWidget(parent) {}
    ~ColorWidget() {}

    void setColor(const QColor &color)
    {
        m_color = color;
    }

    QColor color() const
    {
        return m_color;
    }

private:
    QColor m_color;

    void paintEvent(QPaintEvent *) override
    {
        QPainter p;
        p.begin(this);
        p.fillRect(rect(), m_color);
        p.drawRect(rect().adjusted(0, 0, -1, -1));
        p.end();
    }

    void mousePressEvent(QMouseEvent *) override
    {
        QColor newColor = QColorDialog::getColor(m_color, this, tr("Color"), QColorDialog::ShowAlphaChannel);
        if(newColor.isValid())
            m_color = newColor;
    }
};

class SettingsDialog : public QDialog //The class for edit app's settings.
{
    Q_OBJECT

public:
    SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_chengeDirectoryButton_clicked();

private:
    Ui::SettingsDialog *ui;
    ColorWidget *m_colorWidget;

    void showEvent(QShowEvent *) override;

    void loadSettings();
    void saveSettings();
};

#endif // SETTINGSDIALOG_H
