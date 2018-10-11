# Author: José M. C. Noronha

# Include
import os
import subprocess
import sys

class Functions():
    """
        Init
    """
    def __init__(self):
        self.openInTerminal = "x-terminal-emulator -e"
    
    """
        Only execute command
    """
    def exec_command(self, command):
        os.system(command)

    """
        Execute command and return output
    """
    def exec_command_get_output(self, command):
        process = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
        (output, error) = process.communicate()
        return output.decode("utf-8").strip("\n")
    
    """
        Check if files exists
    """
    def checkFileExist(self, file):
        return os.path.isfile(file)

    """
        Check if directories exists
    """
    def checkDirectoryExist(self, directory):
        return os.path.isdir(directory)

"""
    Main
"""
def main():
    args = sys.argv[1:]
    functions = Functions()

    if args[0] == "existPath":
        return functions.checkDirectoryExist(args[1])

if __name__ == "__main__":
    main()