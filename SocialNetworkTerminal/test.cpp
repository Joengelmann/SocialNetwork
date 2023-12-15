//No Bugs Caught!

#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include "user.h"
#include "network.h"
#include "post.h"

using namespace std;

//checks if the add and remove friends functions works
//pre: none
//post: tests all functions in the user class
void testUsers() {
    cout << "testUsers...";
    User* u1 = new User(1, "Jason Chip", 2002, 94082, vector<size_t> ());
    User* u2 = new User(2, "Joe Shmoe", 2004, 94012, vector<size_t> ());
    u1->addFriend(2);
    u2->addFriend(1);

    assert(u1->getId() == 1);
    assert(u2->getName() == "Joe Shmoe");

    assert(u1->getFriends().size()>0);
    assert(u2->getFriends().size()>0);

    u1->removeFriend(2);
    u2->removeFriend(1);

    assert(u1->getFriends().size()==0);
    assert(u2->getFriends().size()==0);
    // ...
    cout << " Passed!" << endl;
}

//checks if the network functions are working
//pre: none
//post: tests all functions in the network class
void testNetwork(){
    cout << "testNetwork...";

    Network N;
    User* u1 = new User(1, "Jason Chip", 2002, 94082, vector<size_t> ());
    N.addUser(u1);
    User* u2 = new User(2, "Jules Ben", 2009, 94031, vector<size_t> ());
    N.addUser(u2);
    User* u3 = new User(3, "Mike Smith", 2004, 94912, vector<size_t> ());
    N.addUser(u3);

    assert(N.getId("Jules Ben")==2);
    assert(N.getUser(3) == u3);
    assert(N.getSize()==3);

    N.addConnection(u1->getName(),u2->getName());
    N.addConnection(u2->getName(),u3->getName());
   
    assert(u1->getFriends().size()==1);
    assert(u2->getFriends().size()==2);
    assert(u3->getFriends().size()==1);

    N.removeConnection(u2->getName(),u3->getName());

    assert(u2->getFriends().size()==1);
    assert(u3->getFriends().size()==0);

    cout << " Passed!" <<endl;
}

//checks if the search functions in Network.cpp are working
//pre: none
//post: All search functions have been run and no assert statements were failed
void testSearches(){
    cout << "testSearches...";

    Network S;
    User* u0 = new User(0, "Jason Chip", 2002, 94082, vector<size_t> ());
    S.addUser(u0);
    User* u1 = new User(1, "Jules Ben", 2009, 94031, vector<size_t> ());
    S.addUser(u1);
    User* u2 = new User(2, "Mike Smith", 2004, 94912, vector<size_t> ());
    S.addUser(u2);
    User* u3 = new User(3, "Sam Sammy", 2012, 92921, vector<size_t> ());
    S.addUser(u3);
    User* u4 = new User(4, "Bill Billy", 2007, 94781, vector<size_t> ());
    S.addUser(u4);


    S.addConnection(u0->getName(),u1->getName());
    S.addConnection(u1->getName(),u2->getName());
    S.addConnection(u1->getName(),u3->getName());
    S.addConnection(u3->getName(),u4->getName());

    vector<size_t> paths1 = S.shortestPath(u1->getId(),u4->getId());

    assert(paths1.size()==4);
    assert(paths1[2]==u3->getId());
   
    size_t to;
    vector<size_t> paths2 = S.distanceUser(0,to, 3);

    assert(to == 4);
    assert(paths2[1]==1);

    S.addConnection(u0->getName(),u4->getName());

    size_t score;
    vector<size_t> suggestedFriends = S.suggestFriends(0, score);

    assert(suggestedFriends[0]==3);
    assert(score == 2);

    vector<vector<size_t> > groups1;
    groups1 = S.groups();

    assert(groups1.size()==1);

    S.removeConnection(u1->getName(),u2->getName());

    vector<vector<size_t> > groups2;
    groups2 = S.groups();

    assert(groups2.size()==2);

    cout << "Passed!" <<endl;
}

void testPosts(){
    Post* p1 = new Post(0,"hey this is a post",4);
    Post* p2 = new Post(1,"hey this is still a post",8);

    assert(p1->getLikes()==4);
    assert(p2->getLikes()==8);

    incomingPost* ip1 = new incomingPost(2,"this is an incoming post",8, false, "James");
    incomingPost* ip2 = new incomingPost(3,"this is still an incoming post",3, true, "Ben");

    assert(ip1->getLikes()==8);
    assert(ip2->getLikes()==3);

    Network S;
    User* u0 = new User(0, "Jason Chip", 2002, 94082, vector<size_t> ());
    S.addUser(u0);
    User* u1 = new User(1, "Jules Ben", 2009, 94031, vector<size_t> ());
    S.addUser(u1);
    User* u2 = new User(2, "Mike Smith", 2004, 94912, vector<size_t> ());
    S.addUser(u2);
    User* u3 = new User(3, "Sam Sammy", 2012, 92921, vector<size_t> ());
    S.addUser(u3);

    u0->addPost(p1);
    u0->addPost(p2);
    u0->addPost(ip1);
    u0->addPost(ip2);

    string s1 = u0->displayPosts(3,true);
    assert(s1.length()>5);
    assert(u1->getMessagesCount()==0);
    assert(u0->getMessagesCount()==4);

    S.addPost(1,"This is my post!!!", 8, false, 1, true);
    S.addPost(1,"Nice post Jules !", 8, true, 2, true);

    assert(S.getTotalPosts()==6);

    string s2 = S.displayPosts("Jules Ben", 2, true);
    cout<<s2<<endl;
    assert(s2.length()>5);

    cout<< "Passed!" << endl;
}

int main(){
    testUsers();
    testNetwork();
    testSearches();
    testPosts();
    return 0;
}