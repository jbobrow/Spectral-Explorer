#ifndef SETTING_H
#define SETTING_H

#include <QString>
#include <QVariant>
#include <QSettings>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>

class Setting : public QObject
{
    Q_OBJECT
public:
    Setting(QString key, QVariant value, QObject *parent = nullptr);
    Setting(QString key, int value, QObject *parent = nullptr);
    Setting(QString key, bool value, QObject *parent = nullptr);

    void addWidget(QSpinBox *w);
    void addWidget(QComboBox *w);
    void addWidget(QCheckBox *w);

    void load(const QSettings &s);
    void save(QSettings &s);
    /// Restore mValue and overwrite mTempValue
    void reset();
    /// Apply mTempValue and save it in mValue
    void apply();

    void setTempValue(const QVariant &value);
    QVariant value() const;
    void setValue(const QVariant &value);

    bool autoApply() const;
    void setAutoApply(bool autoApply);

signals:
    void updateValue(int v);
    void updateValue(bool v);

public slots:
    void onValueChanged(int v);
    void onValueChanged(bool v);

private:
    QString mKey;
    /// Official value of the setting which is used from other classes
    QVariant mValue;
    /// Temporary value of the setting which is used for the ui. The
    /// methods load, save and apply will save mTempValue in mValue.
    QVariant mTempValue;
    bool mAutoApply = false;
};

#endif // SETTING_H
