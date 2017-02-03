The [http://www.squirrel-lang.org/ squirrel scripting language] seeks to address some problems of lua language and offer a C like syntax with some god extras but it lacks a source code repository and some facilities provided by default in lua like string.gsub, string.gmatch, io.lines, modules, ..., this project try to solve it and extend the language to make it even more useful.

This project is not endorsed by Squirrel or Lua authors, it's an independent effort, and it will try to cooperate with both.

*Some modules, third party code, or sample code can have different licenses !!!*

It is the base of a cross platform application server (windows, macosx, linux, android, wince, http/javascript, ...)

~~A preview of the application server running on an Android phone (Huawei U8105 IDEOS on an ADSL line) can be seem here:~~

-A simple search page for companies in Reading UK http://companies-uk.unolotiene.com/  or https://companies-uk.unolotiene.com/

-A Small business application http/javascript interface http://ourbiz.unolotiene.com/ or https://ourbiz.unolotiene.com/ when asked use username: mingote password: tr14pink

Also you'll find several changes and additions to the original Squirrel 3.0.4:

  * Lua gsub and gmatch similar functionality for strings.
  * Lua gsub and gmatch similar functionality for Squirrel::regexp.
  * New api functions to make easy and less verbose write extensions.
  * A [http://www.zeromq.org/ zmq3] module.
  * A [http://www.sqlite.org sqlite3] module.
  * A [http://www.postgresql.org postgresql] module.
  * A [http://www.mysql.com/ mysql] module.
  * A start point of a [http://www.fltk.org FLTK] gui module.
  * A script to generate {{{SquiLu}}} gui code from [http://www.fltk.org/doc-1.3/fluid.html FLUID] files.
  * [http://www.fpdf.org/ FPDF] module using a c++ port.
  * A webserver module based on [http://code.google.com/p/mongoose/ mongoose].
  * A SSL module based on [http://axtls.sourceforge.net/ axTLS].
  * A template module ([https://github.com/LuaDist/mixlua port of MixLua]).
  * A port of [http://keplerproject.github.com/luafilesystem/ luafilesystem].
  * A port of [http://w3.impa.br/~diego/software/luasocket/ luasocket].
  * A "Decimal" module using [http://www.bytereef.org/mpdecimal/index.html libmpdecimal]. 
  * A XML module based on [https://github.com/leethomason/tinyxml2 tinyxml2].
  * A simple zlib module based on [http://www.zlib.net/ zlib].
  * A rs232 port of [https://github.com/ynezz/librs232 librs232].
  * A bigger subset of C/C++ language syntax (variable/functions/class/struct declarations)

Changes to the Squirrel syntax:

  * Accepts "->" as alias for "."
  * Accepts "var"/"auto" as alias for "local"
  * Accepts lua literal string delimiters "[===[" and "]===]", also "{==={" and "}===}" or "(===(" and ")===)"
  * The "new" keyword is ignored so lines like in C++/Javascript are accepted " var obj = new {{{MyClass();}}}"
  * Local scope for "const/enum", checking for redeclaration on the same compilation unit.

New functions
  * Exposed delegates for string, class, array, numbers.
  * Added new function "array.concat" with similar functionality of lua table.concat.
  * Added "write" and "read" functions to streams (only works with ascii/utf8).
  * Blob now can be used as stringbuffer.
  * Script functions "getstacktop", "get_last_stackinfo" to help debug extensions.
  * Script function print renamed to "print1" and a new "print" that behaves like the Lua print function.
  * Script function "str_from_chars" to allow construct a string from a list of integers like the Lua function "string.chars".
  * New method "concat" for array that behave like the Lua table.concat.
  * New method "bsearch" for array (only work on sorted arrays).
  * New method "get(key, default_value)" for tables that allows to query table keys and return a default value when the key doesn't exists.
  * ...

Internals:

  * *Incompatibility the vargv array allways has the script filenanme at index 0 and allways will have "len()" at minimum equal 1 (2013-02-26)*.
  * Has a modification that allows release hook handlers access the vm normally, in fact a pointer to the vm is passed to then.
  * Added a new member to SQRegFunction that indicates if a method is static.
  * Implemented detection of local variable redeclaration on the same scope.
  * Implemented compiler warnings for local redeclarations on nested scopes, also assignments inside expressions (if/while/do-while,...)
  * Implemented command line "-s" to generate a squilu code representing the generated bytecode.
  * Function parameters and function return type can be specified like in typescript, the compiler is not enforcing it yet but accepts and discards for now.
{{{
function Add(left: number, right: number): number {
        return left + right;
}
}}}
  * Class fields can be specified like in typescript, the compiler is not enforcing it yet but accepts and discards for now.
{{{
class Person {
	public name: string;
	private age: number;
	postcode: string;
	address = null;
 
	constructor(name: string, age: number) {
		this.name = name;
		this.age = age;
		postcode = "none";
		address = "unknown";
	}
 
	function asString(): string {
		return this.name + " (" + this.age + ")";
	}
}
}}}
  * ...

