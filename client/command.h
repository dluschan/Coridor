#ifndef COMMAND_H
#define COMMAND_H

#include <QString>
#include <cstdlib>
#include <iostream>
#include <map>
#include <stdexcept>

using namespace std;

enum CommandType
{
	AskLogin,
	AskHelp,
	AskPlayers,
	CreateLobby,
	AskLobbies,
	WrongCommand
};

class Command
{
public:
	Command(int _type, QString text);

	QString name;

	virtual ~Command()
	{
		if (mLetter)
		{
			// virtual call in destructor!
			erase();
		}

		delete mLetter; // delete Letter for Envelope
						// delete 0      for Letter
	}

	virtual void cast(QString name) const
	{
		mLetter->cast(name);
	}
	virtual void erase()
	{
		mLetter->erase();
	}

protected:
	// letter constructor
	Command()
		: mLetter(NULL)
	{
	}

private:
	Command(const Command&);
	Command& operator=(Command&);

	Command* mLetter; // pointer to letter
};

class Login : public Command
{
public:
	QString login;
	~Login()
	{
		// cout << "~Login()" << endl;
	}
	virtual void cast(QString text) const
	{
		// cout << "Katon!" << endl;
		// login = text;
	}
	virtual void erase()
	{
		// cout << "Login:erase()" << endl;
	}

private:
	friend class Command;
	Login()
	{
	}
	Login(const Login&);
	Login& operator=(Login&);
};

class Help : public Command
{
public:
	QString message = "-help: ask for available commands\n-players list: ask for player list\n-create lobby 'LobbyName': ask for creating a lobby\n-lobbies list: ask for lobbies list\n";
	~Help()
	{
		// cout << "~Help()" << endl;
	}
	virtual void cast(QString text) const
	{
		// cout << "Mokuton!" << endl;
	}
	virtual void erase()
	{
		// cout << "Help::erase()" << endl;
	}

private:
	friend class Command;
	Help()
	{
	}
	Help(const Help&);
	Help& operator=(Help&);
};

/*class PlayersList : public Command
{
public:
	~PlayersList()
	{
		// cout << "~Login()" << endl;
	}
	virtual void cast(QString name) const
	{
		// cout << "Katon!" << endl;
	}
	virtual void show() const
	{
		// cout << "Login::show()" << endl;
	}
	virtual void erase()
	{
		// cout << "Login:erase()" << endl;
	}

private:
	friend class Command;
	PlayersList()
	{
	}
	PlayersList(const Login&);
	PlayersList& operator=(Login&);
};

class Lobby : public Command
{
public:
	~Lobby()
	{
		// cout << "~Help()" << endl;
	}
	virtual void cast(QString name) const
	{
		// cout << "Mokuton!" << endl;
	}
	virtual void show() const
	{
		// cout << "Help::show()" << endl;
	}
	virtual void erase()
	{
		// cout << "Help::erase()" << endl;
	}

private:
	friend class Command;
	Lobby()
	{
	}
	Lobby(const Help&);
	Lobby& operator=(Help&);
};*/

#endif // COMMAND_H
