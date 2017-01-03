#include "dialog_add_rule.h"
#include "ui_dialog_add_rule.h"
#include <QAbstractButton>
#include <QPushButton>
#include <QDebug>
#include "databasesqlite.h"
#include <QCompleter>
#include <QMessageBox>

Dialog_add_rule::Dialog_add_rule(DataBaseSqlite *db, QString s_bank_name, QString s_rule_name, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_add_rule)
{
    ui->setupUi(this);
    //db = new DataBaseSqlite();
    this->db=db;
    this->s_bank_name=s_bank_name;
    this->s_rule_name=s_rule_name;
    //自动完成功能
    QCompleter *pCompleterbank = new QCompleter(ui->comboBox_bank->model(), this);
    ui->comboBox_bank->setCompleter(pCompleterbank);
    QCompleter *pCompleterrule = new QCompleter(ui->comboBox_rule->model(), this);
    ui->comboBox_bank->setCompleter(pCompleterrule);

    QStringList bankList ;
    bankList=db->getAllBankName();
    foreach (QString str,bankList)
      ui->comboBox_bank->addItem(str);
    QStringList ruleList;
    ruleList=db->getAllRuleName();
    foreach (QString str,ruleList)
      ui->comboBox_rule->addItem(str);
    ui->comboBox_bank->setCurrentText(s_bank_name);
    ui->comboBox_rule->setCurrentText(s_rule_name);

    this->setLayout(ui->formLayout);
}

Dialog_add_rule::~Dialog_add_rule()
{
    delete ui;
}



void Dialog_add_rule::on_buttonBox_clicked(QAbstractButton *button)
{

    if(ui->buttonBox->button(QDialogButtonBox::Ok) == button)
    {
        qDebug()<<"OK press";
        QMessageBox msgBox(QMessageBox::Warning,QString(tr("提示")),QString(tr(" \n确定插入？\n")));
        msgBox.setStandardButtons(QMessageBox::Yes| QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret=msgBox.exec();
        if(ret==QMessageBox::Yes)
        {
            db->setAnRule(ui->comboBox_bank->currentText(),
                          ui->comboBox_rule->currentText(),
                          ui->lineEdit_table_head->text(),
                          ui->lineEdit_fmt_str->text(),
                          ui->lineEdit_data_eg->text(),
                          ui->lineEdit_filter->text(),
                          ui->lineEdit_filename->text(),
                          ui->lineEdit_pre->text(),
                          ui->lineEdit_append->text());
        }
        else if(ret==QMessageBox::No)
        {

        }
    }
    if(ui->buttonBox->button(QDialogButtonBox::Cancel) == button){
        qDebug()<<"cancel press";
    }
}
