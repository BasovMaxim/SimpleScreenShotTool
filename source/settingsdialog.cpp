#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    ui->fileNameFormatLineEdit->setValidator(new FileNameFormatValidator(ui->fileNameFormatLineEdit));
    QStringList cl = {"white", "black"};

    m_colorWidget = new ColorWidget();
    ui->colorLayout->addWidget(m_colorWidget);

    connect(ui->fillBackGroundcheckBox, &QCheckBox::stateChanged, m_colorWidget, &ColorWidget::setEnabled);
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->standardButton(button) == QDialogButtonBox::Ok)
    {
        saveSettings();
    }
    else if(ui->buttonBox->standardButton(button) == QDialogButtonBox::RestoreDefaults)
    {
        Settings::restore();
        loadSettings();
    }
}

void SettingsDialog::on_chengeDirectoryButton_clicked()
{
    QString newDirectory = QFileDialog::getExistingDirectory(this, tr("Default folder"), ui->fileDirectoryValueLabel->text());
    if(newDirectory.isNull())
        return;

    Settings::settings().fileDirectory = newDirectory;
    ui->fileDirectoryValueLabel->setText(newDirectory);
}

void SettingsDialog::showEvent(QShowEvent *)
{
    loadSettings();
}

void SettingsDialog::loadSettings()
{
    //Settings::printToScreen();
    ui->lenseSizeSpinBox->setValue(Settings::settings().lenseSize);
    ui->lenseScaleFactorSpinBox->setValue(Settings::settings().lenseScaleFactor);
    ui->fileDirectoryValueLabel->setText(Settings::settings().fileDirectory);
    ui->fileNameFormatLineEdit->setText(Settings::settings().fileNameFormat);
    ui->imageFormatComboBox->setCurrentText(Settings::settings().imageFormat);
    ui->fillBackGroundcheckBox->setChecked(Settings::settings().fillBackgroind);
    ui->closeAfterSaveCheckBox->setChecked(Settings::settings().closeAfterSave);
    ui->closeAfterCopyCheckBox->setChecked(Settings::settings().closeAfterCopy);
    ui->saveSelectionCheckBox->setChecked(Settings::settings().saveSelectionFrame);
    ui->saveAfterSelectionCheckBox->setChecked(Settings::settings().saveAfterSelection);
    ui->copyAfterSelectionCheckBox->setChecked(Settings::settings().copyAfterSelection);
    ui->languageComboBox->setCurrentText(Settings::settings().language);
    m_colorWidget->setColor(Settings::settings().fillColor);
}

void SettingsDialog::saveSettings()
{
    Settings::settings().lenseSize          = ui->lenseSizeSpinBox->value();
    Settings::settings().lenseScaleFactor   = ui->lenseScaleFactorSpinBox->value();
    Settings::settings().fileDirectory      = ui->fileDirectoryValueLabel->text();
    Settings::settings().fileNameFormat     = ui->fileNameFormatLineEdit->text();
    Settings::settings().imageFormat        = ui->imageFormatComboBox->currentText();
    Settings::settings().fillBackgroind     = ui->fillBackGroundcheckBox->isChecked();
    Settings::settings().closeAfterSave     = ui->closeAfterSaveCheckBox->isChecked();
    Settings::settings().closeAfterCopy     = ui->closeAfterCopyCheckBox->isChecked();
    Settings::settings().saveSelectionFrame = ui->saveSelectionCheckBox->isChecked();
    Settings::settings().saveAfterSelection = ui->saveAfterSelectionCheckBox->isChecked();
    Settings::settings().copyAfterSelection = ui->copyAfterSelectionCheckBox->isChecked();
    Settings::settings().language           = ui->languageComboBox->currentText();
    Settings::settings().fillColor          = m_colorWidget->color();

    Settings::save();
}
