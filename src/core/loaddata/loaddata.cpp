
#include "../models/User.h"
#include "../menu/manager_menu.h"
#include "../menu/user_menu.h"
#include "../data/DataStore.h"

void handleLoggedInUser(User& user) {
    DataStore::loadAllData();
    
    if (user.getRole() == UserRole::Manager) {
        showManagerMenu(user);
    } else {
        showUserMenu(user);
    }

    // Tu dong dang xuat sau khi xong menu
    user.setRole(UserRole::Failed);
}

