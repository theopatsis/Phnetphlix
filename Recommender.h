#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <string>
#include <vector>

class UserDatabase;
class MovieDatabase;

struct MovieAndRank
{
    MovieAndRank(const std::string& id, int score)
     : movie_id(id), compatibility_score(score)
    {}

    std::string movie_id;
    int compatibility_score;
};

struct MovieAndPoints //I have defined a new struct similar to MovieAndRank, which has a default constructor
{
    MovieAndPoints(){
        m_id="";
        score=0;
    }
    
    MovieAndPoints(const std::string str, int num){
        m_id=str;
        score=num;
    }
    
    std::string m_id;
    int score;
};

class Recommender
{
  public:
    Recommender(const UserDatabase& user_database,
                const MovieDatabase& movie_database);
    std::vector<MovieAndRank> recommend_movies(const std::string& user_email,
                                               int movie_count) const;

  private:
    const UserDatabase& m_user_database;
    const MovieDatabase& m_movie_database;
    bool hasWatched (const std::string id, const std::string user_email) const;
};

#endif // RECOMMENDER_INCLUDED
