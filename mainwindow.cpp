#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QUrlQuery>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_baseAPI = "";
    connect(ui->btn_getProfiles, &QPushButton::clicked, this, &MainWindow::requestGetProfiles);
    connect(ui->btn_getProfile, &QPushButton::clicked, this, &MainWindow::requestGetProfile);
    connect(ui->btn_updateProfile, &QPushButton::clicked, this, &MainWindow::requestUpdateProfile);
    connect(ui->btn_delProfile, &QPushButton::clicked, this, &MainWindow::requestDeleteProfile);
    connect(ui->btn_createProfile, &QPushButton::clicked, this, &MainWindow::requestCreateProfile);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::requestCreateProfile()
{
    QString profileName = ui->lineEdit_profileName->text();

    networkManager = new QNetworkAccessManager();
    QNetworkRequest postRequest((QUrl(m_baseAPI + "/create")));
    postRequest.setRawHeader(QByteArrayLiteral("Authorization"), "Basic SGVkaW5vcy0xKjE1MmNjOGVkZTQxOWRkNmM4N2U3OGRjMWUwM2IzZmVlZWUwZGI5MWFiYTQ3ZDljMjhjYjBlOWViZTgxMzI2NjNlNmJiZjI5MGY5NThjODZiMjcyMWY5MTUxMGVkZjk1ZGJhODQ2Mjc3YThlMTgyZTk1ZmNkOGQwZTZlNDc4Yjg3OmR1cjIzdA==");
    postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery postData;
    postData.addQueryItem("server_type", "10");
    postData.addQueryItem("name", profileName);
    postData.addQueryItem("group", "2");

    // Подключаем networkManager к обработчику ответа
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onResultForCreateProfile);
    // Получаем данные, а именно JSON файл с сайта по определённому url и параметрам
    networkManager->post(postRequest, postData.toString(QUrl::FullyEncoded).toUtf8());
}

void MainWindow::requestGetProfiles()
{
    ui->textEdit_allProfiles->clear();
    networkManager = new QNetworkAccessManager();
    QNetworkRequest request((QUrl(m_baseAPI)));
    request.setRawHeader(QByteArrayLiteral("Authorization"), "Basic SGVkaW5vcy0xKjE1MmNjOGVkZTQxOWRkNmM4N2U3OGRjMWUwM2IzZmVlZWUwZGI5MWFiYTQ3ZDljMjhjYjBlOWViZTgxMzI2NjNlNmJiZjI5MGY5NThjODZiMjcyMWY5MTUxMGVkZjk1ZGJhODQ2Mjc3YThlMTgyZTk1ZmNkOGQwZTZlNDc4Yjg3OmR1cjIzdA==");
    // Подключаем networkManager к обработчику ответа
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onResultForGetProfiles);
    // Получаем данные, а именно JSON файл с сайта по определённому url
    networkManager->get(request);
}

void MainWindow::requestGetProfile()
{
    networkManager = new QNetworkAccessManager();
    QString tempUrl = m_baseAPI + "/" + ui->lineEdit_profileID->text();
    QNetworkRequest request((QUrl(tempUrl)));
    request.setRawHeader(QByteArrayLiteral("Authorization"), "Basic SGVkaW5vcy0xKjE1MmNjOGVkZTQxOWRkNmM4N2U3OGRjMWUwM2IzZmVlZWUwZGI5MWFiYTQ3ZDljMjhjYjBlOWViZTgxMzI2NjNlNmJiZjI5MGY5NThjODZiMjcyMWY5MTUxMGVkZjk1ZGJhODQ2Mjc3YThlMTgyZTk1ZmNkOGQwZTZlNDc4Yjg3OmR1cjIzdA==");
    // Подключаем networkManager к обработчику ответа
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onResultForGetProfileFromCloud);
    // Получаем данные, а именно JSON файл с сайта по определённому url
    networkManager->get(request);
}

