#include "databasesqlite.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringListIterator>
#include <QDebug>
#include <QList>
#include <QListIterator>
#include <QSqlError>
#include <QStringList>

DataBaseSqlite::DataBaseSqlite()
{
    datebase = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "dataSqlite"));
    //datebase->setDatabaseName(":memory:");
    datebase->setDatabaseName("lgySqLite.db");
    if(!datebase->open())
    {
        qDebug()<<"open datebase fail";
        return;
    }
    qDebug()<<"open datebase succ";

    query = new QSqlQuery(*datebase);
}

DataBaseSqlite::~DataBaseSqlite()
{
    delete datebase;
    delete query;
}

int DataBaseSqlite::getAllBankName(QStringList Result)
{
    if(!query->exec("select bank_name from bank_rule;"))
    {
        qDebug()<<"select err";
        return -1;
    }
    while(query->next()){
        Result.append(query->value(0).toString());
    }
    //    foreach (QString str, Result) {
    //        qDebug()<<str;
    //    }

    return 0;
}

QStringList DataBaseSqlite::getAllBankName()
{
    QStringList Result;
    if(!query->exec("select distinct bank_name from bank_rule;"))
    {
        qDebug()<<"select err";

    }
    while(query->next()){
        Result.append(query->value(0).toString());
    }
    return Result;
}

QStringList DataBaseSqlite::getBankNamebyRuleName(QString RuleName)
{
    QStringList Result;
    query->prepare("select distinct bank_name from bank_rule where rule_name = ?;");
    query->addBindValue(RuleName);
    query->exec();
    while(query->next()){
        Result.append(query->value(0).toString());
    }
    return Result;
}

QStringList DataBaseSqlite::getBankNamebyTableHead(QString TableHead)
{
    qDebug()<<"getBankNamebyTableHead"<<TableHead;
    QStringList Result,ResultRuleNo;
    query->prepare("select rule_no from rule_mapping where tbl_head like ?;");
    query->addBindValue(QString("%1%2%3").arg("%").arg(TableHead).arg("%"));
    if(!query->exec()){
        qDebug()<<"eerr:";
        qDebug()<<query->lastError().text();
        return Result;
    }
    while(query->next()){
        ResultRuleNo.append(query->value(0).toString());
    }

    foreach (QString s_ruleno, ResultRuleNo) {
        query->clear();
        query->prepare("select bank_name from bank_rule where rule_no = ? ;");
        query->addBindValue(s_ruleno);
        if(!query->exec()){
            qDebug()<<query->lastError().text();
            return Result;
        }
        while(query->next()){
            Result.append(query->value(0).toString());
        }
    }

    Result.removeDuplicates();
    return Result;
}

QStringList DataBaseSqlite::getRuleNamebyTableHead(QString TableHead)
{
    QStringList Result,ResultRuleNo;
    query->prepare("select rule_no from rule_mapping where tbl_head like ?;");
    query->addBindValue(QString("%1%2%3").arg("%").arg(TableHead).arg("%"));
    query->exec();
    while(query->next()){
        ResultRuleNo.append(query->value(0).toString());
    }
    foreach (QString s_ruleno, ResultRuleNo) {
        query->prepare("select rule_name from bank_rule  where rule_no = ? ;");
        query->addBindValue(s_ruleno);
        query->exec();
        while(query->next()){
            Result.append(query->value(0).toString());
        }
    }
    Result.removeDuplicates();
    return Result;
}

int DataBaseSqlite::getAllRuleName(QStringList Result)
{
    if(!query->exec("select distinct rule_name from bank_rule;"))
    {
        qDebug()<<"select err";
        return -1;
    }
    while(query->next()){
        Result.append(query->value(0).toString());
    }
    return 0;
}

QStringList DataBaseSqlite::getAllRuleName()
{
    QStringList Result;
    if(!query->exec("select rule_name from bank_rule;"))
    {
        qDebug()<<"select err";
    }
    while(query->next()){
        Result.append(query->value(0).toString());
    }
    return Result;
}

QList<QStringList> DataBaseSqlite::getRule_map()
{
    QList<QStringList> Result;
    if(!query->exec("select * from rule_mapping;"))
    {
        qDebug()<<"select err";
    }
    QStringList colums;
    while(query->next()){
        for(int i=0;i<query->size();i++){
            colums.append(query->value(i).toString());
        }
        Result.append(colums);
    }
    return Result;
    //test
    //    QListIterator iterator(Result);
    //    while(iterator.hasNext()){
    //        QStringList s_list = iterator.next();
    //        QListIterator s_iterator(s_list);
    //        while(s_iterator.hasNext()){
    //            QString str = s_iterator.next();
    //            qDebug()<<str;
    //        }
    //    }
}

