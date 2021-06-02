#ifndef GOALTEXTEDIT_H
#define GOALTEXTEDIT_H

#include <QWidget>
#include <QSplitter>
#include <QVBoxLayout>
#include <QGuiApplication>
#include <QScreen>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QApplication>

#include "gtfile.h"
#include "gtproject.h"
#include "gtmainmenu.h"
#include "gtstackedtextedits.h"
#include "gtsidebar.h"
#include "gtstatusbar.h"

#include "gtdialogs.h"

class GoalTextEdit : public QWidget
{
    Q_OBJECT
protected:
    GTProject* m_pProject;
    GTMainMenu* m_pMainMenu;
    GTStackedTextEdits* m_pStackedTextEdits;
    GTSideBar* m_pSideBar;
    GTStatusBar* m_pStatusBar;

    int m_ntheme;

public:
    GoalTextEdit(QWidget *parent = nullptr);
    ~GoalTextEdit();
    enum Theme{LightTheme, DarkTheme};

private:
    void createProject(const QString&);

public slots:
    //Слоты для работы с файловой системой
    void newProjectSlot();
    void openProjectSlot();
    void newFileSlot();
    void deleteFileSlot();
    void closeProjectSlot();
    void saveCurrentFileSlot();
    void saveProjectSlot();
    void newCurrentFileSlot(const QString&);
    void currentFileIsNotSavedSlot();

    //Слот для работы с компиляцией
    void runSlot();

    //Слот для работы с AboutDialog
    void aboutSlot();

    //Слоты для работы с внешним видом
    void setLeftSideBarSlot();
    void setRightSideBarSlot();
    void setLightThemeSlot();
    void setDarkThemeSlot();

    //Слот для выхода из программы
    void exitSlot();

signals:
    void projectOpenedSignal(const QString& projectName, QVector<GTFile*>* allProjectFiles);
    void projectClosedSignal();
    void newFileSignal(GTFile*);
};
#endif // GOALTEXTEDIT_H
