#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define DEFPATH "/Users/yinyifei/Desktop/test.txt"

#include <QMainWindow>
#include <QVector>

#include "entry.h"

class QLineEdit;
class QTextEdit;
class QListWidget;

enum Mode { DefaultMode = 0, IdentifyMode, SpellMode };

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
    Mode mode;

    // bool monitorInputBoxContent;
    QString inputBoxContent;

    QVector<Entry> wordbank;

    QVector<Entry> wordId, wordSp;
    QVector<Entry>::iterator currentWord;

    QString ListAnEntry(Entry & e) const;
    void AddNewEntry(QString str, bool slientMode = false);

    //
    // Functions associate with logBox
    //
    void AddNewDialog(QString msg);
    void AddNewDialog(QString color, QString msg);
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
    void SaveAs(QString path = DEFPATH);
    void AddFrom(QString path = DEFPATH);
    void OpenFrom(QString path = DEFPATH);


    void Say(QString str);


    void StartPractice(QString arg);
    void NextWord(QString arg);

    QString Mask(QString & str);
};

#endif // MAINWINDOW_H
