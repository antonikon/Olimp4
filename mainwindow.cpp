#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
 * Пользователь вводит ссылку на playlist в youtube
 *Нужно скачать содержимое плейлиста и сконвертировать его в mp3 файлы
 *
 * 1) Скачать страничку по ссылки
 * 2) Распарсить её, вытащить ссылки на все элементы плейлиста
 * 3) Скачать все найденные ссылки
 * 4) Вытащить из них ссылку на видео (вроде в мп4 формате)
 * 5) Скачать видео
 * 6) Сконвертировать его в mp3 файл
 */


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Loader = new TLoader(this,ui);
    ui->setupUi(this);

    /*
     QNetworkAccessManager* mgr = new QNetworkAccessManager();
    ....
    QNetworkRequest req;
    req.setUrl(QUrl("enter url") );
    req.setRawHeader( "User-Agent" , "Mozilla Firefox" );

    mgr->get(req);
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    Loader->Load(ui->lineEdit->text());
}
