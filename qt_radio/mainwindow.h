#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QComboBox>
#include <QStringList>
#include <QString>
#include <QtSql>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void simulation();
    void addRow(QSqlQuery query, int counter);

    void suppress();
    void reset();
private:
    Ui::MainWindow *ui;
    QSqlDatabase db;

    void disableSelection();
};
#endif // MAINWINDOW_H
