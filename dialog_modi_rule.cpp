#include "dialog_modi_rule.h"
#include "ui_dialog_modi_rule.h"
#include "databasesqlite.h"
#include "QMessageBox"
#include <QAbstractButton>
#include <QDialogButtonBox>
#include <QPushButton>

Dialog_modi_rule::Dialog_modi_rule(DataBaseSqlite *db,QString s_rule_no,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_modi_rule)
{
    ui->setupUi(this);
    this->db=db;
    this->s_rule_no=s_rule_no;
    QStringList columns = db->getBankRuleWith(s_rule_no);
    ui->label_bank_name->setText(columns[0]);
    ui->label_rule_name->setText(columns[1]);
    columns.clear();
    columns = db->getRuleMapWith(s_rule_no);
    ui->lineEdit_tbl_head->setText(columns[0]);
    ui->lineEdit_fmstr->setText(columns[1]);
    ui->lineEdit_data_eg->setText(columns[2]);
    ui->lineEdit_filter->setText(columns[3]);
    ui->lineEdit_filename->setText(columns[4]);
    ui->lineEdit_pre->setText(columns[5]);
    ui->lineEdit_append->setText(columns[6]);

    setLayout(ui->formLayout);
}

Dialog_modi_rule::~Dialog_modi_rule()
{
    delete ui;
}

void Dialog_modi_rule::on_buttonBox_clicked(QAbstractButton *button)
{
    if( ui->buttonBox->button(QDialogButtonBox::Ok) == button){
   //     QMessageBox::warning(NULL, "warning", "bank_name,rule_no重复，不能插入", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        QMessageBox msgBox(QMessageBox::Warning,QString(tr("提示")),QString(tr(" \n确定更新？\n")));
        msgBox.setStandardButtons(QMessageBox::Yes| QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret=msgBox.exec();
        if(ret==QMessageBox::Yes)
        {
                db->updateRuleMap(
                            s_rule_no,
                            ui->lineEdit_tbl_head->text(),
                                  ui->lineEdit_fmstr->text(),
                                  ui->lineEdit_data_eg->text(),
                                  ui->lineEdit_filter->text(),
                                  ui->lineEdit_filename->text(),
                                  ui->lineEdit_pre->text(),
                                  ui->lineEdit_append->text()
                                  );
        }
        else if(ret==QMessageBox::No)
        {

        }
    }
    if(ui->buttonBox->button(QDialogButtonBox::Cancel) == button){

    }
}
