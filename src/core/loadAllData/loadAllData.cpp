
#include "../models/User.h"
#include "../menu/admin_menu.h"
#include "../menu/user_menu.h"
#include "../data/DataStore.h"

void handleLoggedInUser(User &user)
{
    DataStore::loadAllData();

    if (user.getRole() == UserRole::Manager)
    {
        showManagerMenu(user);
    }
    else
    {
        showUserMenu(user);
    }

    // Tự động đăng xuất sau khi xong menu
    user.setRole(UserRole::Guest);
}
