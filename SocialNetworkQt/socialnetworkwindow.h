#ifndef SOCIALNETWORKWINDOW_H
#define SOCIALNETWORKWINDOW_H

#include <QMainWindow>
#include <stack>
#include "network.h"
#include "user.h"
#include "post.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SocialNetworkWindow; }
QT_END_NAMESPACE

class SocialNetworkWindow : public QMainWindow
{
    Q_OBJECT

public:
    //constructor for the social network window
    //pre: none
    //post: constructs the social network object and reads in the users and posts
    SocialNetworkWindow(QWidget *parent = nullptr);

    //destructor
    //pre: none
    //post: the window has been deleted
    ~SocialNetworkWindow();

    //displays the recent posts on the left side of the screen
    //pre: either Me or Them pointers in the Network obejct point to a user
    //post: the most recent posts of the correct user's profiles are displayed on the left
    void setPosts();

    // function to handle clicking on a user's friend
    // pre: none
    // post: the user who was clicked on's profile is now displayed
    void friendButtonClicked();

    // function to handle clicking on a suggested friend
    //pre: none
    //post: the friend is added to the logged in user and the text displayed is changed
    void suggestedFriendButtonClicked();

    //displays all friends of the current User's profile
    //pre: Either Me or Them pointers in the network class are pointing at a valid user
    //post: displays the friends of the User who'se profile we are on
    void setFriends();

    //displays all suggested friends of the the logged in user
    //pre: Me pointer is pointing at the user that is logged in
    //post: displays the suggested friends of Me
    void setSuggested();

    // Sets the text of the add friend button depending on if the user is already friends with the logged in user
    // pre: We are on a profile that is not the logged in user
    // post: The text of the add friend button displays the correct text
    void setAddButtonText();

    //button that brings the user back to their profile
    //pre: Someone else's profile is displayed
    //post: The loged in user's profile is redisplayed
    void on_ProfileButton_clicked();

    //sets up the user interface after the user submits a valid name
    //pre: a name has been entered into the text box
    //post: The profile for the name that was entered is shown
    void on_SubmitNameButton_clicked();

    //button that returns user to login screem
    //pre: none
    //post: me pointer is set to null and the login screan is displayed
    void on_LogoutButton_clicked();

    //button that adds a displayed user as friends to the logged in user
    //pre: Someone else's profile is displayed
    //post: the logged in user and displayed user are now friends and calls the function to set the button's text
    void on_AddFriendButton_clicked();

    //button that goes back a page, similar to a browser back button
    //pre: none
    //post: the previous user profile or login page is displayed
     void on_BackButton_clicked();

private:
    Ui::SocialNetworkWindow *ui;
    Network Net;
    User* Me = nullptr;
    User* Them = nullptr;
    std::stack<User*> Prev;
};
#endif // SOCIALNETWORKWINDOW_H
