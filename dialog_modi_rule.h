#ifndef DIALOG_MODI_RULE_H
#define DIALOG_MODI_RULE_H

#include <QDialog>
class QAbstractButton;
class DataBaseSqlite;

namespace Ui {
class Dialog_modi_rule;
}

class Dialog_modi_rule : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_modi_rule(DataBaseSqlite *db, QString s_rule_no, QWidget *parent = 0);
    ~Dialog_modi_rule();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::Dialog_modi_rule *ui;
    DataBaseSqlite *db;
    QString s_rule_no;
};

#endif // DIALOG_MODI_RULE_H
