#include "MainWindow.h"
#include <QtWidgets/QApplication>

#ifdef Q_OS_WIN
#ifdef QT_NO_DEBUG
#include "BugTrap\BugTrap.h"
#pragma comment(lib, "BugTrap/BugTrap-x64.lib")
#else
// #include "vld.h" // 内存泄漏检测
// #pragma comment(lib, "vld.lib")
#endif // QT_NO_DEBUG
#endif // Q_OS_WIN

int main(int argc, char *argv[])
{
    // 安装BugTrap
#ifdef Q_OS_WIN
#ifdef QT_NO_DEBUG
    BT_SetFlags(BTF_DETAILEDMODE | BTF_ATTACHREPORT);
    BT_InstallSehFilter();
#endif // QT_NO_DEBUG
#endif // Q_OS_WIN
//     // 实例化Application
    QApplication app(argc, argv);
//     //QCoreApplication::setOrganizationName("TianFu");
//     QCoreApplication::setApplicationName(QObject::tr("Fake-CAESES"));
//     QCoreApplication::setApplicationVersion("0.0.1.snapshot");
//     // 启动画面
//     //QSplashScreen splash(QPixmap(CommonUtils::getResourcePath("splashimage.png")));
//     //splash.show();
//     //splash.showMessage(QObject::tr("Starting..."), Qt::AlignHCenter | Qt::AlignBottom, Qt::red);	//显示信息  
//     //app.processEvents();	// 点击隐藏
//     // 安装翻译

    // 实例化主窗口
    MainWindow w;
    //w.setStyleSheet("title:'<div align=\"left\">这里是标题</div>'");
//     w.setWindowTitle(QObject::tr("Fake-CAESES"));
    // 批处理模式时采用
    //app.setQuitOnLastWindowClosed(false);

    w.show();

    // QObject::connect(&app, SIGNAL(lastWindowClosed()), qApp, SLOT(quit()));
    // 结束启动画面
    //splash.finish(&w);
    return app.exec();
}
