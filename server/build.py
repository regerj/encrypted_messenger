# Build script

import sys, os

file_list = ""
exe_name = "server.exe"

def main():
    try:
        if sys.argv[1] == "build":
            clean()
            build()
        elif sys.argv[1] == "run":
            clean()
            build()
            run()
        elif sys.argv[1] == "clean":
            clean()
        else:
            print("Invalid option. \nUsage:\n\tbuild - Build the code.\n\trun - Build and run the code.\n\tclean - Delete the bin folder and all files within.")
    except:
        print("Please input an option.\nUsage:\n\tbuild - Build the code.\n\trun - Build and run the code.\n\tclean - Delete the bin folder and all files within.")
    exit()

def build():
    os.mkdir(".\\bin")
    compile("socket_handler", True)
    compile("server_socket", True)
    compile("main", True)
    compile("", False)

def compile(cpp_file, component):
    global file_list
    global exe_name

    if component:
        os.system("echo Compiling " + cpp_file)
        os.system("g++ -std=c++11 -c -g " + cpp_file + ".cpp" + " -o " + ".\\bin\\" + cpp_file + ".o")
        file_list += (".\\bin\\" + cpp_file + ".o ")
    else:
        #print(file_list)
        os.system("g++ -std=c++11 -g " + file_list + "-o " + ".\\bin\\" + exe_name + " -lws2_32")

def run():
    os.system(".\\bin\\" + exe_name)

def clean():
    os.system("rmdir /s /q .\\bin\\")

if __name__ == "__main__":
    main()