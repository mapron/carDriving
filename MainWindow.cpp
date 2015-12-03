#include "MainWindow.h"
#include <QBoxLayout>



MainWindow::MainWindow(const QList<float> &patternConfiguration, QWidget *parent)
    : QMainWindow(parent)
{
    const float sceneW = 900;
    const float sceneH = 700;
    m_scene = new RoadGraphicScene(patternConfiguration, sceneW,sceneH,this);
    m_view = new QGraphicsView( this);
    m_view->setScene(m_scene);
    m_view->centerOn(0,0);
    m_splitter = new QSplitter(this);
    QWidget* rightSide = new QWidget(this);
    rightSide->setMinimumWidth(300);
    m_splitter->setOrientation(Qt::Horizontal);
    m_splitter->addWidget(m_view);
    m_splitter->addWidget(rightSide);
    QVBoxLayout * rightSideLayout = new QVBoxLayout(rightSide);
    m_userHelp = new QLabel(tr("Instructions:\n"
                               "<up> - accelerate, <down> - brake\n"
                               "<left>, <right> - turn\n"
                               "<R> - reverse move."),this);
    m_telemetricsInfo = new QLabel(this);
    m_performingTurn = new QLabel(tr("Turn is perfoming."), this);
    m_turnFinished = new QLabel(tr("Turn finished."), this);
    m_oppositeDriving = new QLabel(tr("Rule violation: opposite lane driving."),this);
    m_splitLaneHit = new QLabel(tr("Rule violation: lane split hit."),this);
    m_ditchSlide = new QLabel(tr("You are on ditch. Return to lane."),this);
    rightSideLayout->setSpacing(15);
    rightSideLayout->addWidget(m_userHelp);
    rightSideLayout->addWidget(m_telemetricsInfo);
    rightSideLayout->addWidget(m_performingTurn);
    rightSideLayout->addWidget(m_turnFinished);
    rightSideLayout->addWidget(m_oppositeDriving);
    rightSideLayout->addWidget(m_splitLaneHit);
    rightSideLayout->addWidget(m_ditchSlide);
    rightSideLayout->addStretch();
    m_performingTurn->hide();
    m_turnFinished->hide();
    m_oppositeDriving->hide();
    m_splitLaneHit->hide();
    m_ditchSlide->hide();

    this->setCentralWidget(m_splitter);

    this->resize(1300, 800);


    QSettings analisisSettings("analisys.ini", QSettings::IniFormat);
    m_analysis = new TelemetryAnalysis(&analisisSettings);

    connect(m_scene, SIGNAL(updateTelemetry(CarTelemetry)), this, SLOT(updateTelemetry(CarTelemetry)));

}

MainWindow::~MainWindow()
{
    delete m_analysis;
}


void MainWindow::updateTelemetry(CarTelemetry telemetrics)
{
    static const float pixelsPersecondToKmh = 0.1;
    m_telemetricsInfo->setText(tr("Car telemetrics: \n speed: %1 km/h, x,y: %2, %3 \n angle:%4, side:%5\n split collision:%6")
                               .arg(pixelsPersecondToKmh*telemetrics.m_speed, 0, 'f', 1)
                               .arg(telemetrics.m_pos.x(), 0, 'f', 1)
                               .arg(telemetrics.m_pos.y(), 0, 'f', 1)
                               .arg(telemetrics.m_angle, 0, 'f', 2)
                               .arg(telemetrics.m_sideRatio, 0, 'f', 3)
                               .arg(telemetrics.m_splitCollision, 0, 'f', 3));

    m_analysis->update(telemetrics);
    m_performingTurn->setVisible(m_analysis->isTurn());
    m_turnFinished->setVisible(m_analysis->isTurnFinished());
    m_oppositeDriving->setVisible(m_analysis->isOppositeDriving());
    m_splitLaneHit->setVisible(m_analysis->isSplitLaneHit());
    m_ditchSlide->setVisible(m_analysis->isDitchSlid());
}
