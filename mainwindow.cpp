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
    mode = DefaultMode;

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

void MainWindow::commandReceived(QString input)
{
    QString arg;
    if (input == "")
    {
        arg = inputBox->text(); // arg: argument
        inputBox->clear();
        AddNewDialog("gray", arg);
    }
    else
    {
        arg = input;
        AddNewDialog("lightgray", arg);
    }


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

    // For debug usage:
    // AddNewDialog("arg1: " + arg1 + "<br>arg2: " + arg2);

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
        { AddNewDialog("red", "Second argument cannot be empty."); return; }

        AddNewEntry(arg2);
    }

    else if (arg1 == "delete" || arg1 == "del")
    {
        if (arg2 == "")
        { AddNewDialog("red", "Second argument cannot be empty."); return; }

        for (int i = 0; i < wordbank.length(); i++)
        {
            if (wordbank[i].word == arg2)
            {
                wordbank.remove(i);
                UpdateSideList();
                AddNewDialog("blue", "The entry has been successfully removed.");
                return;
            }
        }
        AddNewDialog("red", "The entry "+arg2+" couldn't be found.");
    }

    else if (arg1 == "reset")
    {
        if (arg2 == "wordbank")
        {
            wordbank.clear();
            UpdateSideList();
            AddNewDialog("blue", "All entries have been successfully removed.");
        }
        else if (arg2 == "")
        {
            AddNewDialog("red", "Second argument cannot be empty.");
        }
        else
        {
            AddNewDialog("red", "Second argument is invalid.");
        }
    }

    else if (arg1 == "list")
    {
        if (arg2 == "")
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
        else
        {
            for (QVector<Entry>::iterator i = wordbank.begin(); i < wordbank.end(); i++)
            {
                if (i->word == arg2)
                {
                    ListAnEntry(*i);
                    return;
                }
                AddNewDialog("red", "The entry "+arg2+" couldn't be found.");
            }
        }
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

    else if (arg1 == "start-practice" || arg1 == "start" || arg1 == "sp")
    {
        StartPractice(arg2);
    }

    else if ((arg1 == "yes" || arg1 == "no" || arg1 == "pass" || arg1 == "fail") && mode != DefaultMode)
    {
        NextWord((arg1 == "yes" || arg1 == "y" || arg1 == "pass")? "pass" : "fail" );
    }

    else if ((arg1 == "quit" || arg1 == "q") && mode != DefaultMode)
    {
        currentWord = nullptr;
        mode = DefaultMode;
    }

    else if (arg1 == "rrhgyujhgwwsertyu87")
    {

    }

    else if (arg1 == "")
    {
        AddNewDialog("gray", "<i>You have submitted an empty command.</i>");
    }

    else
    {
        AddNewDialog("gray", "<i>Undefined command.</i>");
    }
}

void MainWindow::inputBoxTextEdited()
{
    if (mode == IdentifyMode)
    {
        commandReceived(inputBox->text());
        inputBox->clear();
    }
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
        AddNewDialog("red", "The following entry couldn't be added "
                     "because an identical one was found.");
        AddNewDialog(ListAnEntry(tmp));
    }
    else
    {
        wordbank.push_back(tmp);
        if (!slientMode)
            AddNewDialog("blue", "The entry has been successfully added.");
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

void MainWindow::AddNewDialog(QString color, QString msg)
{
    AddNewDialog("<font color=\"" + color + "\">" + msg + "</font>");
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

        QListWidgetItem * item = new QListWidgetItem (&Find(currentWord->word) == &wordbank[i]?
                                                          ListAnEntry(wordbank[i]) :
                                                          "> " + ListAnEntry(wordbank[i]));
        item->setCheckState(Qt::Unchecked);
        sideList->addItem(item);
    }
}

void MainWindow::SaveAs(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        AddNewDialog("red", "The file could not be opened.");
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
    AddNewDialog("blue", "The file has been successfully saved.");
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
        AddNewDialog("red", "The file could not be opened.");
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
    AddNewDialog("blue", "The file has been successfully added.");
}

void MainWindow::OpenFrom(QString path)
{
    wordbank.clear();
    AddFrom(path);
}