QStringList DataBaseSqlite::getBankRuleWith(QString s_rule_no)
{
    //QList<QStringList> Result;
    QStringList colums;
    query->prepare("select * from bank_rule where rule_no = ? ; ");
    query->addBindValue(s_rule_no);
    query->exec();
    while(query->next()){
        colums.clear();
        colums.append(query->value(0).toString());
        colums.append(query->value(1).toString());
  //      Result.append(colums);
    }
    return colums;
}

QStringList DataBaseSqlite::getRuleMapWith(QString s_rule_no)
{
    QStringList Result;
    query->prepare("select * from rule_mapping where rule_no = ? ; ");
    query->addBindValue(s_rule_no);
    query->exec();
    while(query->next()){
        Result.append(query->value(1).toString());
        Result.append(query->value(2).toString());
        Result.append(query->value(3).toString());
        Result.append(query->value(4).toString());
        Result.append(query->value(5).toString());
        Result.append(query->value(6).toString());
        Result.append(query->value(7).toString());
    }
    return Result;
}

int DataBaseSqlite::setAnRule(QString s_bank_name, QString s_rule_name, QString s_tbl_head,
                              QString s_fm_str, QString s_data_eg, QString s_filter,
                              QString s_file_name,QString s_pre,QString s_append)
{
    QString  s_maxruleno;
    s_maxruleno = getMaxPlusOneRuleNo();
    qDebug()<<"max_rule_no:"<<s_maxruleno;
    query->prepare("insert into bank_rule values(?,?,?);");
    query->addBindValue(s_bank_name);
    query->addBindValue(s_rule_name);
    query->addBindValue(s_maxruleno);
    if(!query->exec()){
        qDebug()<<"insert bank_rule err"<<query->lastError();
    }
    qDebug()<<"insert bank_rule succ";
    query->prepare("insert into rule_mapping values(?,?,?,?,?,?,?,?);");
    query->addBindValue(s_maxruleno);
    query->addBindValue(s_tbl_head);
    query->addBindValue(s_fm_str);
    query->addBindValue(s_data_eg);
    query->addBindValue(s_filter);
    query->addBindValue(s_file_name);
    query->addBindValue(s_pre);
    query->addBindValue(s_append);
    if(!query->exec()){
        qDebug()<<"insert rule_mapping err"<<query->lastError();
    }
    qDebug()<<"insert rule_mapping succ";
    return 0;
}

QString DataBaseSqlite::getMaxPlusOneRuleNo()
{
    QString ruleno;
    if(!query->exec("select max(cast(rule_no AS INTEGER))+1 from rule_mapping;"))
    {
        qDebug()<<"select err";
    }
    while(query->next()){
        ruleno = query->value(0).toString();
    }
    if(0==ruleno.compare("")){
        ruleno="1";
    }
    return ruleno;
}

QString DataBaseSqlite::getMaxRuleNo()
{
    if(!query->exec("select max(cast(rule_no AS INTEGER)) from rule_mapping;"))
    {
        qDebug()<<"select err";
    }
    while(query->next()){
        return query->value(0).toString();
    }
    return NULL;
}

QSqlQuery *DataBaseSqlite::getSqlQuery()
{
    return query;
}

int DataBaseSqlite::getcountBankRule(QString bank_name,QString rule_name)
{
    query->prepare("select count(*) from bank_rule where bank_name=? and rule_name = ? ;");
    query->addBindValue(bank_name);
    query->addBindValue(rule_name);
    if(!query->exec())
    {
        qDebug()<<"select err";
    }

    return query->value(0).toInt();
}

int DataBaseSqlite::deleteAllhasRuleNo(QString s_rule_no)
{
    query->prepare("delete from bank_rule where rule_no = ? ;");
    query->addBindValue(s_rule_no);
    if(!query->exec()){
        qDebug()<<"delete bank_rule with rule_no err";
    }
    query->prepare("delete from rule_mapping where rule_no = ? ;");
    query->addBindValue(s_rule_no);
    if(!query->exec()){
        qDebug()<<"delete rule_mapping with rule_no err";
    }
    return 0;
}

int DataBaseSqlite::updateRuleMap(QString s_rule_no,QString s_tbl_head, QString s_fm_str, QString s_data_eg, QString s_filter, QString s_file_name,QString s_pre,QString s_append)
{
    query->prepare("update rule_mapping set tbl_head = :tbl_head,fmstr = :fmstr,data_eg = :data_eg,filter = :filter,file_name  = :file_name,pre=:pre,append=:append where rule_no = :rule_no;");

    query->bindValue(":tbl_head",s_tbl_head);
    query->bindValue(":fmstr",s_fm_str);
    query->bindValue(":data_eg",s_data_eg);
    query->bindValue(":filter",s_filter);
    query->bindValue(":file_name",s_file_name);
    query->bindValue(":rule_no",s_rule_no);
    query->bindValue(":pre",s_pre);
    query->bindValue(":append",s_append);
    if(!query->exec()){
        return -1;
        qDebug()<<"update fail!!!";
    }
    qDebug()<<"update succ!!!";
}
