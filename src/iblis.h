#pragma once

#include <stdlib.h>
#include <string.h>

/* ivorytower build initialization system */

namespace iblis {
	// util.cpp

	char * strdup_checked(const char * src);

	class CStr {
	public:
		const char * ptr;
		CStr(const char * ptr) : ptr(ptr) {}
		size_t len() const {
			return strlen(ptr);
		}
	};

	class CString : public CStr {
	private:
		explicit CString(char * give) : CStr(give) {}
	public:
		static CString takeown(char * alloc) {
			return CString(alloc);
		}
		CString(const CString & other) : CStr(strdup_checked(other.ptr)) {}
		CString(const CStr & other) : CStr(strdup_checked(other.ptr)) {}
		CString(const CStr & a, const CStr & b);
		CString & operator=(const CStr & other) {
			char * newptr = strdup_checked(other.ptr);
			free((char *) ptr);
			ptr = newptr;
			return *this;
		}
		~CString() {
			free((char *) ptr);
		}
	};

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
		const char * name;
		Cvar(const char * name, const char * purpose);
		virtual bool parse(const char * value) = 0;
		virtual CString get() = 0;
	};

	class CvarBool : public Cvar {
	public:
		bool value;
		CvarBool(const char * name, const char * purpose, bool def);
		virtual bool parse(const char * value) override;
		virtual CString get() override;
	};

	class CvarStr : public Cvar {
	public:
		CString value;
		CvarStr(const char * name, const char * purpose, CStr def);
		virtual bool parse(const char * value) override;
		virtual CString get() override;
	};

	class Component : public CvarBool {
	public:
		Component(const char * name, const char * purpose, bool def);
		virtual bool install() = 0;
	};

	// version.cpp
	extern const char * version;
}
