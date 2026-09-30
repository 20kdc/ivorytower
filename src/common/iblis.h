#pragma once

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>

/* ivorytower build initialization system */

// Immovable (rule of three)
#define IBLIS_IMMOVABLE(T) T(const T &) = delete; T & operator=(const T &) = delete;
#define IBLIS_WARN(str) iblis::warn(__PRETTY_FUNCTION__, str)
#define IBLIS_HAS_KIND static const char * kind; virtual const char * getKind() override;
#define IBLIS_KIND(T, value) const char * T::kind = value; const char * T::getKind() { return T::kind; }

namespace iblis {
	// Environ abstraction.
	struct Environ {
		std::vector<std::string> inner;
		static Environ readProcess();
		void del(const std::string & key);
		void set(const std::string & key, const std::string & val);
	};

	// Runs a command, returns exit status.
	int runCmd(const std::vector<std::string> & argv, const Environ & environ = Environ::readProcess());
	// Canonicalizes a path. This implements a 'graceful path collapse' algorithm and thus cannot fail.
	// However, use with extreme caution; absolute paths can be brittle if the user prefers symlinks for organizational purposes.
	std::string realPath(const std::string & path);
	// Reads a file to a series of lines.
	// Returns empty list on error (would return an empty string if the file is empty)
	std::vector<std::string> readFile(const std::string & path);
	// Writes a file from a string.
	bool writeFile(const std::string & path, const std::string & content);
	void warn(const char * subsystem, const std::string & message);

	// Template magic underlying Subsystem
	class SubsystemFactory {
	public:
		virtual ~SubsystemFactory() {}
		virtual void * build() = 0;
	};

	// Template magic underlying Subsystem
	template <class L>
	class LambdaSubsystemFactory : public SubsystemFactory {
	public:
		LambdaSubsystemFactory(L lambda) : lambda(lambda) {}
		L lambda;
		virtual void * build() { return lambda(); }
	};

	// A subsystem activates when requested by explicit reference.
	template <class T>
	class Subsystem {
	public:
		template<typename... Params> Subsystem(Params ...params) : content(nullptr), builder(new LambdaSubsystemFactory([=] { return (T *) T::build(params...); })) {
		}
		~Subsystem() {
			if (content)
				delete content;
			delete builder;
		}
		IBLIS_IMMOVABLE(Subsystem);
		T * get() {
			if (!content)
				content = (T *) builder->build();
			return content;
		}
	private:
		SubsystemFactory * builder;
		T * content;
	};

	class HelperSys {
	public:
		HelperSys(std::string itsetupDir) : itsetupDir(itsetupDir) {}
		IBLIS_IMMOVABLE(HelperSys);
		static HelperSys * build();
		std::string itsetupDir;
		std::string helper(const char * name);
		std::string osxcrossBinLink();
		std::string w32crossSDK(const char * sdk);
	};
	extern Subsystem<HelperSys> helperSys;

	// cvar.cpp

	class Kind;

	// The Registerable linked-list is a singleton, to which things are attached.
	class Registerable {
	public:
		const char * name;
		const char * purpose;
		Registerable(const char * name, const char * purpose);
		IBLIS_IMMOVABLE(Registerable);
		virtual const char * getKind();
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

	class Act : public Registerable {
	public:
		Act(const char * name, const char * purpose);
		IBLIS_HAS_KIND;
		virtual int execute() = 0;
	};

	class Cvar : public Registerable {
	public:
		Cvar(const char * name, const char * purpose);
		IBLIS_HAS_KIND;
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

	class HelpCategory : public Registerable {
	public:
		HelpCategory(const char * name, const char * purpose);
		IBLIS_HAS_KIND;
		virtual void execute() = 0;
	};

	// version.cpp
	extern const char * version;

	// main.cpp
	int main(int argc, char ** argv);
}
