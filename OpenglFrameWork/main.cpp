#include <QtWidgets/QApplication>
#include"OpenGLWindow.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	OpenGLWindow w;
	w.setFixedWidth(800);
	w.setFixedHeight(800);
	w.show();
	return a.exec();
}