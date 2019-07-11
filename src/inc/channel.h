#pragma once

#include "user.h"
#include "queue_utils.h"

typedef struct Member{
    User* user;
    int valid;
}Member;

typedef struct Channel{
    char* name;
    char* queue_name;
    User* owner;
    Member** members;
    int last_member;
}Channel;

Channel* new_channel(User* owner, char* message);
void add_member(User* member, Channel* channel);
void leave_channel(User* member, Channel* channel);