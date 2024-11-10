//GameMessages.h
#pragma once
#ifndef GAMEMESSAGES_H
#define GAMEMESSAGES_H

enum MessageType {
    ERROR, ROW, COLUMN, HIT, MISS, SINK, WIN1, WIN2, LOSE1, LOSE2, RETRY
};

class GameMessages {
public:
    static void drawMessage(MessageType message);
};

#endif // GAMEMESSAGES_H