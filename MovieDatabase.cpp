#include "MovieDatabase.h"
#include "Movie.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cctype>
#include "utility.h"
using namespace std;

MovieDatabase::MovieDatabase()
{
    loaded=false;
}

MovieDatabase::~MovieDatabase(){
    for(int i=0; i<to_delete.size(); i++){
        delete to_delete[i];
    }
}

bool MovieDatabase::load(const string& filename)
{
    if(loaded){
        return false;
    }
    ifstream file(filename);
    for(;;){ //tested with both an empty line in the end and without one
        string id="";
        getline(file, id);
        if (id.empty()) { //if id is empty
            break; //we have reached the end of the file so return
        }
        id=convert_to_lower(id); //convert everything to lowercase so that we can perform case-insensitive search
        string title="";
        getline (file, title);
        string year="";
        getline(file, year);
        string directors="";
        getline (file, directors);
        vector<string> directors_v=process_commas(directors); //convert the comma separated list into a vector
        directors_v=convert_to_lower(directors_v);
        string actors="";
        getline (file, actors);
        vector <string> actors_v=process_commas(actors); //convert the comma separated list into a vector
        actors_v=convert_to_lower(actors_v);
        string genres="";
        getline (file, genres);
        vector<string> genres_v=process_commas(genres); //convert the comma separated list into a vector
        genres_v=convert_to_lower(genres_v);
        string str_rating="";
        getline (file, str_rating);
        //convert to float
        float rating=stof(str_rating); 
        string line;
        getline(file,line); //advances the empty line after every Movie
        Movie* temp=new Movie(id, title, year, directors_v, actors_v, genres_v, rating);
        to_delete.push_back(temp); //adds the movie pointer in a container of objects to be destructed
        id_to_movie.insert(id, temp);
        for (int i=0; i<directors_v.size(); i++){
            director_to_movie.insert(directors_v[i], temp);
        }
        for (int i=0; i<actors_v.size(); i++){
            actor_to_movie.insert(actors_v[i], temp);
        }
        for (int i=0; i<genres_v.size(); i++){
            genre_to_movie.insert(genres_v[i], temp);
        }
    }
    loaded=true;
    return true;
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    string id_low=convert_to_lower(id);
    TreeMultimap<string, Movie*>::Iterator it=id_to_movie.find(id_low);
    return it.get_value();
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    string director_low=convert_to_lower(director);
    vector<Movie*> v;
    TreeMultimap<string, Movie*>::Iterator it=director_to_movie.find(director_low);
    while(it.is_valid()){
        v.push_back(it.get_value()); //adds all the movies directed by the given director into v and returns it
        it.advance();
    }
    return v;
}

//same function for actor and genre
vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    string actor_low=convert_to_lower(actor);
    vector<Movie*> v;
    TreeMultimap<string, Movie*>::Iterator it=actor_to_movie.find(actor_low);
    while(it.is_valid()){
        v.push_back(it.get_value());
        it.advance();
    }
    return v;
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    string genre_low=convert_to_lower(genre);
    vector<Movie*> v;
    TreeMultimap<string, Movie*>::Iterator it=genre_to_movie.find(genre_low);
    while(it.is_valid()){
        v.push_back(it.get_value());
        it.advance();
    }
    return v;
}

//converts a comma-separated list of strings to a vector of strings
vector<string> MovieDatabase::process_commas(const string s) const {
    vector<string> temp;
    string value;
    stringstream ss(s);
    while(getline(ss,value,',')){
        temp.push_back(value);
    }
    return temp;
}


