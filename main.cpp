#include "GoalTextEdit.h"
#include <QApplication>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   GoalTextEdit textEdit;
   textEdit.showMaximized();
   return a.exec();
}
