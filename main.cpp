#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QDebug>
#include "sunrisesetmodel.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<SunRiseSetModel>("com.avocado", 1, 0, "SunRiseSetModel");

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    //QQuickView view;
    //view.setResizeMode(QQuickView::SizeRootObjectToView);
    //view.setSource(QUrl("qrc:main.qml"));
    //view.show();
    return app.exec();
}
