import os
import subprocess

def runCmd(arg):
    cmd = subprocess.Popen(arg)
    runcode = None
    while runcode is None:
        runcode = cmd.poll()

def submodule():
    print("Initializing submodules and dependencies")
    runCmd(['git', 'submodule', 'init'])
    runCmd(['git', 'submodule', 'update'])

def generate():
    print("Generating solution")
    runCmd(['mkdir', 'build'])
    runCmd(['cd', 'build'])
    runCmd(['cmake', '-G', '"Visual Studio 16 2019"', '-Ax64', '..'])

def main():
    submodule()
    generate()

if __name__ == "__main__":
    main()
