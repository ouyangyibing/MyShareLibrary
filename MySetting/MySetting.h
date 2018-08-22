#ifndef MYSETTING_H
#define MYSETTING_H

#include <QWidget>

class QString;
class QSpinBox;
class QComboBox;
class QSettings;
class QGroupBox;
class QLineEdit;
class QDoubleSpinBox;

class MySetting : public QWidget
{
    Q_OBJECT
public:
    explicit MySetting(QWidget *parent = nullptr);
    //子类需要重写此函数，将文件中的值读取至ui
    virtual void readToUi() = 0;//override
    //子类需要重写此函数，将ui的数据保存至文件
    virtual void saveToFile() = 0;//override
    //如ui控件需要禁用，那么重写此函数，将需要禁用的控件添加进去
    virtual void uiDisabled(bool flag = true);
    //子类需要重写此函数，将ui的数据更新至变量
    virtual void syncValud() = 0;//override

    void setFileName(const QString &value);

public:
    QString getKey(const QGroupBox * const groupBox, QWidget *widget);
    void read(QSettings &setting, const QGroupBox * const groupBox);
    void write(QSettings &setting, const QGroupBox * const groupBox);

protected:
    QString fileName = "./setup/setup.ini";
};

#endif // MYSETTING_H
