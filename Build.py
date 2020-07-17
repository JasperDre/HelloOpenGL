import os
import subprocess

def runCmd(arg):
    cmd = subprocess.Popen(arg)
    runcode = None
    while runcode is None:
        runcode = cmd.poll()

def build():
    print("Building executables")
    runCmd(['cmake', '--build', '.', '--config', 'RelWithDebInfo'])

def main():
    build()

if __name__ == "__main__":
    main()
