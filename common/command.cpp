#include "command.h"

Command::Command(CommandType type) // throw(logic_error)
{
	switch (type)
	{
	case CommandType::AskLogin:
		mLetter = new Login;
		break;

	case CommandType::AskHelp:
		mLetter = new Help;
		break;

		// ...

	default:
		throw std::logic_error("Incorrect type of command");
	}
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

QDataStream& operator>>(QDataStream& stream, Command& command)
{
	stream >> command;
	return stream;
}

QDataStream& operator<<(QDataStream& stream, const Command& command)
{
	stream << command;
	return stream;
}

Command* CommandFactory::create(QDataStream& stream)
{
	stream >> type;
	Command* command = new Command(type);
	stream >> *command;
	return command;
}
