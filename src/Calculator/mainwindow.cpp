#include <QLabel>
#include <QString>
#include <iostream>
using namespace std;

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "s21_calc_polish.h"

//qint64 count_unary = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->resize(700,580);
    ui->setupUi(this);
    connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(digits()));  // отслеживаем сигнал нажатия и вызываем метод(слот) digit
    connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(digits()));
    connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(digits()));

    connect(ui->pushButton_plus, SIGNAL(clicked()), this, SLOT(binary_operators()));
    connect(ui->pushButton_minus, SIGNAL(clicked()), this, SLOT(binary_operators()));
    connect(ui->pushButton_div, SIGNAL(clicked()), this, SLOT(binary_operators()));
    connect(ui->pushButton_mul, SIGNAL(clicked()), this, SLOT(binary_operators()));
    connect(ui->pushButton_pow, SIGNAL(clicked()), this, SLOT(binary_operators()));

    connect(ui->pushButton_sin, SIGNAL(clicked()), this, SLOT(functions()));
    connect(ui->pushButton_cos, SIGNAL(clicked()), this, SLOT(functions()));
    connect(ui->pushButton_tan, SIGNAL(clicked()), this, SLOT(functions()));
    connect(ui->pushButton_asin, SIGNAL(clicked()), this, SLOT(functions()));
    connect(ui->pushButton_acos, SIGNAL(clicked()), this, SLOT(functions()));
    connect(ui->pushButton_atan, SIGNAL(clicked()), this, SLOT(functions()));
    connect(ui->pushButton_sqrt, SIGNAL(clicked()), this, SLOT(functions()));
    connect(ui->pushButton_log, SIGNAL(clicked()), this, SLOT(functions()));
    connect(ui->pushButton_ln, SIGNAL(clicked()), this, SLOT(functions()));
    connect(ui->pushButton_mod, SIGNAL(clicked()), this, SLOT(functions()));

    connect(ui->pushButton_left_bracket, SIGNAL(clicked()), this, SLOT(operators()));
    connect(ui->pushButton_right_bracket, SIGNAL(clicked()), this, SLOT(operators()));
    connect(ui->pushButton_unary, SIGNAL(clicked()), this, SLOT(binary_operators()));
    ui->graph->addGraph();
    QPalette palette = ui->capitalization_check->palette();
    palette.setColor( QPalette::Base, Qt::blue );
    ui->capitalization_check->setPalette( palette );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::digits()
{
    QPushButton *button = (QPushButton *)sender();
    QString string_all;
    string_all = (ui->result_number->text() + button->text());
    ui->result_number->setText(string_all);
}

void MainWindow::on_pushButton_point_clicked()
{
    ui->result_number->setText(ui->result_number->text() + ".");
}

void MainWindow::binary_operators()
{
    QPushButton *button = (QPushButton *)sender();
    QString string_all;
    if (button->text() == '+' || button->text() == '-' || button->text() == '*' || button->text() == '/' || button->text() == '^') {
        string_all = (ui->result_number->text() + button->text());
        ui->result_number->setText(string_all);
    } else if (button->text() == "+/-") {
        QChar unary = '-';
        string_all = (ui->result_number->text() + unary);
        ui->result_number->setText(string_all);
    }
}

void MainWindow::functions()
{
    QPushButton *button = (QPushButton *)sender();
    QString string_all;
    string_all = (ui->result_number->text() + button->text());
    ui->result_number->setText(string_all);
}


void MainWindow::operators()
{
    QPushButton *button = (QPushButton *)sender();
    QString string_all;
    string_all = (ui->result_number->text() + button->text());
    ui->result_number->setText(string_all);
}

void MainWindow::on_pushButton_AC_clicked()
{
    ui->result_number->setText("");
}

void MainWindow::on_pushButton_x_clicked()
{
    QPushButton *button = (QPushButton *)sender();
    QString string_all;
    string_all = (ui->result_number->text() + button->text());
    ui->result_number->setText(string_all);
}


void MainWindow::on_pushButton_equal_clicked()
{
    QString qstr = ui->result_number->text();
    char str[256];
    char equal[2];
    equal[0] = '=';
    double x_num = 0;
    QByteArray bytes = qstr.toLocal8Bit();
    strcpy(str, bytes.data());
    strncat(str, equal, 1);
    if (ui->result_number->text().contains('x')) {
        x_num = ui->x_calc_value->text().toDouble();
    }
    double res = 0;
    res = startCalc(str, x_num);
    QString new_label = QString::number(res, 'f', 6);
    ui->result_number->setText(new_label);

}


