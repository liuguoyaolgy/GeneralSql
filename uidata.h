#ifndef UIDATA_H
#define UIDATA_H
//class QString;
#include <QString>
class UiData
{
public:
    UiData();
    class ListBank{
    public:
        QString getText();
        void setText(QString text);
    private:
        QString text;
    };
    class ListRule{
    public:
        QString getText();
        void setText(QString text);
    private:
        QString text;
    };
    ListBank *listbank;
    ListRule *listrule;
};

#endif // UIDATA_H
