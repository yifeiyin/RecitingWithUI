#ifndef ENTRY_H
#define ENTRY_H

#include <QString>

enum EntryStatus { Identify = 0, Spell, Finish, Pending };

class Entry {
public:
    QString word;
    QString remark;
    QString history;
    EntryStatus status;

    Entry(QString w = "default", QString r = "", QString h = "", EntryStatus s = Identify)
    {
        word = w;
        remark = r;
        history = h;
        status = s;
    }
};

#endif // ENTRY_H
