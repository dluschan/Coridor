#include "command.h"

Command::Command(int _type, QString text) // throw(logic_error)
{
	switch (_type)
	{
	case AskLogin:
		mLetter = new Login;
		break;

	case AskHelp:
		mLetter = new Help;
		break;

		// ...

	default:
		throw std::logic_error("Incorrect type of command");
	}

	// virtual call in constructor!
	cast(text);
}

/*istream& operator>>(istream& in, Command& c)
{
	in >> c.name;
	return in;
}

ostream& operator<<(ostream& out, const Command& c)
{
	out << c.name << endl;
	return out;
}

istream& operator>>(istream& in, Login& l)
{
	in >> l.login;
	return in;
}

ostream& operator<<(ostream& out, const Login& l)
{
	out << l.login << endl;
	return out;
}*/