void MainWindow::on_pushButton_make_graph_clicked()
{
    double x1 = std::atof(ui->value_graph_1->text().toLocal8Bit().data());
    double x2 = std::atof(ui->value_graph_2->text().toLocal8Bit().data());
    char str[256];
    QByteArray x_value = ui->x_calc_value->text().toLocal8Bit();
    QString formula = ui->formula->text();
    QByteArray bytes = formula.toLocal8Bit();
    strcpy(str, bytes.data());
    char equal[2];
    equal[0] = '=';
    strncat(str, equal, 1);
    QVector<double> X,Y;
    double x = 0, mx = 0, y = 0, my = 0;
    x = std::atof(ui->axis_x->text().toLocal8Bit().data());
    y = std::atof(ui->axis_y->text().toLocal8Bit().data());
    my = std::atof(ui->axis_my->text().toLocal8Bit().data());
    mx = std::atof(ui->axis_mx->text().toLocal8Bit().data());
    if (!x || !y || !mx || !my) {
        x = x2;
        y = x2;
        mx = x1;
        my = x1;
    }
    while (x1 <= x2) {
       X.push_back(x1);
       double y = startCalc(str, x1);
       Y.push_back(y);
       x1 += 0.1;
    }
    ui->graph->xAxis->setRange(mx, x);
    ui->graph->yAxis->setRange(my, y);

    ui->graph->graph(0)->addData(X,Y);
    ui->graph->graph(0) ->setPen(QPen(QColor(177, 130, 73)));
    ui->graph->replot();

    X.clear();
    Y.clear();

    ui->graph->graph(0)->data()->clear();
}


void MainWindow::on_pushButton_open_credit_clicked()
{
    if (ui->pushButton_open_credit->text() == "open credit/deposit calculator...") {
        this->resize(1162,580);
        ui->pushButton_open_credit->setText("close credit/deposit calculator...");
    } else {
        ui->pushButton_open_credit->setText("open credit/deposit calculator...");
        this->resize(672,580);
    }
}


void MainWindow::on_pushButton_cr_result_clicked()
{
    double amount = std::atof(ui->cr_amount->text().toLocal8Bit().data());
    double rate = std::atof(ui->cr_rate->text().toLocal8Bit().data());
    double term = std::atof(ui->cr_term->text().toLocal8Bit().data());
    double mounth = std::atof(ui->cr_mounth_number->text().toLocal8Bit().data());
    int flag_error = 0;
    if (term > 0 && amount > 0 && rate > 0 && rate <= 100) {
        int type = 1;
        if (ui->box_type_credit->currentIndex() == 1) {
            type = 2;
        }
        if (ui->box_type_credit->currentIndex() == 2) {
            type = 1;
        }
        double mounth_pay = 0, total = 0, overpayment = 0;
        int mounth_num = mounth;
        double res = takeCreditInfo(amount, (int)term, rate, type, &mounth_pay, &total, &overpayment, mounth_num);
        QString m_p = QString::number(mounth_pay, 'f', 6);
        QString t = QString::number(total, 'f', 6);
        QString o = QString::number(overpayment, 'f', 6);
        ui->result_cr_monthly_pay->setText(m_p);
        ui->result_cr_total->setText(t);
        ui->result_cr_overpayment->setText(o);
    } else {
        ui->result_cr_monthly_pay->setText("Check your input");
        ui->result_cr_total->setText("Check your input");
        ui->result_cr_overpayment->setText("Check your input");
    }
}


void MainWindow::on_pushButton_dep_result_clicked()
{
    double amount = std::atof(ui->dep_amount->text().toLocal8Bit().data());
    double rate = std::atof(ui->dep_rate->text().toLocal8Bit().data());
    double term = std::atof(ui->dep_term->text().toLocal8Bit().data());
    double tax = std::atof(ui->dep_tax->text().toLocal8Bit().data());

    char w_str[256];
    QString w_list = ui->dep_withdrawals_list->text();
    QByteArray w = w_list.toLocal8Bit();
    strcpy(w_str, w.data());

    char r_str[256];
    QString r_list = ui->dep_replenishments_list->text();
    QByteArray bytes = r_list.toLocal8Bit();
    strcpy(r_str, bytes.data());

    if (amount > 0 && rate > 0 && rate <= 100 && term > 0 && tax <= 100) {
        int type = 1;
        if (ui->box_dep_period->currentIndex() == 0) {
            type = 1;
        }
        if (ui->box_dep_period->currentIndex() == 1) {
            type = 2;
        }
        if (ui->box_dep_period->currentIndex() == 2) {
            type = 3;
        }
        double percents = 0, tax_amount = 0, summa_end = 0;
        int mounth_num = term;
        int is_capitalization = 0;
        if (ui->capitalization_check->isChecked()) {
            is_capitalization = 1;
        }
        double res = takeDepositInfo(amount, term, rate, tax, type, is_capitalization, r_str, w_str, &percents, &tax_amount, &summa_end);
        if (res == 0) {
            QString per = QString::number(percents, 'f', 2);
            QString t = QString::number(tax_amount, 'f', 2);
            QString s = QString::number(summa_end, 'f', 2);
            ui->result_dep_amount->setText(s);
            ui->result_dep_percents->setText(per);
            ui->result_dep_tax->setText(t);
        }
    }
}

