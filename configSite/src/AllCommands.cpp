/**
 * Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
 */
#include "../lib/AllCommands.h"

/**
 * Constructor
 */
AllCommands::AllCommands() {
    this->sudo = "sudo";
    this->copy = "cp";
    this->cd = "cd";
    this->mkdir = "mkdir";
    this->del = "rm";
    this->grep = "grep";
    this->mv = "mv";
    this->cat = "cat";
    this->sed = "sed";
    this->chown = "chown";
    this->chmod = "chmod";
    this->otherFunctionsCommand = APPFOLDER + "src/otherFunctionCommands.py";
}

/**
 * Destructor
 */
AllCommands::~AllCommands() {}

/**
 * Return sudo command constructed
 */
string AllCommands::getCommandSudo(string params) {
    return this->sudo + " " + params;
}

/**
 * Return copy command constructed
 */
string AllCommands::getCommandCopy(string params) {
    return this->copy + " " + params;
}

/**
 * Return cd command constructed
 */
string AllCommands::getCommandCd(string params) {
    return this->cd + " " + params;
}

/**
 * Return mkdir command constructed
 */
string AllCommands::getCommandMkdir(string params) {
    return this->mkdir + " " + params;
}

/**
 * Return del command constructed
 */
string AllCommands::getCommandDel(string params) {
    return this->del + " " + params;
}

/**
 * Return grep command constructed
 */
string AllCommands::getCommandGrep(string params) {
    return this->grep + " " + params;
}

/**
 * Return mv command constructed
 */
string AllCommands::getCommandMv(string params) {
    return this->mv + " " + params;
}

/**
 * Return cat command constructed
 */
string AllCommands::getCommandCat(string params) {
    return this->cat + " " + params;
}

/**
 * Return sed command constructed
 */
string AllCommands::getCommandSed(string params) {
    return this->sed + " " + params;
}

/**
 * Return chown command constructed
 */
string AllCommands::getCommandChown(string params) {
    return this->chown + " " + params;
}

/**
 * Return chmod command constructed
 */
string AllCommands::getCommandChmod(string params) {
    return this->chmod + " " + params;
}

/**
 * Execute command
 */
void AllCommands::executeCommands(string command) {
    system(command.c_str());
}

/**
 * Execute command and return output
 */
string AllCommands::executeCommandsWithOutput(const char* command) {
    char buffer[128];
    string output = "";

    FILE* pipe = popen(command, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                output += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return output;
}

/**
 * Set Permission on path
 */
void AllCommands::setPermission(string path) {
    string command = "-R " + LEVELPERMISSION + " " + path;
    command = this->getCommandChmod(command);
    command = this->getCommandSudo(command);

    // Execute
    this->executeCommands(command);
}

/**
 * Check if Dir exist or not
 */
bool AllCommands::checkDirectory(string path, string nameDirectory) {
    string fullPath = "\"" + path + nameDirectory + "\"";
    string command = "./" + FUNCTIONSHELL + " -existFD " + fullPath;
    return (this->executeCommandsWithOutput(command) == "1") ? true : false;
}

/**
 * Check if File exist or not
 */
bool AllCommands::checkFile(string path, string nameFile) {
    string fullPath = "\"" + path + nameFile + "\"";
    string command = "./" + FUNCTIONSHELL + " -existFD " + fullPath;
    return (this->executeCommandsWithOutput(command) == "1") ? true : false;
}