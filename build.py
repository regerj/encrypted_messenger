# Build script

# COMMAND FORMAT:
# py build.py --server/client/both --debug/release --build/run/clean --doc/nodoc
# ALL OPTIONS ARE REQUIRED BECAUSE I'M LAZY

import sys, os

file_list = ""
exe_name = ""
compileServer = False
compileClient = False
debug = False

def main():
    global compileServer
    global compileClient
    global debug
    
    if sys.argv[1] == "--server":
        compileServer = True
    elif sys.argv[1] == "--client":
        compileClient = True
    elif sys.argv[1] == "--both":
        compileServer = True
        compileClient = True
    else:
        print("Invalid option. \nUsage:\n\t--server - Build the server.\n\t--client - Build the client.\n\t--both - Build both client and server.")
       
    if sys.argv[2] == "--debug":
        debug = True
    elif sys.argv[2] == "--release":
        debug = False
    else:
        print("Invalid option. \nUsage:\n\t--debug - Build with debug information and dynamic.\n\t--release - Build static without debug information.")
             
    if sys.argv[3] == "--build":
        clean()
        build()
    elif sys.argv[3] == "--run":
        clean()
        build()
        run()
    elif sys.argv[3] == "--clean":
        clean()
    else:
        print("Invalid option. \nUsage:\n\tbuild - Build the code.\n\trun - Build and run the code.\n\tclean - Delete the bin folder and all files within.")
    
    if sys.argv[4] == "--doc":
        gen_doc()
    elif sys.argv[4] == "--nodoc":
        print("Warning: Documentation not generated, may be out of date!")
    else:
        print("Invalid option. \nUsage:\n\t--doc - Build documentation.\n\t--nodoc - Do not build documentation.")
    return

def build():
    global exe_name
    global file_list
    if compileClient:
        file_list = ""
        exe_name = "client.exe"
        os.chdir(".\\client")
        os.mkdir(".\\bin")
        compile("client_socket", True)
        compile("main", True)
        compile("", False)
        os.chdir("..\\")
    if compileServer:
        file_list = ""
        exe_name = "server.exe"
        os.chdir(".\\server")
        os.mkdir(".\\bin")
        compile("server_socket", True)
        compile("main", True)
        compile("", False)
        os.chdir("..\\")

def compile(cpp_file, component):
    global file_list
    global exe_name

    if component:
        os.system("echo Compiling " + cpp_file)
        os.system("g++ -std=c++11 -c -g " + cpp_file + ".cpp" + " -o " + ".\\bin\\" + cpp_file + ".o" + " -D_WIN32_WINNT=0x0A00")
        file_list += (".\\bin\\" + cpp_file + ".o ")
    else:
        #print(file_list)
        os.system("g++ -std=c++11 -g " + file_list + "-o " + ".\\bin\\" + exe_name + " -L .\\lib -lsocket_handler" + " -lws2_32" + " -D_WIN32_WINNT=0x0A00")

def gen_doc():
    os.system("doxygen .\\docs\\lovelace.doxygen")
    os.chdir(".\\client\\")
    os.chdir("..\\docs\\latex")
    os.system(".\\make")
    os.chdir("..\\..\\client\\")
        
def run():
    if compileServer:
        os.chdir(".\\server")
        os.system(".\\bin\\server.exe")
        os.chdir("..\\")
    if compileClient:
        os.chdir(".\\client")
        os.system(".\\bin\\client.exe")
        os.chdir("..\\")

def clean():
    if compileServer:
        os.chdir(".\\server\\")
        os.system("rmdir /s /q .\\bin\\")
        os.chdir("..\\")
    if compileClient:
        os.chdir(".\\client")
        os.system("rmdir /s /q .\\bin\\")
        os.chdir("..\\")

if __name__ == "__main__":
    main()