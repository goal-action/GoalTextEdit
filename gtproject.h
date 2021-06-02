#ifndef GTPROJECT_H
#define GTPROJECT_H

#include "gtfile.h"

#include <QWidget>
#include <QDir>
#include <QProcess>
#include <QVector>

class GTProject : public QWidget
{
    Q_OBJECT
private:
    QVector<GTFile*>* m_pvProjectFiles;
    GTFile* m_pCurrentFile;
    QString m_sProjectPath;
    QString m_sProjectName;
    QString m_sCompiledProgramPath;

    int m_nTheme;
    enum Theme{LightTheme, DarkTheme};

public:
    explicit GTProject(QString projectPath, int theme, QWidget *parent = nullptr);
    ~GTProject();

    //Геттеры и сеттеры
    QVector<GTFile*>* getProjectFiles();

    GTFile* getCurrentFile();
    void setCurrentFile(GTFile* file);

    QString getProjectPath();
    void setProjectPath(QString path);

    QString getProjectName();
    void setProjectName(QString name);

    QString getCompiledProgramPath();
    void setCompiledProgramPath(QString path);

    //методы
    void findProjectName();
    void addFile(GTFile* newFile);
    void deleteCurrentFile();
    int  countFiles();
    void saveProject();

    //методы для смены темы
    void setLightTheme();
    void setDarkTheme();
    void setLightHighlighter();
    void setDarkHighlighter();

signals:

};

#endif // GTPROJECT_H