void MainWindow::requestUpdateProfile()
{
    networkManager = new QNetworkAccessManager();
    QString tempUrl = m_baseAPI + "/update/" + ui->lineEdit_profileID->text();
    QNetworkRequest request((QUrl(tempUrl)));
    request.setRawHeader(QByteArrayLiteral("Authorization"), "Basic SGVkaW5vcy0xKjE1MmNjOGVkZTQxOWRkNmM4N2U3OGRjMWUwM2IzZmVlZWUwZGI5MWFiYTQ3ZDljMjhjYjBlOWViZTgxMzI2NjNlNmJiZjI5MGY5NThjODZiMjcyMWY5MTUxMGVkZjk1ZGJhODQ2Mjc3YThlMTgyZTk1ZmNkOGQwZTZlNDc4Yjg3OmR1cjIzdA==");
    // Подключаем networkManager к обработчику ответа
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onResultForCreateProfile);
    // Получаем данные, а именно JSON файл с сайта по определённому url
    networkManager->get(request);
}

void MainWindow::requestDeleteProfile()
{
    networkManager = new QNetworkAccessManager();
    QString tempUrl = m_baseAPI + "/delete/" + ui->lineEdit_profileID->text();
    QNetworkRequest request((QUrl(tempUrl)));
    request.setRawHeader(QByteArrayLiteral("Authorization"), "Basic SGVkaW5vcy0xKjE1MmNjOGVkZTQxOWRkNmM4N2U3OGRjMWUwM2IzZmVlZWUwZGI5MWFiYTQ3ZDljMjhjYjBlOWViZTgxMzI2NjNlNmJiZjI5MGY5NThjODZiMjcyMWY5MTUxMGVkZjk1ZGJhODQ2Mjc3YThlMTgyZTk1ZmNkOGQwZTZlNDc4Yjg3OmR1cjIzdA==");
    // Подключаем networkManager к обработчику ответа
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onResultForDeleteProfile);
    // Получаем данные, а именно JSON файл с сайта по определённому url
    networkManager->get(request);
}

void MainWindow::onResultForDeleteProfile(QNetworkReply *reply)
{
    // Если ошибки отсутсвуют
    if (!reply->error()) {
        // То создаём объект Json Document, считав в него все данные из ответа
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        qDebug() << "Delete profile " << document;
        ui->label_currentProfile->setText("Deleted profile");
    }
    reply->deleteLater();
}

void MainWindow::onResultForCreateProfile(QNetworkReply *reply)
{
    // Если ошибки отсутсвуют
    if (!reply->error()) {
        // То создаём объект Json Document, считав в него все данные из ответа
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonReply = document.object();
        QString profileUrl = jsonReply["url"].toString();

        // записываем профиль во временный файл для отправки на сервер
        QStringList profileFileName = QFileDialog::getOpenFileNames(this, tr("Profile to cloud"), QString(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).constFirst()), tr("Profile file (*.prof)"));
        QByteArray byteArray;
        QFile file = QFile(profileFileName[0]);
        if (!file.open(QIODevice::ReadOnly)) return;
        byteArray = file.readAll();
        //
        networkManager = new QNetworkAccessManager();
        QNetworkRequest postRequest((QUrl(profileUrl)));
        postRequest.setRawHeader(QByteArrayLiteral("Authorization"), "Basic SGVkaW5vcy0xKjE1MmNjOGVkZTQxOWRkNmM4N2U3OGRjMWUwM2IzZmVlZWUwZGI5MWFiYTQ3ZDljMjhjYjBlOWViZTgxMzI2NjNlNmJiZjI5MGY5NThjODZiMjcyMWY5MTUxMGVkZjk1ZGJhODQ2Mjc3YThlMTgyZTk1ZmNkOGQwZTZlNDc4Yjg3OmR1cjIzdA==");


        QHttpMultiPart *multiPart = new QHttpMultiPart (QHttpMultiPart::FormDataType);
        postRequest.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=" + multiPart->boundary());
        QHttpPart profilePart;
        profilePart.setHeader(QNetworkRequest::ContentTypeHeader, "profile/prof");
        profilePart.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; filename=test1.prof; name=\"profile\"");
        QFile *fileProf = new QFile(profileFileName[0]);
        fileProf->open(QIODevice::ReadOnly);
        profilePart.setBodyDevice(fileProf);
        fileProf->setParent(multiPart);

        QHttpPart textPart;
        textPart.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"hash\"");
        textPart.setBody(QCryptographicHash::hash(byteArray, QCryptographicHash::Md5).toHex());

        multiPart->append(textPart);
        multiPart->append(profilePart);

        // Подключаем networkManager к обработчику ответа
        connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onResultForPutProfile);
        // Получаем данные, а именно JSON файл с сайта по определённому url и параметрам
        networkManager->post(postRequest, multiPart);
    }
    reply->deleteLater();
}

