#ifndef DIALOG_ADD_RULE_H
#define DIALOG_ADD_RULE_H

#include <QDialog>
#include "databasesqlite.h"
class QAbstractButton;
namespace Ui {
class Dialog_add_rule;
}

class Dialog_add_rule : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_add_rule(DataBaseSqlite *db, QString s_bank_name, QString s_rule_name, QWidget *parent = 0);
    ~Dialog_add_rule();

private slots:


    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::Dialog_add_rule *ui;
    DataBaseSqlite *db;
    QString s_bank_name;
    QString s_rule_name;
};

#endif // DIALOG_ADD_RULE_H
