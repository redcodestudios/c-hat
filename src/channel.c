#include "inc/channel.h"

Channel* new_channel(User* owner, char* message){
    char* tmp = malloc(strlen(message));
    strcpy(tmp, message);

    strtok(tmp, "-");
    char* channel_name  = strtok(NULL, "-");
    Channel* new_channel = malloc(sizeof(Channel));
    new_channel->members = malloc(sizeof(Member*) * 300);
    
    new_channel->name = channel_name;
    new_channel->owner = owner;
    new_channel->last_member = 0;
    
    char* queue_name = (char*) malloc(11);
    
    strcpy(queue_name, "#");
    strcat(queue_name, channel_name);

    new_channel->queue_name = queue_name;
    create_q(new_channel->queue_name);

    return new_channel;
}

void add_member(User* user, Channel* channel){
    Member* new_member = malloc(sizeof(Member));
    new_member->user = user;
    new_member->valid = 1;
    channel->members[channel->last_member] = (Member*) malloc(sizeof(Member));
    channel->members[channel->last_member] = new_member;
    channel->last_member+=1;
}

void leave_channel(User* member, Channel* channel){
    for(int i=0; i<channel->last_member; i++){
        if(channel->members[i]->valid == 1 && strcmp(channel->members[i]->user->name, member->name) == 0){
            channel->members[i]->valid = 0;
        }
    }
}

