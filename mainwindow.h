#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QWidget>

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
    // Обработчики данных полученных от объекта QNetworkAccessManager

    //возвращает список профилей
    void onResultForGetProfiles(QNetworkReply *reply);
    //получает сслылку на профиль и дергает onResultForDownloadProfile
    void onResultForGetProfileFromCloud(QNetworkReply *reply);
    //возвращает урл на сторадж в который нужно положить файл профиля при создании и дергает onResultForPutProfile
    void onResultForCreateProfile(QNetworkReply *reply);
    //загружает файл профиля в сторадж
    void onResultForPutProfile(QNetworkReply *reply);
    //загружает файл профиля из стораджа
    void onResultForDownloadProfile(QNetworkReply *reply);
    //возвращает статус удаления
    void onResultForDeleteProfile(QNetworkReply *reply);
    //запрос на список профилей
    void requestGetProfiles();
    //запрос на урл стораджа откуда скачать конкретный профиль
    void requestGetProfile();
    //запрос на создание профиля
    void requestCreateProfile();
    //запрос на обновление профиля
    void requestUpdateProfile();
    //удаление профиля
    void requestDeleteProfile();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *networkManager;
    QString m_baseAPI; //url api
};
#endif // MAINWINDOW_H
