//GameMessages.cpp
#include "GameMessages.h"
#include <iostream>
#include <string>
using namespace std;

void GameMessages::drawMessage(MessageType message) {
    string emptyheader = "\n\n\n\n\n";
    string startheader = "\n                ___  ____ ____ ____ ____    ____ _  _ ___ ____ ____\n                |__] |__/ |___ [__  [__     |___ |\\ |  |  |___ |__/\n                |    |  \\ |___ ___] ___]    |___ | \\|  |  |___ |  \\ \n\n";
    string rowheader = "\n      ____ ____ _    ____ ____ ___    ____    ____ ____ _ _ _     _      __  \n      [__  |___ |    |___ |     |     |__|    |__/ |  | | | |    /|  __ |__| \n      ___] |___ |___ |___ |___  |     |  |    |  \\ |__| |_|_|    _|_    |__|\n\n";
    string colheader = "\n      ____ _  _ ___     ____    ____ ____ _    _  _ _  _ _  _     _      __  \n      |__| |\\ | |  \\    |__|    |    |  | |    |  | |\\/| |\\ |    /|  __ |__|  \n      |  | | \\| |__/    |  |    |___ |__| |___ |__| |  | | \\|    _|_    |__|\n\n";
    string hitheader = "\n                       _ ___ . ____    ____    _  _ _ ___\n                       |  |  ' [__     |__|    |__| |  |  \n                       |  |    ___]    |  |    |  | |  |  \n\n";
    string missheader = "\n                 _   _ ____ _  _    _  _ _ ____ ____ ____ ___  \n                  \\_/  |  | |  |    |\\/| | [__  [__  |___ |  \\ \n                   |   |__| |__|    |  | | ___] ___] |___ |__/ \n\n";
    string sinkheader = "\n      _   _ ____ _  _    ____ _  _ _  _ _  _    ____    ____ _  _ _ ___  \n       \\_/  |  | |  |    [__  |  | |\\ | |_/     |__|    [__  |__| | |__] \n        |   |__| |__|    ___] |__| | \\| | \\_    |  |    ___] |  | | |    \n\n";
    string winheader1 = "\n      ____ _    _       ____ _  _ _ ___  ____    ____ ____ _  _ _  _ ___  \n      |__| |    |       [__  |__| | |__] [__     |___ |  | |  | |\\ | |  \\ \n      |  | |___ |___    ___] |  | | |    ___]    |    |__| |__| | \\| |__/ \n\n";
    string winheader2 = "\n                         _   _ ____ _  _    _ _ _ _ _  _ \n                         \\_/  |  | |  |    | | | | |\\ | \n                          |   |__| |__|    |_|_| | | \\| \n\n";
    string loseheader1 = "\n      ____ ____ _  _    ____ _  _ ___    ____ ____    ___ _  _ ____ _  _ ____ \n      |__/ |__| |\\ |    |  | |  |  |     |  | |___     |  |  | |__/ |\\ | [__  \n      |  \\ |  | | \\|    |__| |__|  |     |__| |        |  |__| |  \\ | \\| ___] \n\n";
    string loseheader2 = "\n                     _   _ ____ _  _    _    ____ ____ ____ \n                      \\_/  |  | |  |    |    |  | [__  |___ \n                       |   |__| |__|    |___ |__| ___] |___ \n\n";

    switch (message) {
    case ERROR: cout << "\n\nInvalid input.\nPlease try again.\n\n"; break;
    case RETRY: cout << "\n\nYou already guessed that square.\nPlease try again.\n\n"; break;
    case ROW:       cout << rowheader; break;
    case COLUMN:    cout << colheader; break;
    case HIT:       cout << hitheader; break;
    case MISS:      cout << missheader; break;
    case SINK:      cout << sinkheader; break;
    case WIN1:      cout << winheader1; break;
    case WIN2:      cout << winheader2; break;
    case LOSE1:     cout << loseheader1; break;
    }
}