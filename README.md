# RecitingWithUI

Project was designed and built from Augest 27, 2017 to September 2, 2017  
This documentation was written on January 16, 2019

<!-- SOME THOUGHTS
I thought this project is incomplete until I accidentally steped on this on
my local drive. It actually is a working program. I was pretty ambitious about
this project at the time and spent a lot of time planning and designing
the program. And now, I decided to create this readme you are reading.
-->

## Screenshots
### `addfrom`, `delete`
![addfrom](https://raw.githubusercontent.com/yifeiyin/RecitingWithUI/master/readme-img/addfrom.png)
### `add` `list` Identify Mode
![identify](https://github.com/yifeiyin/RecitingWithUI/blob/master/readme-img/identify.png)
### Spell Mode
![spell](https://raw.githubusercontent.com/yifeiyin/RecitingWithUI/master/readme-img/spell.png)

## Purpose
This program aims to help me learn new vocabulary and spellings but I am not 
sure if I really used it or not. What I am sure about is that it improved my
programming ability, specifically, with Qt Framework (using C++).


## Usage
- Down the bottom left, is the `inputBox` (or command line), you type whatever command there.
- On the right side is the list containing all the words
  - `[Identify]` `[Spell]` indicates the state of the word
  - letters after that (i.e. `pfpfpfpp`) indicates practice history (pass or fail)
  - Three consecutive `p`s will put the word into next stage. (identify -> spell -> finish)

### Available commands
  - `add <word>`
    - Add word to the list
  - `addfrom <filepath>` (also `af` `addf`)
    - Add word by reading a file (one word per line)
  - `delete <word>` `del <word>`
    - Delete word from the list
  - `reset wordbank`
    - Remove all words from the list
  - `list` `list <word>`
    - List all words
    - List a word
  - `clear` `cls`
    - Clear the log box
  - `save <filepath>`
    - Save the list to a file
  - `start` `start-practice` `sp`
    - Start practice, getting into practice mode

### Parctice Mode (IdentifyMode, SpellMode)
  - If any of the words are still in `Identify` state, it is `IdentifyMode`
  - Otherwise (all words are in spell mode), it is spell mode.
  - In Identify Mode, the word will show up one by one.
    - In the command line, **press** `y` `n` `p` `f` (yes, no, pass, fail) to indicate whether you know the word or not.
  - In Spell Mode, the word will be pronounced (by using macOS build-in say function)
    - Type the spelling in the upper (big letter) box
    - You can retry unlimited times
    - Type `y[es]` `n[o]` `p[ass]` `f[ail]` in the command line to create the record manually and goto the next word.



## Current Status
1. It works and most edge cases are handled properly.
2. All features, except tags, originally planned were correctly implemented.
3. There are still things to improve or clean up. They are marked in original code.
4. I won't make any changes to the code, feel free to fork this repo and change it yourself.

### Known Issues
- In `SpellMode`, the side list will not update. Just click the command box, it will refresh.
- In `SpellMode`, the side list should be masked so that people cannot cheat.
- `tags` was in the data structure but it was not used.


### License
The project is licensed under [GPLv3.0](https://www.gnu.org/licenses/gpl.md).
```
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
```
