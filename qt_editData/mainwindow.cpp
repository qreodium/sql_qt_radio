#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->signalTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    load();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::load()
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../database.db");
    db.open();
    if( !db.open() )
        qCritical() << "Error. Database can't open.";
    else
    {
        ui->signalTable->setRowCount(0);
        QSqlQuery query(db);
        if( !query.exec("SELECT id, frequency, modulation FROM signals") )
        {
            qCritical() << db.lastError().text();
            query.exec("CREATE TABLE \"signals\" ( \"ID\" INTEGER NOT NULL, \"frequency\" REAL, \"modulation\" INTEGER NOT NULL, PRIMARY KEY(\"ID\") )");
        }
        else
        {
            while (query.next())
            {
                ui->signalTable->insertRow(ui->signalTable->rowCount());
                QTableWidgetItem *item_id = new QTableWidgetItem(query.value(0).toString());
                item_id->setFlags(item_id->flags() & (~Qt::ItemIsEditable));
                QTableWidgetItem *item_Freq = new QTableWidgetItem(query.value(1).toString());
                QComboBox* combo = new QComboBox();
                combo->addItems({"FM","AM","QAM","MFSK","WDM","OOK"});
                if(query.value(2).toInt() < 6)
                    combo->setCurrentIndex(query.value(2).toInt());
                else
                    combo->setCurrentIndex(5);
                ui->signalTable->setItem(ui->signalTable->rowCount()-1,0,item_id);
                ui->signalTable->setItem(ui->signalTable->rowCount()-1,1,item_Freq);
                ui->signalTable->setCellWidget(ui->signalTable->rowCount()-1,2,combo);
            }
        }
     }
}

void MainWindow::addRow()
{
    ui->signalTable->insertRow(ui->signalTable->rowCount());
    QTableWidgetItem *item_id = new QTableWidgetItem(QString::number(ui->signalTable->rowCount()));
    item_id->setFlags(item_id->flags() & (~Qt::ItemIsEditable));
    QTableWidgetItem *item_Freq = new QTableWidgetItem(QString(""));
    QComboBox* combo = new QComboBox();
    combo->addItems({"FM","AM","QAM","MFSK","WDM","OOK"});
    ui->signalTable->setItem(ui->signalTable->rowCount()-1,0,item_id);
    ui->signalTable->setItem(ui->signalTable->rowCount()-1,1,item_Freq);
    ui->signalTable->setCellWidget(ui->signalTable->rowCount()-1,2,combo);
}

void MainWindow::deleteRow()
{
    if(ui->signalTable->selectionModel()->hasSelection())
    {
        int selected_row = ui->signalTable->selectionModel()->selectedRows().at(0).row();
        ui->signalTable->removeRow(selected_row);
        for(int i = selected_row; i<ui->signalTable->rowCount(); i++)
        {
            ui->signalTable->item(i,0)->setText(QString::number(ui->signalTable->item(i,0)->text().toInt()-1));
        }
    }
    else
        ui->signalTable->removeRow(ui->signalTable->rowCount());
}

void MainWindow::save()
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../database.db");
    db.open();
    if( !db.open() ) {
        qCritical() << "Error. Database can't open.";
    }
    QSqlQuery query(db);

    query.prepare("DELETE  FROM signals");
    if( !query.exec() ) {
           qDebug() << db.lastError().text();
    }


    const int N = ui->signalTable->rowCount();
    QString queryString = "INSERT INTO signals( ID, frequency, modulation) VALUES ";
    queryString.append( QString( "(?,?,?)," ).repeated( N ) );
    queryString.chop( 1 );
    if( !query.prepare( queryString ) )
    {
        qDebug() << "Unable to prepare query:" << query.lastError();
    }
    for( int i = 0; i < N; ++i )
    {
        query.addBindValue(ui->signalTable->item(i,0)->text().toInt());
        query.addBindValue(ui->signalTable->item(i,1)->text().toDouble());
        QComboBox* combo = (QComboBox*)ui->signalTable->cellWidget(i,2);
        query.addBindValue(combo->currentIndex());
    }
    if( !query.exec() )
    {
        qDebug() << "Unable to exec query:" << query.lastError();
    }
    /*
    query.prepare(
        "INSERT INTO signals( ID, frequency, modulation ) "
        "   VALUES( :ID, :frequency, :modulation )"
    );
    query.bindValue( ":ID", 10 );
    query.bindValue( ":frequency", 228.8 );
    query.bindValue( ":modulation", 4 );
    if( !query.exec() ) {
           qDebug() << db.lastError().text();
    }
    ui->signalTable->item(ui->signalTable->rowCount()-1,1)->setText("SAVE");
    */
}
