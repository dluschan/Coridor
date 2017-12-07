#include "command.h"

Command::Command(CommandType type) // throw(logic_error)
{
	switch (type)
	{
	case CommandType::AskLogin:
		mLetter = new Login;
		// stream >> *mLetter;
		break;

	case CommandType::AskHelp:
		mLetter = new Help;
		break;

		// ...

	default:
		throw std::logic_error("Incorrect type of command");
	}

	// virtual call in constructor!
	execute();
}

QDataStream& operator>>(QDataStream& stream, CommandType& type)
{
	stream >> type;
	return stream;
}

QDataStream& operator<<(QDataStream& stream, const CommandType& type)
{
	stream << type;
	return stream;
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
