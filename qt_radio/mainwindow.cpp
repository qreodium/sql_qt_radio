#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->enemyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::suppress()
{
    if(ui->enemyTable->selectionModel()->hasSelection())
    {
        int selected_row = ui->enemyTable->selectionModel()->selectedRows().at(0).row();
        ui->enemyTable->item(selected_row, 0)->setBackground(Qt::green);
        ui->enemyTable->item(selected_row, 1)->setBackground(Qt::green);
        ui->enemyTable->clearSelection();
    }
}

void MainWindow::disableSelection()
{
    int row = ui->enemyTable->rowCount()-1;
    ui->enemyTable->item(row,0)->setFlags(ui->enemyTable->item(row,0)->flags() & (~Qt::ItemIsSelectable));
    ui->enemyTable->item(row,1)->setFlags(ui->enemyTable->item(row,1)->flags() & (~Qt::ItemIsSelectable));
    if(ui->enemyTable->item(row, 0)->background() != Qt::green)
    {
        ui->enemyTable->item(row, 0)->setBackground(Qt::red);
        ui->enemyTable->item(row, 1)->setBackground(Qt::red);
    }

}

void MainWindow::reset()
{
    ui->enemyTable->setRowCount(0);
}

void MainWindow::addRow(QSqlQuery query, int counter)
{
    if(ui->enemyTable->rowCount() > 0)
        disableSelection();
    QRandomGenerator gen =  QRandomGenerator(QDateTime::currentMSecsSinceEpoch());
    query.seek(gen.bounded(9));// ДЛИНУ ИЗ БАЗЫ
    ui->enemyTable->insertRow(ui->enemyTable->rowCount());
    QTableWidgetItem *item_Freq = new QTableWidgetItem(query.value(1).toString());
    QTableWidgetItem *item_modulation;
    QStringList names = {"FM","AM","QAM","MFSK","WDM","OOK"};
    if(query.value(2).toInt() < 6)
        item_modulation = new QTableWidgetItem(names[query.value(2).toInt()]);
    else
        item_modulation = new QTableWidgetItem(names[5]);
    ui->enemyTable->setItem(ui->enemyTable->rowCount()-1,0,item_Freq);
    ui->enemyTable->setItem(ui->enemyTable->rowCount()-1,1,item_modulation);
    counter -= 1;
    if (counter != 0)
        QTimer::singleShot(2000, this, [this, query, counter] {
            addRow(query,counter);
        });
    else
        QTimer::singleShot(2000, this, [this] {
            disableSelection();
        });
}

void MainWindow::simulation()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../database.db");
    db.open();
    if( !db.open() )
            qCritical() << "Error. Database can't open.";
        else
        {
            ui->enemyTable->setRowCount(0);
            QSqlQuery query(db);
            if( !query.exec("SELECT id, frequency, modulation FROM signals") )
            {
                qCritical() << db.lastError().text();
            }
            else
            {
                addRow(query, 5);
            }
    }
}
