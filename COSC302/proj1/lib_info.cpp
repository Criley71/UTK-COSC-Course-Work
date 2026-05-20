/* Connor Riley, Malika Arifova
2/1/2023 
Cosc302 
Dr.Emrich
Project1 lib_info.cpp A music library organizer
* Project 1 is about storing data from the music library, so it basically 
prints out all of MP3 files. For each artist the program will print out the artist name,
then the number of songs that have that artist's name, and then the total time of all 
songs that have that artist's name.
*/

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>

using namespace std;

struct Song {
	string songTitle, songTime;
	Song() {};
	Song(string n, string t) {
		songTitle = n;
		songTime = t;
	}

};

struct Album {
	map <int, Song > songMap;
	string albumName;
	int albumTime;
	int albumNsongs;  // optional variable but makes it easier
	Album() {};
	Album(string s) {
		albumName = s;
		albumNsongs = 1;
		albumTime = 0;
	}
};

struct Artist {
	map <string, Album > albumMap;
	string artistName;
	int artistTime;
	int artistNsongs;
	Artist() {};
	Artist(string s) {
		artistName = s;
		artistNsongs = 1;
		artistTime = 0;
	}
}; // Here above we ended up using 3
//structs for songs, albums, and artists. Each of them hold data pertaining to 
//what they represent, like the artist/album or song name, amount of songs for the 
//artist or album and amount of time for the artist or album. The artist struct holds
//a map that contains the album struct for the albums by that artist, similarly the 
//album struct has a map of songs on the album.

int main(int argc, char* argv[])
{
	ifstream fin;
	string lineData, title, sLength, name, album, genre;
	int min, sec, totalSongTime, trackNum;
	char colon;
	stringstream ss;
	
	//We made a map to take in artists, and iterators for the 3 maps
	
	map<string, Artist> artistMap;
	map<string, Artist>::iterator artistItr;
	map<string, Album>::iterator albumItr;
	map<int, Song>::iterator songItr;
	//The music library is taken in through a file given as a command line argument
	fin.open(argv[1]);
	
	/* Here below we read in the data we put getline in a while loop that reads in a line from the file in each loop 
	and store it as one string variable. Using a stringstream we seperate the line into its corresponding
	variables. For the title, name and album we loop through checking for underscores and replacing them with
	a white space instead. last we take the time and using an istringstream we convert the analog time into 2 variables
	for minutes and seconds to convert to its total time in seconds.
	*/
	while (getline(fin, lineData)) {
		istringstream iss(lineData);
		iss >> title >> sLength >> name >> album >> genre >> trackNum;
		for (int i = 0; i < title.length(); i++) {
			if (title[i] == '_') {
				title[i] = ' ';
			}
		}
		for (int i = 0; i < name.length(); i++) {
			if (name[i] == '_') {
				name[i] = ' ';
			}
		}
		for (int i = 0; i < album.length(); i++) {
			if (album[i] == '_') {
				album[i] = ' ';
			}
		}
		ss << sLength;
		ss >> min >> colon >> sec;
		totalSongTime = ((min * 60) + sec);
	/* Here we have assigned variables and we can begin to add them into their corresponding maps.
	* We first see if the artist has already been added into the artist map before by setting
	* having the artist iterator to finding the artists name. If the iterator has reached the end of the map
	* that means the artist hasnt been created yet nor any of their albums. We then inserted the artist into the map
	* with their name being the key value. This creates the artist struct which makes an album map 
	* for their the artist. we then insert the album into the album map creating the album struct for 
	* the artist which holds the song map. last we insert the songs into the map using the trackNum
	* and song name.
	*/
		artistItr = artistMap.find(name);
		//if artist doesnt exist yet
		if (artistItr == artistMap.end()) {
			//add artist to map
			artistMap.insert(make_pair(name, Artist(name)));
			artistMap[name].artistTime += totalSongTime;

			//make an album map and insert pair, set time of album += totalSongtime
			artistMap[name].albumMap.insert(make_pair(album, Album(album)));
			artistMap[name].albumMap[album].albumTime += totalSongTime;

			//add song in the song map, key = tracknum, sets the title and the length in analog time min:sec
			artistMap[name].albumMap[album].songMap.insert(make_pair(trackNum, Song(title, sLength)));
		}
		/*If the artist iterator didnt reach the end then the artist already was in the map
		* meaning we dont need to create the artist. We then use the album iterator to check if
		* the album album exits. if it doesnt make the album struct and add the song to it. If it does then just
		* add the song to songMap in the album.
		*/
		//if the artist does exist
		else if (artistItr != artistMap.end()) {
			//add 1 to artist nSongs, add totalSongTime to artist time;
			artistItr->second.artistNsongs += 1;
			artistItr->second.artistTime += totalSongTime;
			albumItr = artistItr->second.albumMap.find(album);
			//if the album doesnt exist, add the album to the album map, add the total length to album time
			if (albumItr == artistItr->second.albumMap.end()) {
				artistItr->second.albumMap.insert(make_pair(album, Album(album)));
				artistItr->second.albumMap[album].albumTime += totalSongTime;

				artistItr->second.albumMap[album].songMap.insert(make_pair(trackNum, Song(title, sLength)));

			}
			//if the album exists, add the total time and add 1 to albumNsongs
			else if (albumItr != artistItr->second.albumMap.end()) {
				artistItr->second.albumMap[album].albumTime += totalSongTime;
				artistItr->second.albumMap[album].albumNsongs += 1;
				artistItr->second.albumMap[album].songMap.insert(make_pair(trackNum, Song(title, sLength)));
			}

		}

		iss.clear();
		ss.clear();
	}
	/* Here below, we used a for loop to print to iterator through the artist map, then 2 inner for loops for the album and song maps. 
	* We converted the total time of the artist and album back to analog time.
	*/
	for (artistItr = artistMap.begin(); artistItr != artistMap.end(); artistItr++) {
		cout << artistItr->second.artistName << ": " << artistItr->second.artistNsongs << ", " << (artistItr->second.artistTime) / 60;
		if ((artistItr->second.artistTime % 60) >= 10) {
			cout << ":" << (artistItr->second.artistTime) % 60 << "\n";
		}
		else if ((artistItr->second.artistTime % 60) < 10) {
			cout << ":0" << (artistItr->second.artistTime) % 60 << "\n";
		}
		for (albumItr = artistItr->second.albumMap.begin(); albumItr != artistItr->second.albumMap.end(); albumItr++) {
			cout << "        " << albumItr->second.albumName << ": " << albumItr->second.albumNsongs << ", " << (albumItr->second.albumTime) / 60;
			if ((albumItr->second.albumTime % 60) >= 10) {
				cout << ":" << (albumItr->second.albumTime) % 60 << "\n";
			}
			else if ((albumItr->second.albumTime % 60) < 10) {
				cout << ":0" << (albumItr->second.albumTime) % 60 << "\n";
			}
			for (songItr = albumItr->second.songMap.begin(); songItr != albumItr->second.songMap.end(); songItr++) {
				cout << "                " << songItr->first << ". " << songItr->second.songTitle << ": " << songItr->second.songTime << "\n";
			}
		}
	}

	fin.close();
}