#include "api_bili.h"

API_Bili::API_Bili(int id,QObject *parent) : QObject(parent)
{
    this->roomId = id;
}

void API_Bili::start()
{
    this->run();
}
