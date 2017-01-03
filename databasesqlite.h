#ifndef DATABASESQLITE_H
#define DATABASESQLITE_H
class QSqlDatabase;
class QSqlQuery;
class QStringList;
//class QList;
#include <QList>

class DataBaseSqlite
{
public:
    DataBaseSqlite();
    ~DataBaseSqlite();
    int  getAllBankName(QStringList Result);
    QStringList getAllBankName();
    QStringList getBankNamebyRuleName(QString RuleName);
    QStringList getBankNamebyTableHead(QString TableHead);
    QStringList getRuleNamebyTableHead(QString TableHead);
    int  getAllRuleName(QStringList Result);
    QStringList  getAllRuleName();
    QList<QStringList>   getRule_map();
    QStringList getBankRuleWith(QString s_rule_no);
    QStringList getRuleMapWith(QString s_rule_no);
    int  setAnRule(QString s_bank_name, QString s_rule_name , QString s_tbl_head, QString s_fm_str,
                   QString s_data_eg, QString s_filter, QString s_file_name, QString s_pre, QString s_append);
    QString  getMaxPlusOneRuleNo();
    QString  getMaxRuleNo();
    QSqlQuery *getSqlQuery();
    int getcountBankRule(QString bank_name, QString rule_name);
    int deleteAllhasRuleNo(QString s_rule_no);
    int updateRuleMap(QString s_rule_no, QString s_tbl_head, QString s_fm_str,
                      QString s_data_eg, QString s_filter, QString s_file_name, QString s_pre, QString s_append);
private:
    QSqlDatabase *datebase;
    QSqlQuery *query;
};

#endif // DATABASESQLITE_H