void MainWindow::Say(QString str)
{
    while (str.endsWith("\n") || str.endsWith(" "))
        str.chop(1);

    if (str.isEmpty())
    {
        AddNewDialog("red", "<i>An empty string has been passed to Say().</i>");
        return;
    }

    QByteArray a = QString("say " + str).toLatin1();
    system(a.data());
}

void MainWindow::StartPractice(QString arg)
{
    if (arg.isEmpty()) // [^]
        arg = "";

    wordId.clear();
    wordSp.clear();

    for (QVector<Entry>::iterator i = wordbank.begin(); i < wordbank.end(); i++)
    {
        if (i->status == Identify)
            wordId.push_back(*i);
        else if (i->status == Spell)
            wordSp.push_back(*i);
    }

    currentWord = wordId.begin();
    if (currentWord != wordId.end())
    {
        mode = IdentifyMode;
        AddNewDialog("Identify the word: " + currentWord->word);
    }
    else
    {
        currentWord = wordSp.begin();
        if (currentWord != wordSp.end())
        {
            mode = SpellMode;
            AddNewDialog("Spell the word: " + Mask(currentWord->word));
        }
        else
        {
            currentWord = nullptr;
            mode = DefaultMode;
            AddNewDialog("blue", "You have learned all words.");
        }
    }
}

void MainWindow::NextWord(QString arg)
{

    if (arg == "pass")
    {
        currentWord->history.push_back('p');
        Find(currentWord->word).history.push_back('p');
        if (currentWord->history.endsWith("ppp")) // [^]
        {
            if (currentWord->status == Identify) // =================== IDENTIFY
            {
                currentWord->status = Spell;
                Find(currentWord->word).status = Spell;
                currentWord->history.clear();
                Find(currentWord->word).history.clear();
                wordSp.push_back(*currentWord);
                wordId.erase(currentWord);
                if (wordId.isEmpty()) // ------------------------------ Switch over to wordSp
                {
                    mode = SpellMode;
                    currentWord = wordSp.begin();
                    if (currentWord == wordSp.end()) // --------------- Switch over and then exit
                    {
                        mode = DefaultMode;
                        AddNewDialog("blue", "You have completed the list.");
                        return;
                    }
                }
                currentWord++;
                if (currentWord == wordId.end()) // ------------------- Restart
                    currentWord = wordId.begin();
            }
            else if (currentWord->status == Spell) // ================= SPELL
            {
                currentWord->status = Finish;
                Find(currentWord->word).status = Finish;
                currentWord->history.clear();
                Find(currentWord->word).history.clear();
                wordSp.erase(currentWord);
                if (wordSp.isEmpty()) //-------------------------------- Exit
                {
                    mode = DefaultMode;
                    AddNewDialog("blue", "You have completed the list.");
                    return;
                }
                currentWord++;
                if (currentWord == wordSp.end()) // ------------------- Restart
                    currentWord = wordSp.begin();
            }
        }
    }
    else if (arg == "fail")
    {
        currentWord->history.push_back('f');
        Find(currentWord->word).history.push_back('f');
        currentWord++;
        if (currentWord == wordSp.end()) // [^]
            currentWord = wordSp.begin();
        if (currentWord == wordId.end())
            currentWord = wordId.begin();
    }
    else
    {

    }

    currentWord++;
    if (mode == IdentifyMode)
    {
        if (currentWord == wordId.end())
        {
            mode = SpellMode;
            currentWord = wordSp.begin();
        }
    }
    else if (mode == SpellMode)
    {
        if (currentWord == wordSp.end())
        {
            mode = DefaultMode;

        }
    }


}

QString MainWindow::Mask(QString & str)
{
    QString tmp;
    for (int i = 0; i < str.length(); i++)
    {
        if (i == 0 || i == str.length() - 1 || str[i] == ' ')
            tmp.push_back(str[i]);
        else
            tmp.push_back('*');
    }
    return tmp;
}

Entry & MainWindow::Find(QString word)
{
    for (QVector<Entry>::iterator i = wordbank.begin(); i < wordbank.end(); i++)
    {
        if (i->word == word)
            return *i;
    }
    Entry * tmp = new Entry;
    AddNewDialog("red", "IN-PROGRAM ERROR: NOT FOUND THE WORD");
    return *tmp;
}
