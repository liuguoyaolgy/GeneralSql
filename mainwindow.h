#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
//#include "ui_mainwindow.h" //hhh
//class db;
class QSqlDatabase;
class QSqlQuery;
class QString;
class UiData;
class QLabel;
class QAction;

namespace Ui {
class MainWindow;
}
#include "databasesqlite.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(DataBaseSqlite *db,UiData *uidata,QWidget *parent = 0);
    ~MainWindow();
//    Ui::MainWindow *ui;

//public slots:
//    QString savaPtah();
//    QString openPath();


public slots:
    void closeEvent(QCloseEvent *event);
private slots:
    void on_pushButton_openPath_clicked();
    void on_pushButton_savePath_clicked();
    void on_lineEdit_textChanged(const QString &arg1);
    void on_listWidget_rule_itemPressed(QListWidgetItem *item);
    void btn_modi_click();
    void btn_delet_click();
    void on_tableWidget_mapping_cellClicked(int row, int column);
    void on_pushButton_add_map_clicked();

    void on_lineEdit_openPath_textChanged(const QString &arg1);

    void on_lineEdit_bank_textChanged(const QString &arg1);

    void on_listWidget_bank_itemClicked(QListWidgetItem *item);

    void on_listWidget_rule_itemClicked(QListWidgetItem *item);

    void on_pushButton_2_clicked();

    void updateUI();
    void updateUIbyRuleNo(QString s_rule_no);
    void tbl_map_widget_AddRowByRuleNO(QString s_rule_no);
    void RefreshUidate();

    void on_listWidget_bank_clicked(const QModelIndex &index);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
//    db *datebase;
    DataBaseSqlite *datebase;
    QSqlQuery *query;
    QString filterstr;
    QString s_rule_no;
    UiData *uidata;
    QLabel *LabelstatusBar;
    QAction *aboutAction;
    QMenu *aboutMenu;
};

#endif // MAINWINDOW_H
