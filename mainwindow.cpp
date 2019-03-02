#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QByteArray>
#include <QMessageBox>

#include "database.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString path = QFileDialog::getExistingDirectory();
    this->grabKeyboard();
    state = "product";
    db = Database(path);
    db.data_init();
    warn = new QMessageBox(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete warn;
}

void MainWindow::on_pushButton_2_clicked()
{
    QString text = ui->lineEdit_2->text();
    Goods goods = db.select_goods_by_code(text);
    text = text + "\n" + goods.get_name();
    ui->textEdit_2->setText(text);
}

void MainWindow::on_pushButton_clicked()
{
    QString text = ui->lineEdit->text();
    Product product = db.select_product_by_code(text);
    text = text + "\n" + product.get_name();
    QStringList goods_list = product.get_goods();
    for(int i=0;i<goods_list.size();i++)
    {
        text = text + "\n" + db.select_goods_by_code(goods_list.at(i)).get_name();
    }
    ui->textEdit->setText(text);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    using namespace std;
    if(event->key() == Qt::Key_Return)
    {
        ui->label->setText(input);
        ui->textEdit_3->setText(input);
        judge(input);
        input.clear();
    }
    else
    {
        input = input + event->key();
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    Product p1 = db.select_product_by_code(ui->lineEdit_2->text());
    QString s1 = p1.get_name();
    QStringList l1 = p1.get_goods();
    Goods goods;

    for(int i=0;i<l1.size();i++)
    {
        goods = db.select_goods_by_code(l1.at(i));
        s1 = s1 + "-" + goods.get_name();
    }
    ui->textEdit->setText(s1);
}

void MainWindow::judge(QString input)
{
    ui->label->setText(state);
    if(state == "product")
    {
        product = db.select_product_by_code(input);
        goods_code_list = product.get_goods();
        QString name = "";
        QString code = "";
        QLabel * label;
        for(int i=0;i<goods_code_list.size();i++)
        {
            code = goods_code_list.at(i);
            name = db.select_goods_by_code(code).get_name();
            label = new QLabel(name);
            label_map.insert(name,label);
            ui->verticalLayout->addWidget(label);
        }

        ui->label_2->setText(QString::number(goods_code_list.size()));
        QString text = db.select_goods_by_code(goods_code_list.at(3)).get_name();
        for(int i=0;i<goods_code_list.size();i++)
        {
            ;
//            text = text + "-" + db.select_goods_by_code(goods_code_list.at(i)).get_name();
        }
        ui->textEdit->setText(text);

        state = "goods";
        return ;
    }
    int index;
    if(state == "goods")
    {
        QString name = "";
        index = goods_code_list.indexOf(input);

        ui->label_2->setText(QString::number(goods_code_list.size()));
        QString text = "";
        for(int i=0;i<goods_code_list.size();i++)
        {
            text = text + "-" + db.select_goods_by_code(goods_code_list.at(i)).get_name();
        }
        ui->textEdit->setText(text);

        if(index != -1)
        {
            name = db.select_goods_by_code(goods_code_list.at(index)).get_name();
            ui->verticalLayout->removeWidget(label_map[name]);
            delete label_map[name];
            goods_code_list.removeAt(index);
            if(goods_code_list.isEmpty())
            {
                state = "confirm";
            }
        }
        else
        {
            warn->setText("物品错误");
            warn->show();
        }
        return ;
    }
    if(state == "confirm")
    {
        ui->lineEdit->setText(input);
        ui->lineEdit_2->setText(db.confirm_code);
        if(input == db.confirm_code)
        {
            state = "product";
            warn->setWindowTitle("成功");
            warn->setText("产品封装完成");
            warn->show();
        }
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    QString warn_title = "警告";
    QString warn_text = "物品过多";
    warn->setText(warn_text);
    warn->setWindowTitle(warn_title);
    warn->show();
}
