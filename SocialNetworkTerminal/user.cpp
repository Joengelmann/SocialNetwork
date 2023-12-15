#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include "user.h"

//add a friend to the user
//pre: FriendID is the number Id of the friend trying to be added
//post: a friend is added to the user's friends away if they aren't already friends
void User::addFriend(std::size_t friendId) {
    for (auto e: Friends) {
        if(friendId == e) { //checks if user is already friends
            std::cout<<"user is already a friend!"<<std::endl;
            return;
        }
    }
    Friends.push_back(friendId);
}

//remove a friend from the user
//pre: FriendID is the number Id of the friend trying to be removed
//post: a friend is removed from the user if they were friends to begin with
void User::removeFriend(std::size_t friendId) {
    auto it = std::find(Friends.begin(), Friends.end(), friendId);
    if(it != Friends.end()){
        Friends.erase(it);
    }
    else{ //only runs if friendId is not found
        std::cout<<"users are not friends!"<<std::endl;
    }
}

//prints the amount of recent posts requested
//pre: howMany is the amount of posts wanted, and showOnlyPublic is true or false depending on which posts wanted to be shown
//post: howMany posts are printed
std::string User::displayPosts(std::size_t howMany, bool showOnlyPublic){
    std::size_t counter = 0;
    std::string posts;
    for(auto e: Messages){ //iterate across all messages 
        if(counter == howMany){ //end loop if enough posts have been added
            return posts;
        }
        if(e->displayPost().find("Private: ") == 0 ){ //if a private incoming post
            if(showOnlyPublic){
                continue;
            }
            posts+= e->displayPost().substr(9);//add post to the string
            posts += "\n\n";
            counter++;
        }
        else if(e->displayPost().find("Public: ") == 0 ){ //if a public incoming post
            posts+= e->displayPost().substr(8); //add the post to the string
            posts += "\n\n";
            counter++;
        }
        else{
            posts += e->displayPost(); //add post to the string
            posts += "\n\n";
            counter++;
        }
    }
    if(posts.size()==0){ //handles edge case of a user with no posts
        posts += "No posts were found!";
    }
    return posts;
}
