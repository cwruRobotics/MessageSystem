#Input: python MessageCodeGenerator.py "input directory" "output directory"
#ex: python MessageCodeGenerator.py "C:\Example Input Directory" "C:\Example Output Directory"

import sys
import os

inputRoot = sys.argv[1]
outputRoot = sys.argv[2]

for file in os.listdir(inputRoot):
    if(file.endswith(".xml")):

        #contains string to write to file
        toWrite = ""
        fileObject = open(file)
        contents = fileObject.read()
        outputFilename = ""

        i = 0
        while(file[i] != "."):
            outputFilename += file[i]
            i += 1
        outputFilename += ".cpp"
        outputFile = open(outputRoot + "\\" + outputFilename, 'w')

        #inner struct ares constant and outer one isn't
        outerStructUsed = False
        i = 0
        currentArg = ""
        while(i < len(contents)):
            if(contents[i] == "<"):
                currentCommand = ""
                j = 0
                while(j < len(currentArg) and currentArg[j] != ">"):
                    currentCommand += currentArg[j]
                    j += 1

                #if it is a struct brace needed after non command part
                isStruct = False
                if(currentCommand == "<struct"):
                    toWrite += "typedef "
                    if(outerStructUsed):
                        toWrite += "const "
                    toWrite += "struct "
                    isStruct = True
                    outerStructUsed = True
                elif(currentCommand == "</struct"):
                    toWrite += "}\n"
                elif(currentCommand == "</varName"):
                    toWrite += ";\n"
                elif(currentCommand == "<dataType"):
                    toWrite += "const "

                j += 1
                while(j < len(currentArg)):

                    #ignore tabs, new lines
                    if(not(currentArg[j] == "\n" or currentArg[j] == "\t")):   
                        toWrite += currentArg[j]
                    j += 1
                toWrite += " "
                if(isStruct):
                    toWrite += "{\n"
                currentArg = ""
            currentArg += contents[i]
            i += 1
        outputFile.write(toWrite)
        outputFile.close()
