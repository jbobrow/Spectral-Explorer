#include "setting.h"
#include <QDebug>

Setting::Setting(QString key, QVariant value, QObject *parent) : QObject(parent) {
    mKey = key;
    mValue = value;
}
Setting::Setting(QString key, int value, QObject *parent) : QObject(parent) {
    mKey = key;
    mValue = QVariant(value);
}
Setting::Setting(QString key, bool value, QObject *parent) : QObject(parent) {
    mKey = key;
    mValue = QVariant(value);
}

void Setting::load(const QSettings &s) {
    setValue(s.value(mKey,mValue));
    apply();
    qDebug() << "[Sett] Load Setting" << mKey << ":" << mValue;
}

void Setting::save(QSettings &s) {
    apply();
    s.setValue(mKey,mValue);
    qDebug() << "[Sett] Save Setting" << mKey << ":" << mValue;
}

void Setting::reset()
{
    setValue(mValue);
    apply();
    qDebug() << "[Sett] Reset Setting" << mKey << ":" << mValue;
}

QVariant Setting::value() const
{
    return mValue;
}

void Setting::setValue(const QVariant &value)
{
    setTempValue(value);
    updateValue(mTempValue.toInt());
}

void Setting::setTempValue(const QVariant &value)
{
    mTempValue = value;
    if (mAutoApply) apply();
}

void Setting::addWidget(QSpinBox *w)
{
    w->setValue(mTempValue.toInt());
    connect(w, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
    connect(this,SIGNAL(updateValue(int)), w, SLOT(setValue(int)));
}

void Setting::addWidget(QComboBox *w)
{
    w->setCurrentIndex(mTempValue.toInt());
    connect(w, SIGNAL(currentIndexChanged(int)), this, SLOT(onValueChanged(int)));
    connect(this,SIGNAL(updateValue(int)), w, SLOT(setCurrentIndex(int)));
}

void Setting::addWidget(QCheckBox *w)
{
    w->setChecked(mTempValue.toBool());
    connect(w, SIGNAL(toggled(bool)), this, SLOT(onValueChanged(bool)));
    connect(this, SIGNAL(updateValue(bool)), w, SLOT(setChecked(bool)));
}

void Setting::onValueChanged(int v)
{
    // don't use setValue or updateValue (endless loop)
    setTempValue(v);
}

void Setting::onValueChanged(bool v)
{
    // don't use setValue or updateValue (endless loop)
    setTempValue(v);
}

void Setting::apply()
{
    mValue = mTempValue;
}

bool Setting::autoApply() const
{
    return mAutoApply;
}

void Setting::setAutoApply(bool autoApply)
{
    mAutoApply = autoApply;
}

