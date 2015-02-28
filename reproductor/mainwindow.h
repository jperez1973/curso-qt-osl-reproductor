#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QDir>
#include <QSlider>
#include <QFileDialog>
#include <QToolButton>
#include <QUdpSocket>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QHBoxLayout>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    QGridLayout*  lytMain_;
    QWidget*      wgtMain_;
    QMediaPlayer* mediaPlayer_;
    QSlider*      playerSlider_;
    QVideoWidget* videoWidget_;
    QSlider*      volumeSlider_;
    QToolButton*  btnOpen_;
    QToolButton*  btnPlay_;
    QToolButton*  btnPause_;
    QToolButton*  btnStop_;
    QMenuBar*     mainMenu_;
    QMenu*        mnuArchivo_;
    QMenu*        mnuVer_;
    QMenu*        mnuAyuda_;
    QAction*      actArchivoAbrir_;
    QAction*      actFull_;
    QAction*      actAyudaAcerca_;
    QAction*      actEscPress_;
    QDialog*      dialogo_;
    QHBoxLayout*  lytDialogo_;
    QList<QAction*> recientes_;

private slots:
    void onOpen();
    void onSeek();
    void onDurationChanged(qint64 duration);
    void onPositionChanged(qint64 position);
    void onVolumeChanged(int volume);
    void onAcercaDe();
    void onFull();
    void onRestablecer();
    void onRecienteAbrir();

};

#endif // MAINWINDOW_H