void MainWindow::onResultForPutProfile(QNetworkReply *reply)
{
    // Если ошибки отсутсвуют
    if (!reply->error()) {
        // То создаём объект Json Document, считав в него все данные из ответа
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        qDebug() << "Download prof after create " << document;
        ui->label_currentProfile->setText("Put profile in cloud");
    }
    reply->deleteLater();
}

void MainWindow::onResultForGetProfiles(QNetworkReply *reply)
{
    // Если ошибки отсутсвуют
    if (!reply->error()) {
        // То создаём объект Json Document, считав в него все данные из ответа
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonArray jsonArray = document.array();
        QJsonValue jv = jsonArray;
        if (jv.isArray()) {
            // ... то забираем массив из данного свойства
            QJsonArray ja = jv.toArray();
            // Перебирая все элементы массива ...
            for (int i = 0; i < ja.count(); i++) {
                QJsonObject subtree = ja.at(i).toObject();
                ui->textEdit_allProfiles->append(subtree.value("id").toString() +
                                                 " " +
                                                 subtree.value("name").toString());
            }
        }
    }
    reply->deleteLater();
}


void MainWindow::onResultForGetProfileFromCloud(QNetworkReply *reply)
{
    // Если ошибки отсутсвуют
    if (!reply->error()) {
        // То создаём объект Json Document, считав в него все данные из ответа
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonReply = document.object();
        QString profileUrl = jsonReply["url"].toString();
        qDebug() << profileUrl;
        networkManager = new QNetworkAccessManager();
        QNetworkRequest request((QUrl(profileUrl)));
        request.setRawHeader(QByteArrayLiteral("Authorization"), "Basic SGVkaW5vcy0xKjE1MmNjOGVkZTQxOWRkNmM4N2U3OGRjMWUwM2IzZmVlZWUwZGI5MWFiYTQ3ZDljMjhjYjBlOWViZTgxMzI2NjNlNmJiZjI5MGY5NThjODZiMjcyMWY5MTUxMGVkZjk1ZGJhODQ2Mjc3YThlMTgyZTk1ZmNkOGQwZTZlNDc4Yjg3OmR1cjIzdA==");
        request.setHeader(QNetworkRequest::ContentTypeHeader, "X-Accel-Redirect");
        // Подключаем networkManager к обработчику ответа
        connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onResultForDownloadProfile);
        // Получаем данные, а именно JSON файл с сайта по определённому url и параметрам
        networkManager->get(request);
    }
    reply->deleteLater();
}

void MainWindow::onResultForDownloadProfile(QNetworkReply *reply)
{
    // Если ошибки отсутсвуют
    if (!reply->error()) {
        QFile *file = new QFile("C:/test/test1.prof");
        if (file->open(QFile::WriteOnly)) {
            file->write(reply->readAll());
            file->close();
            qDebug() << "Downloading is completed";
            ui->label_currentProfile->setText("Downloading is completed");
        }
    }
    reply->deleteLater();
}
