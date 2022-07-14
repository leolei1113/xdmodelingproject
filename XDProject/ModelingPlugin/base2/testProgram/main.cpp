#include "MainWindow.h"
#include <QtWidgets/QApplication>

#ifdef Q_OS_WIN
#ifdef QT_NO_DEBUG
#include "BugTrap\BugTrap.h"
#pragma comment(lib, "BugTrap/BugTrap-x64.lib")
#else
// #include "vld.h" // �ڴ�й©���
// #pragma comment(lib, "vld.lib")
#endif // QT_NO_DEBUG
#endif // Q_OS_WIN

int main(int argc, char *argv[])
{
    // ��װBugTrap
#ifdef Q_OS_WIN
#ifdef QT_NO_DEBUG
    BT_SetFlags(BTF_DETAILEDMODE | BTF_ATTACHREPORT);
    BT_InstallSehFilter();
#endif // QT_NO_DEBUG
#endif // Q_OS_WIN
//     // ʵ����Application
    QApplication app(argc, argv);
//     //QCoreApplication::setOrganizationName("TianFu");
//     QCoreApplication::setApplicationName(QObject::tr("Fake-CAESES"));
//     QCoreApplication::setApplicationVersion("0.0.1.snapshot");
//     // ��������
//     //QSplashScreen splash(QPixmap(CommonUtils::getResourcePath("splashimage.png")));
//     //splash.show();
//     //splash.showMessage(QObject::tr("Starting..."), Qt::AlignHCenter | Qt::AlignBottom, Qt::red);	//��ʾ��Ϣ  
//     //app.processEvents();	// �������
//     // ��װ����

    // ʵ����������
    MainWindow w;
    //w.setStyleSheet("title:'<div align=\"left\">�����Ǳ���</div>'");
//     w.setWindowTitle(QObject::tr("Fake-CAESES"));
    // ������ģʽʱ����
    //app.setQuitOnLastWindowClosed(false);

    w.show();

    // QObject::connect(&app, SIGNAL(lastWindowClosed()), qApp, SLOT(quit()));
    // ������������
    //splash.finish(&w);
    return app.exec();
}
