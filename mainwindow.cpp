#include "mainwindow.h"

#include <QtWidgets>
#include <QFile>
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    display = new QLineEdit;
    logBox = new QTextEdit;
    inputBox = new QLineEdit;
    sideList = new QListWidget;

    QWidget * widget = new QWidget();
    setCentralWidget(widget);

    QGridLayout * mainLayout = new QGridLayout;

    // mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addWidget(display,1,1,2,10);
    mainLayout->addWidget(logBox,3,1,8,10);
    mainLayout->addWidget(inputBox,11,1,1,10);
    mainLayout->addWidget(sideList,1,11,11,8);

    widget->setLayout(mainLayout);

    connect(display, SIGNAL(returnPressed()), this, SLOT(displayReturnPressed()));
    connect(display, SIGNAL(textEdited(QString)), this, SLOT(displayTextEdited()));
    connect(inputBox, SIGNAL(returnPressed()), this, SLOT(commandReceived()));
    connect(inputBox, SIGNAL(textEdited(QString)), this, SLOT(inputBoxTextEdited()));

    //
    // Setting up QLineEdit ***display***
    //
    display->setAlignment(Qt::AlignCenter);
    display->setFont(QFont(".SF NS Text",40,600,false));

    //
    // Setting up QTextEdit ***logBox***
    //
    logBox->setReadOnly(true);
    // logBox->setPlainText("Hello!\n1");
    logBox->setHtml("debugtest!");

    //
    // Setting up QListWidget ***sideList***
    //
    sideListSecretMode = false;

}

MainWindow::~MainWindow()
{
    delete display;
    delete logBox;
    delete inputBox;
    delete sideList;
}

void MainWindow::displayReturnPressed()
{

}

void MainWindow::displayTextEdited()
{

}

void MainWindow::commandReceived()
{
    QString arg = inputBox->text(); // arg: argument
    inputBox->clear();

    AddNewDialog("<font color=\"gray\">" + arg + "</font>");

    QString arg1, arg2;
    for (int i = 0; i < arg.length(); i++)
    {
        if (arg[i] == ' ')
        {
            arg1 = arg.mid(0, i);
            arg2 = arg.mid(i + 1, arg.length() - i - 1);
            break;
        }
        else if (i == arg.length() - 1)
        {
            arg1 = arg;
        }
    }

    // For debug usage: AddNewDialog("arg1: " + arg1 + "<br>arg2: " + arg2);

    if (arg1 == "help")
    {
        // AddNewDialog("<ol><li>clear</li><li>add <i>item</i></li></ol><br>");
        AddNewDialog("<b>This part is still in development.</b>");
    }

    else if (arg1 == "clear" || arg1 == "cls")
    {
        ClearAllDialog();
    }

    else if (arg1 == "add")
    {
        if (arg2 == "")
        { AddNewDialog("<font color=\"red\">Second argument cannot be empty.</font>"); return; }

        AddNewEntry(arg2);
    }

    else if (arg1 == "delete" || arg1 == "del")
    {
        if (arg2 == "")
        { AddNewDialog("<font color=\"red\">Empty second argument.</font>"); return; }

        for (int i = 0; i < wordbank.length(); i++)
        {
            if (wordbank[i].word == arg2)
            {
                wordbank.remove(i);
                UpdateSideList();
                AddNewDialog("<font color=\"blue\">The entry has been successfully removed.</font>");
                return;
            }
        }
        AddNewDialog("<font color=\"red\">The entry "+arg2+" couldn't be found.</font>");
    }

    else if (arg1 == "removeall")
    {
        wordbank.clear();
        UpdateSideList();
        AddNewDialog("<font color=\"blue\">All entries have been successfully removed.</font>");
    }

    else if (arg1 == "list")
    {
        if (wordbank.isEmpty())
        {
            AddNewDialog("<font color=\"red\">The list is currently empty.</font>");
            return;
        }
        QString s;
        s += "<b>List:</b><p style = \"margin:4px\">";
        for (QVector<Entry>::iterator i = wordbank.begin(); i < wordbank.end(); i++)
        {
            s+= ListAnEntry(*i);
            s+="<br>";
        }
        s+="</p>";

        AddNewDialog(s);
    }

    else if (arg1 == "addfrom" || arg1 == "af")
    {
        if (arg2.isEmpty())
            AddFrom();
        else
            AddFrom(arg2);
    }

    else if (arg1 == "save")
    {
        if (arg2.isEmpty())
            SaveAs();
        else
            SaveAs(arg2);
    }

    else if (arg1 == "j76h5gf7j65bv46h5g")
    {

    }

    else if (arg1 == "jkiu54ewdvbnmkiu6")
    {

    }

    else if (arg1 == "edfghjytrdvbny54e")
    {

    }

    else if (arg1 == "rrhgyujhgwwsertyu87")
    {

    }

    else if (arg1 == "")
    {
        AddNewDialog("<font color=\"gray\"><i>You have submitted an empty command.</i></font>");
    }

    else
    {
        AddNewDialog("<font color=\"gray\"><i>Undefined command.</i></font>");
    }
}

