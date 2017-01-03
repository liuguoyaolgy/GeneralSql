#include "uidata.h"
#include <QString>
#include <QDebug>
UiData::UiData()
{
    qDebug()<<"init UiData begin";
    listbank = new ListBank();
    listrule = new ListRule();
    qDebug()<<"init UiData end";
}


QString UiData::ListBank::getText()
{
    return text;
}

void UiData::ListBank::setText(QString text)
{
    this->text=text;
}


QString UiData::ListRule::getText()
{
    return text;
}

void UiData::ListRule::setText(QString text)
{
    this->text=text;

}
