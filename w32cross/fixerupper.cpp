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