void MainWindow::inputBoxTextEdited()
{

}

QString MainWindow::ListAnEntry(Entry & e) const
{
    QString s;
    s += e.word;
    if (!e.remark.isEmpty())
        s += "  #" + e.remark;
    s += "  ";
    switch (e.status)
    {
    case Identify: s+="[Identify]"; break;
    case Spell: s+="[Spell]"; break;
    case Finish: s+="[Finish]"; break;
    case Pending: s+="[Pending]";break;
    // default: s+="[UndefinedStatus!!]";
    }
    s+=e.history;
    return s;
}

void MainWindow::AddNewEntry(QString str, bool slientMode)
{
    while (str.endsWith("\n") || str.endsWith(" "))
        str = str.remove(str.length()-1,1);

    Entry tmp;

    int pos1 = -1;
    int pos2 = -1;
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == '#')
            pos1 = i;
        else if (str[i] == '%')
            pos2 = i;
    }

    if (pos1 == -1 && pos2 == -1)
    {
        tmp.word = str;
    }

    else if (pos1 == -1 && pos2 != -1)
    {
        tmp.word = str.mid(0,(pos2-2)-(0)+1);
        tmp.status = EntryStatus(str.mid(pos2 + 1, 1).toInt());
        if (pos2 + 2 < str.length())
            tmp.history = str.mid(pos2+2,(str.length()-1)-(pos2+2)+1);
    }

    else if (pos1 != -1 && pos2 == -1)
    {
        tmp.word = str.mid(0, pos1 - 1);
        tmp.remark = str.mid(pos1+1, (str.length()-1)-(pos1+1)+1);
    }

    else if (pos1 != -1 && pos2 != -1)
    {
        tmp.word = str.mid(0, pos1 - 1);
        tmp.remark = str.mid(pos1+1, (pos2-2)-(pos1+1)+1);
        tmp.status = EntryStatus(str.mid(pos2 + 1, 1).toInt());
        if (pos2 + 2 < str.length())
            tmp.history = str.mid(pos2+2,(str.length()-1)-(pos2+2)+1);
    }

    bool isAlreadyExist = false;
    for (QVector<Entry>::iterator i = wordbank.begin(); i < wordbank.end(); i++)
    {
        if (i->word == tmp.word)
        {
            isAlreadyExist = true;
            break;
        }
    }
    if (isAlreadyExist)
    {
        AddNewDialog("<font color=\"red\">The following entry couldn't be added "
                     "because an identical one was found.</font>");
        AddNewDialog(ListAnEntry(tmp));
    }
    else
    {
        wordbank.push_back(tmp);
        if (!slientMode)
            AddNewDialog("<font color=\"blue\">The entry has been successfully added.</font>");
        UpdateSideList();
    }
}

void MainWindow::AddNewDialog(QString msg)
{
    logBox->setHtml(logBox->toHtml() + msg);

    logBox->QAbstractScrollArea::verticalScrollBar()->setValue(
                logBox->QAbstractScrollArea::verticalScrollBar()->maximum()
                );
}

void MainWindow::ClearAllDialog()
{
    logBox->setHtml("");
}

void MainWindow::UpdateSideList()
{
    sideList->clear();
    for (int i = 0; i < wordbank.length(); i++)
    {
        QListWidgetItem * item = new QListWidgetItem (wordbank[i].word);
        item->setCheckState(Qt::Unchecked);
        sideList->addItem(item);
    }
}

void MainWindow::SaveAs(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        AddNewDialog("<font color=\"red\">The file could not be opened.</font>");
        return;
    }

    QTextStream out(&file);
    for (QVector<Entry>::iterator i = wordbank.begin(); i < wordbank.end(); i++)
    {
        out << i->word;
        if (!i->remark.isEmpty())
            out << " " << i->remark;
        out << " %" << i->status;
        if (!i->history.isEmpty())
            out << i->history;
        out << "\n";
    }

    out.reset(); // randomly added without any reason
    file.close();
    AddNewDialog("<font color=\"blue\">The file has been successfully saved.</font>");
}

void MainWindow::AddFrom(QString path)
{
    /*
    QFile file("/proc/modules");
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return;

     QTextStream in(&file);
     QString line = in.readLine();
     while (!line.isNull()) {
         process_line(line);
         line = in.readLine();
     }
     */
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        AddNewDialog("<font color=\"red\">The file could not be opened.</font>");
        return;
    }
    while(!file.atEnd())
    {
        QByteArray line = file.readLine();
        QString str(line);
        /* the word #balabala %0fpfpfpfppff
         *
         * a #b %0p
         * -------8
         * 01234567
         *   ^  ^
         */

        AddNewEntry(str, true);
    }
    file.close();
    UpdateSideList();
    AddNewDialog("<font color=\"blue\">The file has been successfully added.</font>");
}

void MainWindow::OpenFrom(QString path)
{
    wordbank.clear();
    AddFrom(path);
}
