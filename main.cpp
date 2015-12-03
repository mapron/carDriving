#include "MainWindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QList<float> patternConfiguration;
    patternConfiguration << 500 << 100 << 200<< 150 << 700 << 300;

    QApplication a(argc, argv);
    QTranslator* translator = new QTranslator();
    translator->load(":/driving/translate.qm");
    QApplication::installTranslator(translator);

    MainWindow w(patternConfiguration);
    w.show();

    return a.exec();
}
