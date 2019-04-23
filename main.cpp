#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "dotsmodel.h"
#include "linesmodel.h"
#include "xmlhandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    DotsModel dotsModel;
    LinesModel linesModel;
    XmlHandler xmlHandler(&dotsModel, &linesModel);

    engine.rootContext()->setContextProperty("dotsModel", &dotsModel);
    engine.rootContext()->setContextProperty("linesModel", &linesModel);
    engine.rootContext()->setContextProperty("xmlHandler", &xmlHandler);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
