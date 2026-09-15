#include <QApplication>

#include "RenderWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    RenderWidget widget;
    widget.show();

    return app.exec();
}
