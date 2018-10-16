/**
 * Create By José M. C. Noronha
 * Comando: g++ -Wall config.cpp -o config
 * nomestring.c_str() -> converte uma string em char c
 */
#include "../lib/AllOperationGlobal.h"

/**
 * Constructor
 */
AllOperationGlobal::AllOperationGlobal() {
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
AllOperationGlobal::~AllOperationGlobal() {}

/**
 * Return sudo command constructed
 */
string AllOperationGlobal::getCommandSudo(string params) {
    return this->sudo + " " + params;
}

/**
 * Return copy command constructed
 */
string AllOperationGlobal::getCommandCopy(string params) {
    return this->copy + " " + params;
}

/**
 * Return cd command constructed
 */
string AllOperationGlobal::getCommandCd(string params) {
    return this->cd + " " + params;
}

/**
 * Return mkdir command constructed
 */
string AllOperationGlobal::getCommandMkdir(string params) {
    return this->mkdir + " " + params;
}

/**
 * Return del command constructed
 */
string AllOperationGlobal::getCommandDel(string params) {
    return this->del + " " + params;
}

/**
 * Return grep command constructed
 */
string AllOperationGlobal::getCommandGrep(string params) {
    return this->grep + " " + params;
}

/**
 * Return mv command constructed
 */
string AllOperationGlobal::getCommandMv(string params) {
    return this->mv + " " + params;
}

/**
 * Return cat command constructed
 */
string AllOperationGlobal::getCommandCat(string params) {
    return this->cat + " " + params;
}

/**
 * Return sed command constructed
 */
string AllOperationGlobal::getCommandSed(string params) {
    return this->sed + " " + params;
}

/**
 * Return chown command constructed
 */
string AllOperationGlobal::getCommandChown(string params) {
    return this->chown + " " + params;
}

/**
 * Return chmod command constructed
 */
string AllOperationGlobal::getCommandChmod(string params) {
    return this->chmod + " " + params;
}

/**
 * Execute command
 */
void AllOperationGlobal::executeCommands(string command) {
    system(command.c_str());
}

/**
 * Execute command and return output
 */
string AllOperationGlobal::executeCommandsWithOutput(const char* command) {
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
void AllOperationGlobal::setPermission(string path) {
    string command = "-R " + LEVELPERMISSION + " " + path;
    command = this->getCommandChmod(command);
    command = this->getCommandSudo(command);

    // Execute
    this->executeCommands(command);
}

/**
 * Check if Dir exist or not
 */
bool AllOperationGlobal::checkDirectory(string path, string nameDirectory) {
    string fullPath = "\"" + path + nameDirectory + "\"";
    string command = "./" + FUNCTIONSHELL + " -existFD " + fullPath;
    return (this->executeCommandsWithOutput(command.c_str()) == "1") ? true : false;
}

/**
 * Check if File exist or not
 */
bool AllOperationGlobal::checkFile(string path, string nameFile) {
    string fullPath = "\"" + path + nameFile + "\"";
    string command = "./" + FUNCTIONSHELL + " -existFD " + fullPath;
    return (this->executeCommandsWithOutput(command.c_str()) == "1") ? true : false;
}

/**
 * Convert intinger to string
 */
string AllOperationGlobal::intToString(int integerToString) {
	// get Int to convert to string
	stringstream ssInt;
	ssInt << integerToString;
	return ssInt.str();
}

/**
 * Clear cin
 */
void AllOperationGlobal::clearCin(){
	cin.clear();
    cin.ignore(256,'\n');
}

/**
 * Get only number from cin
 */
int AllOperationGlobal::getOlyInteger(string errorMessage) {
	int integer;
	cin >> integer;

	// ask when user not introduce integer
    while(cin.fail()) {
        cout << errorMessage;
        clearCin();
        cin >> integer;
    }
    return integer;
}