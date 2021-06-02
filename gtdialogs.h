#ifndef GTDIALOGS_H
#define GTDIALOGS_H

#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QListWidget>
#include <QTextEdit>

//CNewFileDialog================================================================
class CNewProjectDialog : public QDialog{
    Q_OBJECT
private:
    QLineEdit* m_ptxtProjectPath;
public:
    CNewProjectDialog(QWidget* parent = nullptr)
        : QDialog(parent)
    {
        setWindowTitle("New project");

        m_ptxtProjectPath = new QLineEdit;
        m_ptxtProjectPath->setMinimumWidth(180);

        QLabel* m_plblProjectPath = new QLabel("Project path:");

        QPushButton* pcmdOk = new QPushButton("&Ok");
        QPushButton* pcmdCancel = new QPushButton("&Cancel");

        connect(pcmdOk, SIGNAL(clicked()), SLOT(accept()));
        connect(pcmdCancel, SIGNAL(clicked()), SLOT(reject()));

        //Layouts
        QHBoxLayout* projectPathAreaLayout = new QHBoxLayout;
        projectPathAreaLayout->addWidget(m_plblProjectPath);
        projectPathAreaLayout->addWidget(m_ptxtProjectPath);

        QHBoxLayout* buttonsAreaLayout = new QHBoxLayout;
        buttonsAreaLayout->addWidget(pcmdCancel);
        buttonsAreaLayout->addWidget(pcmdOk);

        QGridLayout* mainLayout = new QGridLayout;
        mainLayout->addLayout(projectPathAreaLayout, 0, 0, 1, 2);
        mainLayout->addLayout(buttonsAreaLayout, 1, 1);

        setLayout(mainLayout);
    }

    QString getProjectPath(){return m_ptxtProjectPath->text();};
};

//CNewFileDialog================================================================
class CNewFileDialog : public QDialog{
    Q_OBJECT
private:
    QLineEdit* m_ptxtFileName;
    QComboBox* m_pcboFilesType;
public:
    CNewFileDialog(QWidget* parent = nullptr)
        : QDialog(parent)
    {
        setWindowTitle("New file");

        m_ptxtFileName = new QLineEdit;

        m_pcboFilesType = new QComboBox;
        m_pcboFilesType->addItem(".cpp");
        m_pcboFilesType->addItem(".h");

        QLabel* m_plblName = new QLabel("Name:");

        QPushButton* pcmdOk = new QPushButton("&Ok");
        QPushButton* pcmdCancel = new QPushButton("&Cancel");

        connect(pcmdOk, SIGNAL(clicked()), SLOT(accept()));
        connect(pcmdCancel, SIGNAL(clicked()), SLOT(reject()));

        //Layouts
        QHBoxLayout* fileNameAreaLayout = new QHBoxLayout;
        fileNameAreaLayout->addWidget(m_ptxtFileName);
        fileNameAreaLayout->addWidget(m_pcboFilesType);

        QHBoxLayout* buttonsAreaLayout = new QHBoxLayout;
        buttonsAreaLayout->addWidget(pcmdCancel);
        buttonsAreaLayout->addWidget(pcmdOk);

        QGridLayout* mainLayout = new QGridLayout;
        mainLayout->addWidget(m_plblName, 0, 0);
        mainLayout->addLayout(fileNameAreaLayout, 1, 0, 1, 2);
        mainLayout->addLayout(buttonsAreaLayout, 2, 1);

        setLayout(mainLayout);
    }

    ~CNewFileDialog(){}

    QString getNameOfNewFile(){
        return (m_ptxtFileName->text() + m_pcboFilesType->currentText());
    }

public slots:

signals:

};


