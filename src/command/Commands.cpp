#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include "../json/JSONReader.hpp"
#include "Commands.hpp"
#include "CommandBuilder.hpp"

using std::cout;
using std::cerr;
using std::endl;

void Commands::createDirectory(string path)
{
    if (!std::filesystem::create_directory(path))
    {
        cerr << "Failed to create directory! \'" << path << "\'" << endl;
    }
    else
    {
        cout << "Created \'" + path + "\'" << endl;
    }
}

void Commands::createAndWriteFile(string path, string content)
{
    // Create file and write file with supplied contents
    std::ofstream file(path);
    file << content;
    file.close();
}

void Commands::run(char const *argv[])
{
    // Get JSON data
    JsonData jsonData = JSONReader::getJsonData();

    string command;

    if (sizeof(argv) / sizeof(argv[0]) > 2)
    {
        if (strcmp(argv[2], "jar") == 0)
        {
            // Build and run commands from JSON data for the .jar file
            command = CommandBuilder::buildJavaJarCommand(jsonData);
            system(command.c_str());
            return;
        }
    }

    // Build and run commands from JSON data for the .class files
    cout << "Compiling..." << endl;
    command = CommandBuilder::buildJavacCommand(jsonData);
    system(command.c_str());

    command = CommandBuilder::buildJavaCommand(jsonData);
    system(command.c_str());
    cout << "Done!" << endl;
}

void Commands::toJar(char const *argv[])
{
    // Get JSON data
    JsonData jsonData = JSONReader::getJsonData();

    string command;

    // Compile project to .jar file
    command = CommandBuilder::buildJavacCommand(jsonData);
    system(command.c_str());
    command = CommandBuilder::buildJarCommand(jsonData);
    system(command.c_str());
    command = CommandBuilder::buildJavaJarCommand(jsonData);
    system(command.c_str());
}

void Commands::_new(char const *argv[])
{
    // Get name from arguments
    string projectName = argv[2];
    // Get new project path
    string currentPath = std::filesystem::current_path().u8string() + OS_PATH_DELIMETER + projectName;

    // Create project directories
    createDirectory(currentPath);
    createDirectory(currentPath + OS_PATH_DELIMETER + "src");
    createDirectory(currentPath + OS_PATH_DELIMETER + "src" + OS_PATH_DELIMETER + "main");
    createDirectory(currentPath + OS_PATH_DELIMETER + "lib");
    createDirectory(currentPath + OS_PATH_DELIMETER + "lib" + OS_PATH_DELIMETER + "natives");
    createDirectory(currentPath + OS_PATH_DELIMETER + "bin");

    // Create JSON file and write basic setup
    createAndWriteFile(currentPath + OS_PATH_DELIMETER + "jocoa.json", 
        "{\n\t\"projectName\": \"MyProject\",\n\t\"projectType\": \"Program\",\n\t\"sourceFiles\": \n\t[\n\t\t\"src/main/Main.java\"\n\t],\n\t\"dependencies\": [],\n\t\"natives\": \"lib/natives\",\n\t\"classpath\": \"bin\"\n}");
    // Create main Java file and write a basic program
    createAndWriteFile(currentPath + OS_PATH_DELIMETER + "src" + OS_PATH_DELIMETER + "main" + OS_PATH_DELIMETER + "Main.java", 
        "package main;\n\npublic class Main {\n\n\tpublic static void main(String[] args) {\n\n\t\tSystem.out.println(\"Hello World!\");\n\n\t}\n\n}");
}

void Commands::help(char const *argv[])
{
    cout << "Usage:\n \t help \t\t\t\t Shows this command\n \t new <projectName> \t\t Creates new Jocoa Java project\n \t run \t\t\t\t Runs current Jocoa Java project\n \t to-jar \t\t\t Packages Jocoa Java project into a .jar file" << endl;
}
