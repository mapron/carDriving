#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QSplitter>
#include <QLabel>

#include "RoadGraphicScene.h"
#include "TelemetryAnalysis.h"

/**
 * \brief Главное окно программы.
 *
 * В центре содержит графическую сцену дороги,
 * Справа - отображение предупреждений и сообщений, а также телеметрии.
 *
 * Анализ телеметрии и выдача предупреждений содержится в TelemetryAnalysis,
 * Управление авто и отрисовка - в RoadGraphicScene
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QList<float> &patternConfiguration, QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updateTelemetry(CarTelemetry telemetrics);

private:
    RoadGraphicScene *m_scene;
    QGraphicsView *m_view;
    QSplitter* m_splitter;
    TelemetryAnalysis *m_analysis;

    QLabel *m_userHelp;
    QLabel *m_telemetricsInfo;
    QLabel *m_performingTurn;
    QLabel *m_turnFinished;
    QLabel *m_oppositeDriving;
    QLabel *m_splitLaneHit;
    QLabel *m_ditchSlide;


};

#endif // MAINWINDOW_H
