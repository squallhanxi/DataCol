#include "widget.h"

#include <QApplication>
#include "loginform.h"

int main(int argc, char *argv[])
{
#if (QT_VERSION >= QT_VERSION_CHECK(5,9,0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication a(argc, argv);
    LoginForm login;
    if (login.exec() == QDialog::Accepted)  //调用login.exec()，阻塞主控制流，直到完成返回，继续执行主控制流
    {
        Widget w;
        w.show();
        return a.exec();
    }
    else return 0;
}
