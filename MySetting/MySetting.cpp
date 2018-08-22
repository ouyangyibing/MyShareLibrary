#include "MySetting.h"
#include <QString>
#include <QSettings>
#include <QGroupBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QWidget>
#include <QComboBox>

MySetting::MySetting(QWidget *parent) : QWidget(parent)
{    

}

void MySetting::uiDisabled(bool flag)
{
    Q_UNUSED(flag)
}

void MySetting::read(QSettings &setting, const QGroupBox * const groupBox)
{
    foreach (QLineEdit* child, groupBox->findChildren<QLineEdit*>()) {
        child->setText(setting.value(getKey(groupBox, child), child->text()).toString());
    }

    foreach (QTextEdit* child, groupBox->findChildren<QTextEdit*>()) {
        child->setPlainText(setting.value(getKey(groupBox, child), child->toPlainText()).toString());
    }

    foreach (QDoubleSpinBox* child, groupBox->findChildren<QDoubleSpinBox*>()) {
        child->setValue(setting.value(getKey(groupBox, child), child->value()).toDouble());
    }

    foreach (QSpinBox* child, groupBox->findChildren<QSpinBox*>()) {
        child->setValue(setting.value(getKey(groupBox, child), child->value()).toInt());
    }

    foreach (QComboBox* child, groupBox->findChildren<QComboBox*>()) {
        child->setCurrentText(setting.value(getKey(groupBox, child), child->currentText()).toString());
    }
}

void MySetting::write(QSettings &setting, const QGroupBox * const groupBox)
{
    foreach (QLineEdit* child, groupBox->findChildren<QLineEdit*>()) {
        setting.setValue(getKey(groupBox, child), child->text());
    }

    foreach (QTextEdit* child, groupBox->findChildren<QTextEdit*>()) {
        setting.setValue(getKey(groupBox, child), child->toPlainText());
    }

    foreach (QDoubleSpinBox* child, groupBox->findChildren<QDoubleSpinBox*>()) {
        setting.setValue(getKey(groupBox, child), child->value());
    }

    foreach (QSpinBox* child, groupBox->findChildren<QSpinBox*>()) {
        setting.setValue(getKey(groupBox, child), child->value());
    }

    foreach (QComboBox* child, groupBox->findChildren<QComboBox*>()) {
        setting.setValue(getKey(groupBox, child), child->currentText());
    }
}

void MySetting::setFileName(const QString &value)
{
    fileName = value;
}

QString MySetting::getKey(const QGroupBox * const groupBox, QWidget *widget)
{
    return QString("%1/%2").arg(groupBox->objectName(), widget->objectName());
}
