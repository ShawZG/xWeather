#include <QObject>
#include <QTextCodec>
#include <QApplication>
#include "singleapplication.h"
#include "CommonHelper.h"
#include "MainDialog.h"
#include "DarkStyle.h"
#include "AppConfig.h"

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    SingleApplication app(argc, argv,  true, SingleApplication::User, 100 );
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    app.setStyle(new DarkStyle);
    app.setApplicationName("xweather");
    app.setApplicationVersion(AppConfig::getAppVersion());
    app.setOrganizationName("xzg");


#if defined(Q_OS_WIN)
#elif defined(Q_OS_LINUX)
#elif defined(Q_OS_MACOS)
#endif

    CommonHelper::setApplicationFont(":/font/font/yaheisongti.ttf");
    CommonHelper::setStyle(QString(":/dark_style/dark_style/darkstyle.qss"));

    MainDialog w;

    QObject::connect(&app, &SingleApplication::instanceStarted, &w, &MainDialog::raise);

    if (true == app.isPrimary()) {
        w.show();
    } else {
        emit app.instanceStarted();
        app.exit((0));
    }

    return app.exec();
}
