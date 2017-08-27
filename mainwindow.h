#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define DEFPATH "/Users/yinyifei/Desktop/test.txt"

#include <QMainWindow>
#include <QVector>

#include "entry.h"

class QLineEdit;
class QTextEdit;
class QListWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void displayReturnPressed();
    void displayTextEdited();

    void commandReceived();
    void inputBoxTextEdited();

private:

    QLineEdit * display;
    QTextEdit * logBox;
    QLineEdit * inputBox;
    QListWidget * sideList;

    bool sideListSecretMode;

    QVector<Entry> wordbank;


    QString ListAnEntry(Entry & e) const;
    void AddNewEntry(QString str, bool slientMode = false);

    //
    // Functions associate with logBox
    //
    void AddNewDialog(QString msg);
    void ClearAllDialog();


    //
    // Functions associate with sideList
    //
    void UpdateSideList();
    inline void SetSideListSecretMode(bool slsm)
    {
        sideListSecretMode = slsm; UpdateSideList();
    }
    inline bool GetSideListSecretMode() const
    {
        return sideListSecretMode;
    }


    //
    // Functions associate with files
    //
    void ExportTo(QString path = DEFPATH) const;
    void AddFrom(QString path = DEFPATH);
    void OpenFrom(QString path = DEFPATH);
};

#endif // MAINWINDOW_H
