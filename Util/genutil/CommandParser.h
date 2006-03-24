/***************************************************************************
 *
 *   Copyright (C) 2002, 2006 by Willem van Straten
 *   Licensed under the Academic Free License version 2.1
 *
 ***************************************************************************/
#ifndef __CommandParser_h
#define __CommandParser_h

#include "ReferenceAble.h"
#include "Error.h"

#include <vector>
#include <string>
#include <iostream>

class CommandParser : public Reference::Able {

 public:

  //! verbose output from CommandParser methods
  static bool debug;

  //! the prompt shown to the user
  std::string prompt;

  //! null constructor
  CommandParser ();

  //! destructor
  virtual ~CommandParser () {}

  //! Initialize GNU readline and enable command completion
  void initialize_readline (const char*);

  //! return a help string
  std::string help (const std::string& command);

  //! parse a command and arguments in one string
  std::string parse (const std::string& commandargs);

  //! parse a command and its arguments
  std::string parse (const std::string& command, const std::string& args);

  //! quit flag raised by "quit"
  bool quit;

  //! verbose operation enabled by "verbose"
  bool verbose;

  // std::string usage ();

 protected:

  //! Derived classes add commands to the list using this method
  template <class Parser>
    void add_command (std::string (Parser::*method)(const std::string&),
		      const std::string& command,
		      const std::string& help,
		      const std::string& detailed_help = "",
		      char shortcut = 0);

  //! So that the shortcut key is not lost
  template <class Parser>
    void add_command (std::string (Parser::*method)(const std::string&),
		      char shortcut,
		      const std::string& command,
		      const std::string& help,
		      const std::string& detailed_help = "")
    { add_command (method, command, help, detailed_help, shortcut); }

 private:

  //! Pure virtual base class of interface to parser methods
  class Method;

  //! Available commands
  std::vector<Method*> commands;

  //! the command index, used by usage()
  // unsigned current_command;

  // readline interface
  static char** completion (const char *text, int start, int end);
  
  // readline interface
  static char* command_generator (const char* text, int state);

};

//! Pure virtual base class of the template class Command
class CommandParser::Method {
 public:
  Method() {}
  virtual ~Method () {}
  virtual std::string execute (const std::string& command) = 0;

  //! The command string corresponding to this method
  std::string command;
  //! The help string for this method
  std::string help;
  //! The detailed help string for this method
  std::string detail;
  //! The shortcut character corresponding to this method
  char shortcut;
};

//! Stores a pointer to a CommandParser sub-class and one of its methods
template <class Parser> class Command : public CommandParser::Method 
{
  friend class CommandParser;
  
  typedef std::string (Parser::*Method) (const std::string&);
  
 public:
  
  Command (Parser* _instance, Method _method, const std::string& _command,
	   const std::string& _help, const std::string& _detailed_help,
	   char _shortcut)
    {
      instance = _instance;
      method   = _method;
      command  = _command;
      help     = _help;
      detail   = _detailed_help;
      shortcut = _shortcut;
    }

  //! Execute method
  std::string execute (const std::string& args)
    {
      return (instance->*method) (args);
    }

 protected:
  //! Method of the sub-class to execute
  Method method;

  //! Instance through which method is called
  Parser* instance;

};

//! derived types may add commands to the list using this method
template<class P>
void CommandParser::add_command (std::string (P::*method) (const std::string&),
				 const std::string& cmd,
				 const std::string& help, 
				 const std::string& detailed_help,
				 char shortcut)
{
  if (debug)
    std::cerr << "CommandParser::add_command \"" << cmd << "\"" << std::endl;

  P* instance = dynamic_cast<P*> (this);
  if (!instance)
    throw Error (InvalidState, "CommandParser::add_command",
		 "instance/method mis-match");

  for (unsigned icmd=0; icmd < commands.size(); icmd++)
    if (cmd == commands[icmd]->command) {
      std::string error ("CommandParser::add_command command key taken");
      std::cerr << error << std::endl;
      throw error;
    }
  
  if (debug)
    std::cerr << "CommandParser::add_command new Command<P>" << std::endl;

  commands.push_back ( new Command<P> (instance, method,
				       cmd, help, detailed_help, shortcut));
}


#endif
