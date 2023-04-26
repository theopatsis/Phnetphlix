#ifndef USERDATABASE_INCLUDED
#define USERDATABASE_INCLUDED

#include <string>
#include "treemm.h"
class User;

class UserDatabase
{
  public:
    UserDatabase();
    ~UserDatabase();
    bool load(const std::string& filename);
    User* get_user_from_email(const std::string& email) const;

  private:
    TreeMultimap<std::string, User*> user_map;
    bool loaded;
    std::vector<User*> to_delete;
    int convert(const std::string s) const;
};

#endif // USERDATABASE_INCLUDED
