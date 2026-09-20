#pragma once

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>

/* ivorytower build initialization system */

namespace iblis {
	// Runs a command, returns exit status.
	int runCmd(const std::string & cmd, const std::vector<std::string> & args);
	// Writes a file.
	bool writeFile(const std::string & path, const std::string & content);

	// cvar.cpp

	// The Registerable linked-list is a singleton, to which things are attached.
	class Registerable {
	public:
		const char * name;
		const char * purpose;
		Registerable(const char * name, const char * purpose);
		virtual ~Registerable();
		Registerable * regNext;
		static Registerable * regFirst;
		template<class T> static T * find(const char * name) {
			for (Registerable * i = regFirst; i; i = i->regNext) {
				if (strcmp(name, i->name))
					continue;
				T * asTarg = dynamic_cast<T *>(i);
				if (asTarg)
					return asTarg;
			}
			return nullptr;
		}
	};

	extern Registerable * first;

	class Act : public Registerable {
	public:
		Act(const char * name, const char * purpose);
		virtual int execute() = 0;
	};

	class Cvar : public Registerable {
	public:
		Cvar(const char * name, const char * purpose);
		virtual bool parse(const char * value) = 0;
		virtual std::string get() = 0;
	};

	class CvarBool : public Cvar {
	public:
		bool value;
		CvarBool(const char * name, const char * purpose, bool def);
		virtual bool parse(const char * value) override;
		virtual std::string get() override;
	};

	class CvarStr : public Cvar {
	public:
		std::string value;
		CvarStr(const char * name, const char * purpose, std::string def);
		virtual bool parse(const char * value) override;
		virtual std::string get() override;
	};

	class Component : public CvarBool {
	public:
		Component(const char * name, const char * purpose, bool def);
		// 'meta component' flag (false, is for "all" only)
		virtual bool isMeta();
		virtual bool install() = 0;
	};

	// version.cpp
	extern const char * version;
}