//UnSavedFilesDialog================================================================
class CUnSavedFilesDialog : public QDialog{
    Q_OBJECT
public:
    enum status{DEFAULT, SAVEALL, DONOTSAVE, CANCEL};
private:
    QListWidget* m_plwgAllFiles;
    status m_status;
public:
    CUnSavedFilesDialog(QVector<QString> namesOfUnSavedFiles, QWidget* parent = nullptr)
        : QDialog(parent)
    {
        m_status = DEFAULT;
        QLabel* plblMainText = new QLabel;
        plblMainText->setText("The following files have unsaved changes:");
        m_plwgAllFiles = new QListWidget;

        int numberOfUnSavedFiles = namesOfUnSavedFiles.size();
        for(int i = 0; i < numberOfUnSavedFiles; i++){
            m_plwgAllFiles->addItem(namesOfUnSavedFiles.at(i));
        }

        QPushButton* pcmdSaveAll = new QPushButton("&Save All");
        QPushButton* pcmdDoNotSave = new QPushButton("&Do not save");
        QPushButton* pcmdCancel = new QPushButton("Cancel");
        connect(pcmdSaveAll, SIGNAL(clicked()), SLOT(setSaveAllSlot()));
        connect(pcmdDoNotSave, SIGNAL(clicked()), SLOT(setDoNotSaveSlot()));
        connect(pcmdCancel, SIGNAL(clicked()), SLOT(setCancelSlot()));

        QHBoxLayout* buttonsLayout = new QHBoxLayout;
        buttonsLayout->addWidget(pcmdSaveAll);
        buttonsLayout->addWidget(pcmdDoNotSave);
        buttonsLayout->addWidget(pcmdCancel);

        QVBoxLayout* mainLayout = new QVBoxLayout;
        mainLayout->addWidget(plblMainText);
        mainLayout->addWidget(m_plwgAllFiles);
        mainLayout->addLayout(buttonsLayout);

        setLayout(mainLayout);
    }

    status getStatus(){return m_status;}

public slots:
    void setSaveAllSlot(){
        m_status = SAVEALL;
        close();
    }

    void setDoNotSaveSlot(){
        m_status = DONOTSAVE;
        close();
    }

    void setCancelSlot(){
        m_status = CANCEL;
        close();
    }

signals:

};

//compileErrors
class CCompileErrorsDialog : public QDialog{
    Q_OBJECT
private:
    QTextEdit* m_pErrorsField;
public:
    CCompileErrorsDialog(QByteArray errors, QWidget* parent = nullptr)
        : QDialog(parent)
    {
        QLabel* mainText = new QLabel;
        mainText->setText("Several errors were found during compilation:");

        m_pErrorsField = new QTextEdit;
        m_pErrorsField->setText(errors.data());

        QPushButton* cmdOk = new QPushButton("&Ok");
        connect(cmdOk, SIGNAL(clicked()), SLOT(accept()));

        QGridLayout* mainLayout = new QGridLayout;
        mainLayout->addWidget(mainText, 0, 0);
        mainLayout->addWidget(m_pErrorsField, 1, 0, 1, 2);
        mainLayout->addWidget(cmdOk, 2, 1);
        setLayout(mainLayout);
    }
    ~CCompileErrorsDialog(){

    }

public slots:
signals:
};

class CAboutGoalTextEditorDialog : public QDialog
{
public:
    CAboutGoalTextEditorDialog(QWidget* parent = nullptr)
        : QDialog(parent)
    {
        QLabel* name = new QLabel("GOALtext Editor");
        QLabel* version = new QLabel("version: 0.1");
        QLabel* copyrights = new QLabel("Copyright 2021");
        QPushButton* cmdClose  = new QPushButton("&Close");
        cmdClose->setMinimumWidth(160);

        connect(cmdClose, SIGNAL(clicked()), SLOT(accept()));

        QVBoxLayout* mainLayout = new QVBoxLayout;
        mainLayout->addWidget(name);
        mainLayout->addWidget(version);
        mainLayout->addWidget(copyrights);
        mainLayout->addWidget(cmdClose);
        setLayout(mainLayout);
    }
    ~CAboutGoalTextEditorDialog(){

    }
};



#endif // GTDIALOGS_H
