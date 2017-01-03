#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog_add_rule.h"
#include "dialog_modi_rule.h"
#include <QFileDialog>
#include <QDebug>
#include <QMutex>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QSqlRecord>
#include <QStringList>
#include <QTableWidgetItem>
#include <QTextBlock>
#include <QTextDocument>
#include "databasesqlite.h"
#include <QMessageBox>
#include <uidata.h>
#include <QVBoxLayout>
#include <QDateTime>
#include <QProcess>
#include <QProgressDialog>
#include <QDesktopServices>
#include <QLabel>
#include <QAction>
#include <QMenu>
#include <QSettings>


MainWindow::MainWindow(DataBaseSqlite *db,UiData *uidata,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qDebug()<<"MainWindow init begin";
    ui->setupUi(this);
    //使Ui可自适应父窗口大小
    //    QVBoxLayout* mainLayout = new QVBoxLayout;
    //    mainLayout->addWidget(ui->splitter_3);
    setCentralWidget(ui->splitter_3);
    ui->lineEdit_savePath->setText("D:/GeneralSql_data");
    this->uidata=uidata;
    //    uidata->ListBank.setText();
    //
    ui->tableWidget_mapping->setColumnCount(8);
    ui->tableWidget_mapping->setColumnWidth(2,50);
    ui->tableWidget_mapping->setColumnWidth(3,50);
    //ui->tableWidget_mapping->setHorizontalHeaderLabels();

    QStringList verticalheaders;
    verticalheaders << "fileName" << "RuleNo" << "mod"<<"del"<<"模板脚本"<<"filter"<<"PreTex"<<"AppendTxt";
    ui->tableWidget_mapping->setHorizontalHeaderLabels(verticalheaders);
//    QTableWidgetItem *item = new QTableWidgetItem;
//    QString txt = QString("(%1,%2)").arg(12).arg(56);
//    item->setText(txt);
//    setVerticalHeaderItem(2,item); //这里2指行数，即第三行的标题就是item值（12,56）

    //
    //    datebase = new db(this);
    //打开数据库，不存在时候，创建新的数据库
    //    datebase = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "dataSqlite"));
    //    //datebase->setDatabaseName(":memory:");
    //    datebase->setDatabaseName("lgySqLite.db");
    //    if(!datebase->open())
    //    {
    //        qDebug()<<"open datebase fail";
    //        return;
    //    }
    //    qDebug()<<"open datebase succ";
    //    query = new QSqlQuery(*datebase);

    this->datebase=db;
    query = db->getSqlQuery();
    query->exec("select distinct rule_name from bank_rule limit 0,13;");
    while(query->next())
    {
        qDebug()<<query->value(0).toString();
        ui->listWidget_rule->addItem(query->value(0).toString());

    }

    query->prepare("select distinct bank_name from bank_rule ;");
    query->exec();
    while(query->next())
    {
        qDebug()<<query->value(0).toString()<<"column count:"<<query->record().count();
        ui->listWidget_bank->addItem(query->value(0).toString());
    }

    qDebug()<<"has focus"<<ui->listWidget_rule->currentRow();
    qDebug()<<"MainWindow init end";

    //状态栏
    LabelstatusBar = new QLabel("OK");
    statusBar()->addWidget(LabelstatusBar,1);
    //菜单
    aboutAction = new QAction("Abount Qt",this);
    aboutAction->setToolTip("About Qt");
    connect(aboutAction,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
    aboutMenu = menuBar()->addMenu("About");
    aboutMenu->addAction(aboutAction);
    //读取设置
    QSettings settings("LGY","GeneralSql");
    restoreGeometry(settings.value("geometry").toByteArray());
   // settings.setValue("geometry",saveGeometry());
}

MainWindow::~MainWindow()
{
    delete ui;
    delete datebase;
    delete query;
}

void MainWindow::on_pushButton_openPath_clicked()
{
    //    QFileDialog fd;
    //    QString sopenPath = fd.getExistingDirectory(this,"get openfilePath","",QFileDialog::ShowDirsOnly);
    //    ui->lineEdit_openPath->setText(sopenPath);
    QDesktopServices::openUrl(QUrl(ui->lineEdit_savePath->text(), QUrl::TolerantMode));
}

void MainWindow::on_pushButton_savePath_clicked()
{
    QFileDialog fd;
    QString sopenPath = fd.getExistingDirectory(this,"get savefilePath","",QFileDialog::ShowDirsOnly);
    ui->lineEdit_savePath->setText(sopenPath);
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    //从数据库中筛选
    qDebug()<<arg1;

    //    if(0 > ui->listWidget_bank->currentRow()) {
    //        qDebug()<<"return";
    //        return;
    //    }

    ui->listWidget_rule->clear();
    //    query->prepare("select distinct rule_name from  bank_rule where rule_name like ? and bank_name like ? ;");
    //    query->addBindValue(QString("%1%2%3").arg("%").arg(arg1).arg("%"));
    //    query->addBindValue(QString("%1%2%3").arg("%").arg(ui->listWidget_bank->currentItem()->text()).arg("%"));
    query->prepare("select distinct rule_name from  bank_rule where rule_name like ?  ;");
    query->addBindValue(QString("%1%2%3").arg("%").arg(arg1).arg("%"));
    query->exec();
    while(query->next())
    {
        qDebug()<<query->value(0).toString();
        ui->listWidget_rule->addItem(query->value(0).toString());
    }

}

void MainWindow::on_listWidget_rule_itemPressed(QListWidgetItem *item)
{
    qDebug()<<item->text();

}

void MainWindow::btn_modi_click()
{
    qDebug()<<"btn_click_modi";
    QPushButton *btn = dynamic_cast<QPushButton *>(QObject::sender());
    QModelIndex index=ui->tableWidget_mapping->indexAt(btn->pos());
    QString s_rule_no = ui->tableWidget_mapping->item(index.row(),1)->text();
    qDebug()<<"s_rule_no btn_mod_click"<<s_rule_no;
    Dialog_modi_rule d_modi(datebase,s_rule_no,this);
    d_modi.exec();
    tbl_map_widget_AddRowByRuleNO(datebase->getMaxRuleNo());

}

void MainWindow::btn_delet_click()
{

    //定位button
    QPushButton *button = dynamic_cast<QPushButton *>(QObject::sender()); //找到信号发送者
    QModelIndex index = ui->tableWidget_mapping->indexAt(button->pos()); //定位按钮
    //QTableWidgetItem *item = ui->tableWidget->item(index.row(), 0);　　//得到item
    qDebug()<<"btn_click_delet"<<index.row()<<ui->tableWidget_mapping->item(index.row(),1)->text();
    QMessageBox msgBox(QMessageBox::Warning,QString(tr("提示")),QString(tr(" \n确定删除？\n")));
    msgBox.setStandardButtons(QMessageBox::Yes| QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret=msgBox.exec();
    if(ret==QMessageBox::Yes)
    {
        datebase->deleteAllhasRuleNo(ui->tableWidget_mapping->item(index.row(),1)->text());
        ui->tableWidget_mapping->removeRow(index.row());
    }
    else if(ret==QMessageBox::No)
    {

    }
    updateUI();
}

void MainWindow::on_tableWidget_mapping_cellClicked(int row, int column)
{
    qDebug()<<"btn_click"<<row<<column;
    QString s_Rule_no = ui->tableWidget_mapping->item(row,1)->text();
    LabelstatusBar->setText(ui->tableWidget_mapping->item(row,column)->text());
    //clear
    //    qDebug()<<s_Rule_no;
    ui->tableWidget_sourse->clear();
    int irowcnt = ui->tableWidget_sourse->rowCount();
    for(int i=irowcnt-1;i>=0;i--){
        ui->tableWidget_sourse->removeRow(i);
    }

    //更新 tableWidget_sourse
    query->prepare("select * from rule_mapping where  rule_no =?  ;");
    query->addBindValue(s_Rule_no);
    query->exec();
    while(query->next())
    {
        //fill table head
        ui->tableWidget_sourse->insertRow(ui->tableWidget_sourse->rowCount());
        QString str = query->value(1).toString();
        QStringList strlist = str.split("\t");
        ui->tableWidget_sourse->setColumnCount(strlist.size());
        for(int i=0;i<strlist.size();i++){
            ui->tableWidget_sourse->setItem(ui->tableWidget_sourse->rowCount()-1,i,new QTableWidgetItem(strlist[i]));
        }
        //fill table body
        ui->tableWidget_sourse->insertRow(ui->tableWidget_sourse->rowCount());
        str = query->value(3).toString();
        strlist = str.split("\t");
        ui->tableWidget_sourse->setColumnCount(strlist.size());
        for(int i=0;i<strlist.size();i++){
            //            qDebug()<<"list:"<<strlist[i];
            ui->tableWidget_sourse->setItem(ui->tableWidget_sourse->rowCount()-1,i,new QTableWidgetItem(strlist[i]));
        }
        //fill filterstr
        filterstr = query->value(4).toString();
        //        qDebug()<<"filterstr::"<<filterstr;
    }


}



void MainWindow::on_pushButton_add_map_clicked()
{
    qDebug()<<"add_map"<<uidata->listbank->getText();
    RefreshUidate();
    Dialog_add_rule d_addrule(this->datebase,uidata->listbank->getText(),
                              uidata->listrule->getText());
    d_addrule.exec();
    tbl_map_widget_AddRowByRuleNO(datebase->getMaxRuleNo());

    //updateUIbyRuleNo( datebase->getMaxRuleNo());
    //    d_addrule.raise();
    //add row

    //    ui->tableWidget_mapping->insertRow(ui->tableWidget_mapping->rowCount());
    //    ui->tableWidget_mapping->setItem(ui->tableWidget_mapping->rowCount()-1,0,new QTableWidgetItem("this is test"));
    //    ui->tableWidget_mapping->setCellWidget(ui->tableWidget_mapping->rowCount()-1,2,new QPushButton("modi"));
    //    ui->tableWidget_mapping->setCellWidget(ui->tableWidget_mapping->rowCount()-1,3,new QPushButton("delet"));
    //    //add slots
    //    QPushButton *btn_tmp = (QPushButton *)ui->tableWidget_mapping->cellWidget(ui->tableWidget_mapping->rowCount()-1,2);
    //    connect(btn_tmp,SIGNAL(clicked()),this,SLOT(btn_modi_click()));
    //    btn_tmp = (QPushButton *)ui->tableWidget_mapping->cellWidget(ui->tableWidget_mapping->rowCount()-1,3);
    //    connect(btn_tmp,SIGNAL(clicked()),this,SLOT(btn_delet_click()));
}

void MainWindow::on_lineEdit_openPath_textChanged(const QString &arg1)
{
    qDebug()<<"line_Edit_openPath_textChanged:"<<arg1;
}

void MainWindow::on_lineEdit_bank_textChanged(const QString &arg1)
{
        qDebug()<<"lineEdit_bank_textChanged:"<<arg1;

        ui->listWidget_bank->clear();
        for(int i=0;i<ui->listWidget_bank->count();i++){
            ui->listWidget_bank->takeItem(0);
        }
        query->prepare("select distinct bank_name from bank_rule where bank_name like ? ;");
        query->addBindValue(QString("%1%2%3").arg("%").arg(arg1).arg("%"));
        query->exec();
        while(query->next())
        {
            qDebug()<<query->value(0).toString();
            ui->listWidget_bank->addItem(query->value(0).toString());
        }
}
void MainWindow::on_listWidget_bank_clicked(const QModelIndex &index)
{

}

void MainWindow::on_listWidget_bank_itemClicked(QListWidgetItem *item)
{
    qDebug()<<"listWidget_bank"<<item->text();

    ui->listWidget_rule->clear();
    int listItemCnt = ui->listWidget_rule->count();
    for(int i=0;i<listItemCnt;i++){
        ui->listWidget_rule->takeItem(0);
    }

    query->prepare("select distinct rule_name from bank_rule where  bank_name like ? ;");
    query->addBindValue(QString("%1%2%3").arg("%").arg(item->text()).arg("%"));
    query->exec();
    while(query->next())
    {
        qDebug()<<query->value(0).toString();
        ui->listWidget_rule->addItem(query->value(0).toString());
    }
}

void MainWindow::on_listWidget_rule_itemClicked(QListWidgetItem *item)
{
    //clear
    qDebug()<<"listWidget_rule:"<<item->text();
    if(ui->listWidget_bank->currentItem() == NULL){
        //QMessageBox::warning(this,tr("WARNING"),"请点击分行！！",QMessageBox::Yes);
        //return;
    }

    if(ui->listWidget_bank->currentItem() == NULL){
        ui->listWidget_bank->clear();
        int listItemCnt = ui->listWidget_bank->count();
        for(int i=0;i<listItemCnt;i++){
            ui->listWidget_bank->takeItem(0);
        }
        ui->listWidget_bank->addItems(datebase->getBankNamebyRuleName(ui->listWidget_rule->currentItem()->text()));
        return;
    }

    ui->tableWidget_sourse->clear();
    int irowcnt = ui->tableWidget_sourse->rowCount();
    for(int i=irowcnt-1;i>=0;i--){
        ui->tableWidget_sourse->removeRow(i);
    }
    ui->tableWidget_mapping->clear();
    irowcnt = ui->tableWidget_mapping->rowCount();
    qDebug()<<"mapping_rowcount:"<<irowcnt;
    for(int i=irowcnt-1;i>=0;i--){
        ui->tableWidget_mapping->removeRow(i);
    }
    QStringList verticalheaders;
    verticalheaders << "fileName" << "RuleNo" << "mod"<<"del"<<"模板脚本"<<"filter"<<"PreTex"<<"AppendTxt";
    ui->tableWidget_mapping->setHorizontalHeaderLabels(verticalheaders);
    //更新 bank list widget

    //更新 tableWidget_sourse
    //    ui->tableWidget_sourse->insertRow(ui->tableWidget_sourse->rowCount()+1);
    query->prepare("select * from rule_mapping where  \
                   rule_no in ( \
                       select rule_no from bank_rule \
                       where bank_name=? and rule_name=? \
                ) ;");
            query->addBindValue(ui->listWidget_bank->currentItem()->text());
    query->addBindValue(item->text());
    query->exec();
    while(query->next())
    {
        QString str = query->value(5).toString();
        //        on_pushButton_add_map_clicked();
        //add row
        ui->tableWidget_mapping->insertRow(ui->tableWidget_mapping->rowCount());
        ui->tableWidget_mapping->setItem(ui->tableWidget_mapping->rowCount()-1,0,new QTableWidgetItem("this is test"));
        ui->tableWidget_mapping->setCellWidget(ui->tableWidget_mapping->rowCount()-1,2,new QPushButton("modi"));
        ui->tableWidget_mapping->setCellWidget(ui->tableWidget_mapping->rowCount()-1,3,new QPushButton("delet"));
        //add slots
        QPushButton *btn_tmp = (QPushButton *)ui->tableWidget_mapping->cellWidget(ui->tableWidget_mapping->rowCount()-1,2);
        connect(btn_tmp,SIGNAL(clicked()),this,SLOT(btn_modi_click()));
        btn_tmp = (QPushButton *)ui->tableWidget_mapping->cellWidget(ui->tableWidget_mapping->rowCount()-1,3);
        connect(btn_tmp,SIGNAL(clicked()),this,SLOT(btn_delet_click()));

        ui->tableWidget_mapping->rowCount();
        ui->tableWidget_mapping->item(ui->tableWidget_mapping->rowCount()-1,0)->setText(str);
        str = query->value(0).toString();
        ui->tableWidget_mapping->setItem(ui->tableWidget_mapping->rowCount()-1,1,new QTableWidgetItem(str));
        ui->tableWidget_mapping->setItem(ui->tableWidget_mapping->rowCount()-1,4,new QTableWidgetItem(query->value(2).toString()));
        ui->tableWidget_mapping->setItem(ui->tableWidget_mapping->rowCount()-1,5,new QTableWidgetItem(query->value(4).toString()));
        ui->tableWidget_mapping->setItem(ui->tableWidget_mapping->rowCount()-1,6,new QTableWidgetItem(query->value(6).toString()));
        ui->tableWidget_mapping->setItem(ui->tableWidget_mapping->rowCount()-1,7,new QTableWidgetItem(query->value(7).toString()));
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    //开始生成sql语句
    //ui->plainTextEdit_in
    if(ui->tableWidget_mapping->currentItem()==  NULL ){
        QMessageBox::warning(this,"WARNIG","请选择映射规则",QMessageBox::Yes);
        return;
    }
    if(ui->tableWidget_mapping->currentItem() == NULL ){
        QMessageBox::warning(this,"WARNIG","请选择映射规则",QMessageBox::Yes);
        return;
    }
    QTextDocument * document = 0;
    QTextBlock	textBlock;
    QString	executeCommand,str,s_formatSql,s_Sql,s_pre,s_append;
    QStringList s_list;
    int s_Sqlcnt=0;
    int progresscnt;
    qDebug()<<"general sql begin  1 -------";
    document = ui->plainTextEdit_in->document();
    s_formatSql = ui->tableWidget_mapping->item(ui->tableWidget_mapping->currentRow(),4)->text();
    s_pre = ui->tableWidget_mapping->item(ui->tableWidget_mapping->currentRow(),6)->text();
    s_append = ui->tableWidget_mapping->item(ui->tableWidget_mapping->currentRow(),7)->text();
    QString file_name = ui->tableWidget_mapping->item(ui->tableWidget_mapping->currentRow(),0)->text();
    qDebug()<<"general sql begin  2 -------"<<file_name;
    if(ui->lineEdit_bank_2->text().compare("")!=0){
        file_name.replace("FH",ui->lineEdit_bank_2->text());
    }
    if(ui->lineEdit_mod__date->text().compare("")!=0){
        file_name.replace("YYYYMMDD",ui->lineEdit_mod__date->text());
    }
    if(ui->lineEdit_effect_date->text().compare("")!=0){
        s_formatSql.replace("YYYYMMDDMOD",ui->lineEdit_mod__date->text());
        s_formatSql.replace("YYYYMMDD",ui->lineEdit_effect_date->text());
    }

    QDir *dir = new QDir();
    if(!dir->exists(ui->lineEdit_savePath->text())){
        dir->mkpath(ui->lineEdit_savePath->text());
    }

    QFile file(ui->lineEdit_savePath->text()+"/"+file_name);
    QTextStream out;
    if(file.exists()){
        if (!file.open(QIODevice::Append | QIODevice::Text))
            return;
        ui->plainTextEdit_log->appendPlainText(QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss")
                                               +" : "+file_name
                                               +" : file exist ; append text !");
        out.setDevice(&file);
        out <<"\n\n";
    }else{
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        out.setDevice(&file);
        ui->plainTextEdit_log->appendPlainText(QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss")
                                               +" : "+file_name
                                               +" : create file !");
    }

    //pre process
    if(s_pre.compare("")!=0){
        s_pre.replace("YYYYMMDD",ui->lineEdit_mod__date->text());
        out<<s_pre;
    }
    //append process
    if(s_append.compare("")!=0){
        s_append.replace("YYYYMMDD",ui->lineEdit_mod__date->text());
        out<<s_append;
    }
    //formatsql process
    if(s_formatSql.compare("")!=0){
        qDebug()<<"general sql begin--------";
        QApplication::setOverrideCursor(Qt::WaitCursor);
        //--
        QProgressDialog process(this);
        process.setLabelText(tr("processing..."));
        process.setRange(0,document->lineCount());
        process.setModal(true);
        process.setCancelButtonText(tr("cancel"));
        progresscnt = 0;
        process.setValue(progresscnt+1);
        int loopcnt=0;

        for(textBlock = document->begin();
            textBlock != document->end();
            textBlock = textBlock.next() )
        {
            if(loopcnt<500){
                loopcnt++;
                progresscnt++;
            }else{
                process.setValue(progresscnt);
                loopcnt=0;
            }
            if(process.wasCanceled())
                break;
#if 1
            executeCommand = textBlock.text();
            //qDebug()<<"size:"<<executeCommand.size();
            if(2 > executeCommand.size()) continue;
            s_list=executeCommand.split("\t");

            // filter general sql statment
            if(0!=filterstr.compare(""))
            {
                QStringList ft_list = filterstr.split(";");
                for(int j=0;j<ft_list.size();j++)
                {

                    if(2 > ft_list[j].size()) break;
                    QString ft_tmp = ft_list[j];
                    QStringList ft_tmp_list = ft_tmp.split("=");

                    //                foreach (str,ft_tmp_list) qDebug()<<"ft_tmp_list:"<<str;
                    //                qDebug()<<"ft_tmp_list[0].toInt():"<<ft_tmp_list[0].toInt();
                    //                qDebug()<<"ft_tmp_list[1]:"<<ft_tmp_list[1];
                    //                qDebug()<<"s_list[ft_tmp_list[0].toInt()-1]:"<<s_list[ft_tmp_list[0].toInt()-1];
                    if(0 == s_list[ft_tmp_list[0].toInt()-1].compare(ft_tmp_list[1]))
                    {
                        //qDebug()<<"equal";
                    }
                    else
                    {
                        //qDebug()<<"endforloop";
                        goto endforloop;
                    }
                }

            }
            s_Sql=s_formatSql;
            for(int i=0;i<s_list.size();i++)
            {
                str = "%";
                str.append(QString::number(i+1));
                s_Sql.replace(str,s_list[i].simplified());
            }
            ui->plainTextEdit_out->appendPlainText(s_Sql);
            //s_Sql.replace("YYYYMMDD",ui->lineEdit_mod__date->text());
            out << s_Sql<<"\n";
            // 计数 count
            s_Sqlcnt++;
endforloop:;
#endif
            qApp->processEvents();

        }
        QApplication::restoreOverrideCursor();
    }
    file.close();
    ui->plainTextEdit_log->appendPlainText("SUCCESS END!!!  line:["+QString::number(s_Sqlcnt,10)+"]");
}

void MainWindow::updateUI()
{
    ui->listWidget_bank->clear();
    ui->listWidget_rule->clear();
    ui->listWidget_bank->addItems(datebase->getAllBankName());
    ui->listWidget_rule->addItems(datebase->getAllRuleName());

    ui->tableWidget_sourse->clear();
    int irowcnt = ui->tableWidget_sourse->rowCount();
    for(int i=irowcnt-1;i>=0;i--){
        ui->tableWidget_sourse->removeRow(i);
    }
    ui->tableWidget_mapping->clear();
    irowcnt = ui->tableWidget_mapping->rowCount();
    qDebug()<<"mapping_rowcount:"<<irowcnt;
    for(int i=irowcnt-1;i>=0;i--){
        ui->tableWidget_mapping->removeRow(i);
    }
}

void MainWindow::updateUIbyRuleNo(QString s_rule_no)
{
    updateUI();
    tbl_map_widget_AddRowByRuleNO(s_rule_no);
    //        ui->listWidget_bank->clear();
    //        ui->listWidget_rule->clear();
    //        QList<QStringList> Result = datebase->getBankRuleWith(s_rule_no);
    //        foreach (QStringList colums, Result) {
    //                ui->listWidget_rule->addItem(colums[1]);
    //        }
    //        ui->listWidget_bank->addItem(Result[0][0]);

}

void MainWindow::tbl_map_widget_AddRowByRuleNO(QString s_rule_no)
{
    ui->tableWidget_sourse->clear();
    int irowcnt = ui->tableWidget_sourse->rowCount();
    for(int i=irowcnt-1;i>=0;i--){
        ui->tableWidget_sourse->removeRow(i);
    }
    ui->tableWidget_mapping->clear();
    irowcnt = ui->tableWidget_mapping->rowCount();
    qDebug()<<"mapping_rowcount:"<<irowcnt;
    for(int i=irowcnt-1;i>=0;i--){
        ui->tableWidget_mapping->removeRow(i);
    }
    //更新 tableWidget_sourse
    //    ui->tableWidget_sourse->insertRow(ui->tableWidget_sourse->rowCount()+1);
    query->prepare("select * from rule_mapping where   rule_no in ( ? ) ;");
    query->addBindValue(s_rule_no);
    query->exec();
    while(query->next())
    {
        QString str = query->value(5).toString();
        //        on_pushButton_add_map_clicked();
        //add row
        ui->tableWidget_mapping->insertRow(ui->tableWidget_mapping->rowCount());
        ui->tableWidget_mapping->setItem(ui->tableWidget_mapping->rowCount()-1,0,new QTableWidgetItem("this is test"));
        ui->tableWidget_mapping->setCellWidget(ui->tableWidget_mapping->rowCount()-1,2,new QPushButton("modi"));
        ui->tableWidget_mapping->setCellWidget(ui->tableWidget_mapping->rowCount()-1,3,new QPushButton("delet"));
        //add slots
        QPushButton *btn_tmp = (QPushButton *)ui->tableWidget_mapping->cellWidget(ui->tableWidget_mapping->rowCount()-1,2);
        connect(btn_tmp,SIGNAL(clicked()),this,SLOT(btn_modi_click()));
        btn_tmp = (QPushButton *)ui->tableWidget_mapping->cellWidget(ui->tableWidget_mapping->rowCount()-1,3);
        connect(btn_tmp,SIGNAL(clicked()),this,SLOT(btn_delet_click()));

        ui->tableWidget_mapping->rowCount();
        ui->tableWidget_mapping->item(ui->tableWidget_mapping->rowCount()-1,0)->setText(str);
        str = query->value(0).toString();
        ui->tableWidget_mapping->setItem(ui->tableWidget_mapping->rowCount()-1,1,new QTableWidgetItem(str));
        ui->tableWidget_mapping->setItem(ui->tableWidget_mapping->rowCount()-1,4,new QTableWidgetItem(query->value(2).toString()));
        ui->tableWidget_mapping->setItem(ui->tableWidget_mapping->rowCount()-1,5,new QTableWidgetItem(query->value(4).toString()));
    }
}

void MainWindow::RefreshUidate()
{
    if(ui->listWidget_bank->currentItem() == NULL ){
        uidata->listbank->setText("");
    }else{
        uidata->listbank->setText(ui->listWidget_bank->currentItem()->text());
    }
    if(ui->listWidget_rule->currentItem() == NULL ){
        uidata->listrule->setText("");
    }else{
        uidata->listrule->setText(ui->listWidget_rule->currentItem()->text());
    }
}
void MainWindow::closeEvent( QCloseEvent * event )
{
    QSettings settings("LGY","GeneralSql");
    switch( QMessageBox::information( this, tr("关闭窗口"),tr("确定要关闭程序？"),tr("确定"), tr("取消"),0, 1 ) )
    {
    case 0:
        settings.setValue("geometry",saveGeometry());
        event->accept();
        break;
    case 1:
    default:
        event->ignore();
        break;
    }
}


void MainWindow::on_pushButton_clicked()
{
    qDebug()<<"on_pushButton_clicked searth";
    if(ui->lineEdit_searth->text().compare("")==0){}
    QString s_tblHead = ui->lineEdit_searth->text();

    ui->listWidget_rule->clear();
    int listItemCnt = ui->listWidget_rule->count();
    for(int i=0;i<listItemCnt;i++){
        ui->listWidget_rule->takeItem(0);
    }

    ui->listWidget_bank->clear();
    listItemCnt = ui->listWidget_bank->count();
    for(int i=0;i<listItemCnt;i++){
        ui->listWidget_bank->takeItem(0);
    }

    ui->listWidget_bank->addItems(datebase->getBankNamebyTableHead(s_tblHead));
    ui->listWidget_rule->addItems(datebase->getRuleNamebyTableHead(s_tblHead));

}
