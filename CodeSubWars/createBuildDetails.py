# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

import os
import subprocess

file = open("Source/BuildDetails2.cpp", "w")

file.write("//warning: this is an automatically generated file!\n")
file.write("#include \"PrecompiledHeader.h\"\n")
file.write("#include \"BuildDetails.h\"\n")
file.write("\n")
file.write("namespace CodeSubWars\n")
file.write("{\n")

p = subprocess.Popen("git describe --always", stdout=subprocess.PIPE, shell=True)
output = p.communicate()[0].decode("utf-8") 
output = output.replace("\n", "")
file.write("  const char* BuildDetails::COMMIT_SHORT = \"" + output + "\";\n")

file.write("}\n")
file.close()


originalContent = ""
try:
  file = open("Source/BuildDetails.cpp", "r")
  originalContent = file.read()
  file.close()
except:
  pass

newContent = ""
try:
  file = open("Source/BuildDetails2.cpp", "r")
  newContent = file.read()
  file.close()
except:
  pass
  
if originalContent == newContent:
  os.remove("Source/BuildDetails2.cpp")
else:
  os.remove("Source/BuildDetails.cpp")
  os.rename("Source/BuildDetails2.cpp", "Source/BuildDetails.cpp")
  print("BuildDetails updated")
