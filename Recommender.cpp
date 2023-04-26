#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "User.h"
#include "treemm.h"
#include "Movie.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <cctype>
#include "utility.h"
using namespace std;

Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database)
:m_user_database(user_database), m_movie_database(movie_database){
}

void add(const string& m_id, unordered_map<string, MovieAndPoints>& s, int score){
    unordered_map<string, MovieAndPoints>::iterator it = s.find(m_id); //checks if the id is already in recs
    if(it != s.end()){
        MovieAndPoints& movie = s[m_id];
        movie.score += score; //if it is just adjust the compatibility score by score
    } else{
        MovieAndPoints obj(m_id, score); //if not, create a new object and add it to the unordered map
        s[m_id] = obj;
    }
}


bool Recommender::hasWatched (const string m_id, const string user_email) const{
    vector<string> watch_history=m_user_database.get_user_from_email(user_email)->get_watch_history();
    for(int i=0; i<watch_history.size(); i++){ //check if a user has watched a movie with m_id
        if(m_id==convert_to_lower(watch_history[i])){
            return true;
        }
    }
    return false;
}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    unordered_map<string, int> directors;
    unordered_map<string, int> actors;
    unordered_map<string, int> genres;
    unordered_map<string, MovieAndPoints> recs;
    vector<MovieAndRank> recommendations;
    if(movie_count<=0){
        return recommendations;
    }
    vector<string> watch_history=m_user_database.get_user_from_email(user_email)->get_watch_history();
    for (int i=0; i<watch_history.size(); i++){
        Movie* ptr=m_movie_database.get_movie_from_id(watch_history[i]);
        vector<string> directors_v = ptr->get_directors();
        vector<string> actors_v =ptr->get_actors();
        vector <string> genres_v =ptr->get_genres();
        for (int i=0; i<directors_v.size(); i++){
            directors[directors_v[i]]++; //add every director of every movie that the User has seen in an unordered map (along with an integer tracking the frequency of appearance)
        }
        for (int i=0; i<actors_v.size(); i++){ //do the same for actors and genres
            actors[actors_v[i]]++;
        }
        for (int i=0; i<genres_v.size(); i++){
            genres[genres_v[i]]++;
        }
    }
    
    for(unordered_map<string, int>::iterator it=directors.begin(); it!=directors.end(); ++it){
        if(!m_movie_database.get_movies_with_director(it->first).empty()){
            vector<Movie*> temp=m_movie_database.get_movies_with_director(it->first);
            for (int i=0; i<temp.size(); i++){
                add(temp[i]->get_id(), recs, it->second*20); //get all the movies directed by a given director and add their frequency times 20 to the compatibility score referring to the specific movie
            }
        }
    }
    
    for(unordered_map<string, int>::iterator it=actors.begin(); it!=actors.end(); ++it){
        if(!m_movie_database.get_movies_with_actor(it->first).empty()){
            vector<Movie*> temp=m_movie_database.get_movies_with_actor(it->first);
            for (int i=0; i<temp.size(); i++){
                add(temp[i]->get_id(), recs, it->second*30); //get all the movies of a given actor and add their frequency times 30 to the compatibility score referring to the specific movie

            }
        }
    }
    
    for(unordered_map<string, int>::iterator it=genres.begin(); it!=genres.end(); ++it){
        if(!m_movie_database.get_movies_with_genre(it->first).empty()){
            vector<Movie*> temp=m_movie_database.get_movies_with_genre(it->first);
            for (int i=0; i<temp.size(); i++){
                add(temp[i]->get_id(), recs, it->second); //get all the movies of a given genre and add their frequency times 1 to the compatibility score referring to the specific movie

            }
        }
    }
    
    for(unordered_map<string, MovieAndPoints>::iterator it=recs.begin(); it!=recs.end(); ++it){
        recommendations.push_back(MovieAndRank(it->first, it->second.score));
    } //now we add them to a container of type of MovieAndRank, as declared in the spec
    
    
    vector<MovieAndRank>::iterator it=recommendations.begin();
    while (it!=recommendations.end()){ //filters out movies that the User has already watched by deleting them from the vector
        if(hasWatched((*it).movie_id, user_email)){
            it=recommendations.erase(it);
        } else{
            ++it;
        }
    }
    
    sort(recommendations.begin(), recommendations.end(), [this](const MovieAndRank& a, const MovieAndRank& b) {
        if(a.compatibility_score!=b.compatibility_score){ //we use the STL sort algorithm to sort the vector
            return a.compatibility_score>b.compatibility_score;
        } else{
            Movie* m1=this->m_movie_database.get_movie_from_id(a.movie_id); //and a lambda function to sort based on all of the sorting requirements, so that movies with a higher score appear first
            Movie* m2=this->m_movie_database.get_movie_from_id(b.movie_id);
            if(m1->get_rating()!=m2->get_rating()){
                return m1->get_rating()>m2->get_rating();
            } else{
                return m1->get_title() < m2->get_title(); //we perform a case-sensitive ordering of the titles
            }
        }
    });
    
    if(movie_count>=recommendations.size()){
        return recommendations; //if the number of recommendations is less than what we specified, just return all of them
    }
    
    vector<MovieAndRank> final_recs;
    for (int i=0; i<movie_count; i++){ //if number of recommendations is greater than what we specified, return all recommendaions up to the specified number
        final_recs.push_back(recommendations[i]);
    }
    return final_recs;
}


