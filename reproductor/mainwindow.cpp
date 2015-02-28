#include "mainwindow.h"
#include <QPicture>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    //Create central widget and set main layout
    wgtMain_ = new QWidget(this);
    lytMain_ = new QGridLayout(wgtMain_);
    wgtMain_->setLayout(lytMain_);
    setCentralWidget(wgtMain_);

    //Initialize widgets
    mediaPlayer_  = new QMediaPlayer(this);
    playerSlider_ = new QSlider(Qt::Horizontal, this);
    videoWidget_  = new QVideoWidget(this);
    volumeSlider_ = new QSlider(Qt::Horizontal, this);
    btnOpen_      = new QToolButton(this);
    btnPlay_      = new QToolButton(this);
    btnPause_     = new QToolButton(this);
    btnStop_      = new QToolButton(this);

    //Setup widwgets
    videoWidget_->setMinimumSize(400, 400);
    mediaPlayer_->setVideoOutput(videoWidget_);
    mediaPlayer_->setVolume(50);
    videoWidget_->setAspectRatioMode(Qt::KeepAspectRatio);
    volumeSlider_->setRange(0, 100);
    volumeSlider_->setSliderPosition(50);

    //Populate grid layout
    lytMain_->addWidget(videoWidget_,  0, 0, 1, 5);
    lytMain_->addWidget(playerSlider_, 1, 0, 1, 5);
    lytMain_->addWidget(btnOpen_,      2, 0, 1, 1);
    lytMain_->addWidget(btnPlay_,      2, 1, 1, 1);
    lytMain_->addWidget(btnPause_,     2, 2, 1, 1);
    lytMain_->addWidget(btnStop_,      2, 3, 1, 1);
    lytMain_->addWidget(volumeSlider_, 2, 4, 1, 1);

    //Buttons icons
    btnOpen_->setIcon(QIcon(QPixmap(":/icons/resources/eject.png")));
    btnPause_->setIcon(QIcon(QPixmap(":/icons/resources/pause.png")));
    btnPlay_->setIcon(QIcon(QPixmap(":/icons/resources/play.png")));
    btnStop_->setIcon(QIcon(QPixmap(":/icons/resources/stop.png")));

    //Inicializamos los menús
    mainMenu_ = new QMenuBar(this);

    mnuArchivo_ = new QMenu(tr("&Archivo"), this);
    mnuVer_     = new QMenu(tr("&Ver"), this);
    mnuAyuda_   = new QMenu(tr("A&yuda"), this);

    mainMenu_->addMenu(mnuArchivo_);
    mainMenu_->addMenu(mnuVer_);
    mainMenu_->addMenu(mnuAyuda_);

    actArchivoAbrir_ = new QAction(tr("&Abrir"), this);
    actFull_         = new QAction(tr("&Pantalla Completa"), this);
    actAyudaAcerca_  = new QAction(tr("&Acerca de"), this);
    actEscPress_     = new QAction(this);

    mnuArchivo_->addAction(actArchivoAbrir_);
    mnuVer_->addAction(actFull_);
    mnuAyuda_->addAction(actAyudaAcerca_);

    setMenuBar(mainMenu_);

    //Dialogo
    dialogo_ = new QDialog(this);
    dialogo_->setModal(true);
    lytDialogo_ = new QHBoxLayout();
    QImage *Img=  new QImage(":/icons/resources/play.png");
    QLabel *imgDisplayLabel = new QLabel;
    imgDisplayLabel->setPixmap(QPixmap::fromImage(*Img));
    lytDialogo_->addWidget(imgDisplayLabel);
    lytDialogo_->addSpacing(1);
    lytDialogo_->addWidget(new QLabel("Practica con un reproductor multimedia", dialogo_));
    dialogo_->setLayout(lytDialogo_);

    //Connections
    connect(btnOpen_,         SIGNAL(pressed()),               this,         SLOT(onOpen()));
    connect(btnPlay_,         SIGNAL(pressed()),               mediaPlayer_, SLOT(play()));
    connect(btnPause_,        SIGNAL(pressed()),               mediaPlayer_, SLOT(pause()));
    connect(btnStop_,         SIGNAL(pressed()),               mediaPlayer_, SLOT(stop()));
    connect(playerSlider_,    SIGNAL(sliderReleased()),        this,         SLOT(onSeek()));
    connect(mediaPlayer_,     SIGNAL(durationChanged(qint64)), this,         SLOT(onDurationChanged(qint64)));
    connect(mediaPlayer_,     SIGNAL(positionChanged(qint64)), this,         SLOT(onPositionChanged(qint64)));
    connect(volumeSlider_,    SIGNAL(sliderMoved(int)),        this,         SLOT(onVolumeChanged(int)));
    connect(actArchivoAbrir_, SIGNAL(triggered()),             this,         SLOT(onOpen()));
    connect(actAyudaAcerca_,  SIGNAL(triggered()),             this,         SLOT(onAcercaDe()));
    connect(actFull_,         SIGNAL(triggered()),             this,         SLOT(onFull()));
    connect(actEscPress_,     SIGNAL(keyPressEvent()),         this,         SLOT(onRestablecer()));

    //Recientes
    QFile archivo("recientes.txt");
    archivo.open(QIODevice::ReadOnly);
    QAction* tmpAction;
    while(!archivo.atEnd()) {
        QString linea = archivo.readLine();
        tmpAction = new QAction(linea.section('/', -1), this);
        tmpAction->setData(linea);
        mnuArchivo_->addAction(tmpAction);

        recientes_.push_back(tmpAction);

        connect(tmpAction, SIGNAL(triggered()), this, SLOT(onRecienteAbrir()));
    }

    archivo.close();
}

MainWindow::~MainWindow()
{

}

void MainWindow::onOpen()
{
    //Show file open dialog
    QString fileName = QFileDialog::getOpenFileName(this,
                                            tr("Abrir archivo"));
    if (fileName != "") {
        mediaPlayer_->setMedia(QUrl::fromLocalFile(fileName));
    }

    //Recientes
    QFile archivo("recientes.txt");
    archivo.open(QIODevice::WriteOnly | QIODevice::Truncate);
    fileName += "\n";
    archivo.writeData(fileName.toStdString().c_str(), fileName.length());
    for(int i = 0; i < recientes_.size(); ++i) {

    }

    archivo.close();
}

void MainWindow::onSeek()
{
    mediaPlayer_->setPosition(playerSlider_->sliderPosition());
}

void MainWindow::onDurationChanged(qint64 duration)
{
    playerSlider_->setRange(0, duration);
}

void MainWindow::onPositionChanged(qint64 position)
{
    playerSlider_->setSliderPosition(position);
}

void MainWindow::onVolumeChanged(int volume)
{
    mediaPlayer_->setVolume(volume);
}

void MainWindow::onAcercaDe()
{
    dialogo_->show();
}

void MainWindow::onFull()
{
    videoWidget_->setFullScreen(true);
}

void MainWindow::onRestablecer()
{
    videoWidget_->setFullScreen(false);
}

void MainWindow::onRecienteAbrir()
{
    QAction* sender = (QAction*)QObject::sender();

    sender->data();
}
