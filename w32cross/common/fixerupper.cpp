// The fixerupper is what turns the produced SDK into something that will parse on a Linux system with things like case sensitivity.
// To audit the include files, I used:
//  `grep -r "\.[hH][\"\>]" | less`
//   notes: pattern seems to be #[ ]include, can this be denied?
//  `grep -r "\.[hH][\"\>] | grep -v "#[ ]*include"`
//   notes: `import "wsdxmldom.h";` exists in IDL. should probably also assume IDL is being included/imported
//  `grep -ri "\.idl[\"\>]" | less`
//   notes: so, yeah. "import" is a magic word, as is "importlib". both should result in lowering INSIDE IDL (only occurred in *.idl files)
//  `grep -ri "import" um/*.idl | less`
//   notes: so, "import" is only meaningful when it is the first word on the line
//  `grep -r "\.[hH][\"\>]" | grep cpp_quote`
//   notes: making sure that proposed changes won't break anything'
// the results:
//  for both IDL and C, the "#[ ]*include" pattern should lowercase the whole line.
//  for IDL specifically, "[ \t]*import " and "[ \t]*importlib" should lowercase the whole line.

#include <ctype.h>
#include <stdio.h>
#include <regex.h>

#include <fstream>
#include <filesystem>
#include <vector>

#define vec std::vector
namespace fs = std::filesystem;

void str2lower(std::string & thing) {
	for (size_t i = 0; i < thing.size(); i++)
		thing[i] = tolower(thing[i]);
}

// In an ideal world, we wouldn't use symlinking here, we'd just change the case.
// However, code which we *haven't* run a fixerupper pass on may assume 'vanilla' case.
// So this approach takes a 'do no harm' stance to such code.
// If they pick the upstream name, we let them have it.
// If they pick MinGW-style names (always lowercase), we let them have that.
// But we can't generate literally every combination of case. Such code will need to be passed over with fixerupper.
void linkLowerNames(const vec<fs::path> & paths, bool wouldBeDirSymlink) {
	for (auto & path : paths) {
		auto orig_filename = path.filename();
		auto fix_filename = orig_filename.u8string();
		str2lower(fix_filename);
		auto fix_path = path;
		fix_path.replace_filename(fix_filename);
		// printf("%s -> %s\n", path.c_str(), adjusted.c_str());
		if (fix_path != path) {
			if (fs::exists(fix_path))
				continue;
			if (wouldBeDirSymlink) {
				fs::create_directory_symlink(orig_filename, fix_path);
			} else {
				fs::create_symlink(orig_filename, fix_path);
			}
		}
	}
}

regex_t regexCLower, regexIDLLower;

void fixupFile(const fs::path & path) {
	auto detectref = path.filename().u8string();
	size_t lastIdx = detectref.find_last_of('.');
	bool isC = false;
	bool isIDL = false;
	if (lastIdx != std::string::npos) {
		auto ext = detectref.substr(lastIdx);
		str2lower(ext);
		// control.odl is a good one to look at
		if (ext == ".idl" || ext == ".odl")
			isIDL = true;
		if (ext == ".c" || ext == ".h" || ext == ".cc" || ext == ".cxx" || ext == ".cpp" || ext == ".hxx" || ext == ".hpp")
			isC = true;
	}
	if (isC || isIDL) {
		// We only mess with the file if it's a C or IDL file.
		// puts("WOULD MESS WITH FILE");
		// Read entire file and process each individual line.
		vec<std::string> lines;
		{
			auto reader = std::ifstream(path);
			std::string line;
			while (std::getline(reader, line)) {
				bool shouldLower = false;
				if (isC)
					shouldLower = !regexec(&regexCLower, line.c_str(), 0, nullptr, 0);
				else if (isIDL)
					shouldLower = !regexec(&regexIDLLower, line.c_str(), 0, nullptr, 0);
				if (shouldLower)
					str2lower(line);
				lines.push_back(line);
			}
		}
		auto writer = std::ofstream(path);
		for (auto const & line : lines) {
			writer << line << "\n";
		}
		writer.close();
	}
}

void fixupNode(const fs::path & path) {
	vec<fs::path> adjDirs;
	vec<fs::path> adjFiles;
	for (auto const& dirent : fs::directory_iterator{path}) {
		if (dirent.is_symlink()) {
			// we ignore symlinks. we make symlinks
			continue;
		} else if (dirent.is_directory()) {
			adjDirs.push_back(dirent.path());
		} else {
			adjFiles.push_back(dirent.path());
		}
	}
	linkLowerNames(adjDirs, true);
	linkLowerNames(adjFiles, false);
	for (auto const& path : adjFiles)
		fixupFile(path);
	for (auto const& path : adjDirs)
		fixupNode(path);
}

int main(int argc, char ** argv) {
	regcomp(&regexCLower, "#[ \t]*include", REG_EXTENDED | REG_ICASE);
	regcomp(&regexIDLLower, "^[ \t]*import(lib| )|#[ \t]*include", REG_EXTENDED | REG_ICASE);
	puts("W32Cross Fixerupper");
	puts("Creates lowercase symlinks and lowercases C, C++, and IDL files");
	if (argc != 2) {
		printf("%s DIRPATH", argv[0]);
		return 1;
	}
	fixupNode(fs::path(argv[1]));
}
