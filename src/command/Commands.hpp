#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#if defined(_WIN32) || defined(__WINDOWS__)
    #define OS_PATH_DELIMETER "\\"
#elif defined(__unix__) || defined(__linux__)
    #define OS_PATH_DELIMETER "/"
#endif

/*
Commands:
 - run --jar
 - to-jar
 - new (create new project)
 - clean-up (clean any unused libraries and files)
 - help (shows list of commands)
 - add-library <name> <name> ... (add packages from maven repo)
 - remove-library <name> <name> ... (remove packages from local project)
 - search-library <name> (search packages from maven repo)
*/

/*
Java Commands:
 - javac -d <classfiles dir> -cp .;1.jar;2,jar dir1/*.java dir2/*.java
 - java -cp .;<classfiles>;1.jar;2.jar -Djava.library.path=<dll dir> main/Main
*/

class Commands
{
private:
    static void createDirectory(string path);
public:
    static void run(char const *argv[]);
    static void toJar(char const *argv[]);
    static void _new(char const *argv[]);
    static void cleanUp(char const *argv[]);
    static void help(char const *argv[]);
    static void addLibrary(char const *argv[]);
    static void removeLibrary(char const *argv[]);
    static void searchLibrary(char const *argv[]);
};


#endif