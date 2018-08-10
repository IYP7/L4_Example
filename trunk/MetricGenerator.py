"""
    Python script to calculate the Metrics of the Component

    @author Albert Serrallo
"""

import os
from subprocess import Popen, PIPE, call
import argparse
import logging

parser = argparse.ArgumentParser(description='Montseny Metrics')
parser.add_argument('--nameArc', action="store", default="stm32f0xx", dest="nameArc", help="Name of the Architecture")
parser.add_argument('-o', action="store", default="default", dest="optimitzation", help="Optimitzation type")
parser.add_argument('-c', action="store", default="all", dest="componentName", help="Name of the component to calculate metric.")
parser.add_argument('--path', action="store", default=".", dest="pathArg", help="Path of the project (trunk)")
parser.add_argument('--configuration', action="store", default="Debug", dest="configuration", help="Compile Configuration")
args = parser.parse_args()

logging.basicConfig(level=logging.WARNING)


class montsenyMetric:
    ATOLLIC_BASE = "C:\\Program Files (x86)\\Atollic"
    MONLAYER = ["Architecture", "Middleware", "HAL", "Drivers", "Applications"]
    GLOBAL_FLASH = 0
    GLOBAL_RAM = 0
    GLOBAL_DATA = 0
    GLOBAL_MONTSENY = [0,0,0]
    GLOBAL_HALST= [0,0,0]
    GLOBAL_LIBR= [0,0,0]
    
    def __init__(self):
        self.logger = logging.getLogger(__name__)

    def atollicVersion(self):
        """
            Search for the any Atollic Version. 
        """
        for root,dir,files in os.walk(self.ATOLLIC_BASE):
            if "TrueSTUDIO" in root:
                self.ATOLLIC_BASE = os.path.join(self.ATOLLIC_BASE, root)
                break
        return 
        
    
    def getELFPath(self, dir):
        if os.path.exists(dir):
            for f in os.listdir(os.path.join(args.pathArg, args.configuration)):
                if f.endswith(".elf"):
                    self.logger.info("ELF file Found %s." % f)
                    return os.path.join(args.pathArg, args.configuration, f)
        self.logger.info("ELF file NOT Found on %s directory." % os.path.join(args.pathArg, args.configuration))
        return None

                
    def getAtollicNM(self):
        ATOLLIC_PATH = os.path.join(self.ATOLLIC_BASE,'ARMTools','bin','arm-atollic-eabi-nm.exe');
        p = Popen([ATOLLIC_PATH, self.getELFPath(args.pathArg), "-l", "-t","d",
                 "-r","--print-size", "--size-sort"], stdout=PIPE, stderr=PIPE)
        sizeComponent, error = p.communicate()
        return sizeComponent

        
    def getAtollicSize(self):
        """
            sizeElem [ rodata, isr, stackBss, array]
        """
        sizeElem = [0,0,0,0]
        size = 0
        ATOLLIC_PATH = os.path.join(self.ATOLLIC_BASE,'ARMTools','bin','arm-atollic-eabi-size.exe');
        p = Popen([ATOLLIC_PATH, self.getELFPath(args.pathArg), "-A","-d"], stdout=PIPE, stderr=PIPE)
        stdout, error = p.communicate()
        lines = stdout.splitlines()
        list = [lines[i].split() for i in range(len(lines))]
        for elem in list:
            try:
                if ".rodata" in elem[0]:
                    sizeElem[0] = int(elem[1])
                elif ".isr_vector" in elem[0]:
                    sizeElem[1] = int(elem[1])
                elif "._user_heap_stack" in elem[0]:
                    sizeElem[2] = int(elem[1])
                elif ".init_array" in elem[0]:
                    sizeElem[3] += int(elem[1])
                elif ".fini_array" in elem[0]:
                    sizeElem[3] += int(elem[1])
            except:
                pass
        return sizeElem       

        

    def __getComponentList(self, component, fileName):
        """
            return the list of components on the file name.
        """
        Componentlist = []
        list = [component[i].split() for i in range(len(component))]
        for num, elem in enumerate(component):
            if fileName.lower() in elem.lower():
                Componentlist.append(list[num])
            
        return Componentlist
        
        
    def parseComponentListToFile(self, component, moduleDir, fileName):
        """
            return a parsed String with the componentName
        """
        list = self.__getComponentList(component, fileName)
        self.GLOBAL_FLASH += self.getSizeFlash(list)
        self.GLOBAL_RAM += self.getSizeRAM(list)
        self.GLOBAL_DATA += self.getSizeData(list)

        string = ""
        if self.getSizeFlash(list) == 0 and self.getSizeRAM(list) == 0 and self.getSizeData(list) == 0:
            string = ""
        else:
            string += "{0:30}\t{1:6d}\t{2:6d}\t{3:6d}\n".format(moduleDir.upper(),
            self.getSizeFlash(list), self.getSizeRAM(list), self.getSizeData(list)) 
        return string
        
        
    def searchMontsenyModules(self, dirBase):
        """
            Search for components inside Montseny Architcture.
        """
        string = ""
        string += "{0:30}\t{1:6}\t{2:6}\t{3:6}\n".format('COMPONENT',' FLASH','   RAM','   DATA')
        string += "-"*80+"\n"
        
        for dir in self.MONLAYER:
            for moduleDir in os.listdir(os.path.join(dirBase, dir)):
                componentDirectory = os.path.join(dirBase, dir, moduleDir)
                metricsFolder = os.path.join(componentDirectory, "metrics")
                if not os.path.isdir(metricsFolder):
                    os.makedirs(metricsFolder)
                
                listComponents = self.getAtollicNM().splitlines()
                string+= self.parseComponentListToFile(listComponents, moduleDir, componentDirectory)
                self.writeMetricComponent(listComponents, moduleDir, componentDirectory )
        self.GLOBAL_MONTSENY = [self.GLOBAL_FLASH, self.GLOBAL_RAM, self.GLOBAL_DATA]
        return string
        
        
    def getSubTotal(self):
        string = "-"*80+"\n"

        string += "{0:30}\t{1:6d}\t{2:6d}\t{3:6}\n".format(' TOTAL MONTSENY (Bytes)',
            self.GLOBAL_MONTSENY[0], self.GLOBAL_MONTSENY[1], self.GLOBAL_MONTSENY[2])
        string += "{0:30}\t{1:6d}\t{2:6d}\t{3:6}\n".format(' TOTAL ST (Bytes)',
            self.GLOBAL_HALST[0], self.GLOBAL_HALST[1], self.GLOBAL_HALST[2])
        string += "{0:30}\t{1:6d}\t{2:6d}\t{3:6}\n".format(' TOTAL OTHERS (Bytes)',
            self.GLOBAL_LIBR[0], self.GLOBAL_LIBR[1], self.GLOBAL_LIBR[2])
        string += "-"*80+"\n"
        return string
        
    def getTotalResultMetric(self):
        string = "{0:30}\t{1:6d}\t{2:6d}".format(' TOTAL (Bytes)',
                self.GLOBAL_FLASH + self.GLOBAL_DATA, self.GLOBAL_RAM)
        string += "\n"+"-"*80+"\n"
        return string 
        
        
    def searchHALModules(self, dirBase):
        string = "-"*80+"\n"
        string += "\t\t ST LIBRARY \n"
        string += "-"*80+"\n" 
        for moduleDir in os.listdir(os.path.join(dirBase, "ST_HAL", "Src")):
            listComponents = self.getAtollicNM().splitlines()
            string += self.parseComponentListToFile(listComponents, moduleDir, moduleDir)
        string += "\n"
        
        self.GLOBAL_HALST = [self.GLOBAL_FLASH -  self.GLOBAL_MONTSENY[0], 
                             self.GLOBAL_RAM - self.GLOBAL_MONTSENY[1], 
                             self.GLOBAL_DATA -  self.GLOBAL_MONTSENY[2]]
        return string
        
    def searchOtherLibraries(self, dirBase):
        string = ""
        listComponents = self.getAtollicNM().splitlines()
        
        Componentlist = []
        list = [listComponents[i].split() for i in range(len(listComponents))]
        for num, elem in enumerate(listComponents):
            if len(list[num]) == 4:
                Componentlist.append(list[num])
        
        self.GLOBAL_FLASH += self.getSizeFlash(Componentlist)
        self.GLOBAL_RAM += self.getSizeRAM(Componentlist)
        self.GLOBAL_DATA += self.getSizeData(Componentlist)
        
        string = "-"*80+"\n"
        string += "\t\t ARM LIBRARY METRIC COMPONENT WITH %s \n" % (args.optimitzation )
        string += "-"*80+"\n"  
        string += "FLASH: \n"           
        string += self.getStringFlash(Componentlist) + "\n"
        string += "RAM: \n"  
        string += self.getStringRAM(Componentlist) + "\n"
        string += "DATA: \n"  
        string += self.getStringData(Componentlist) + "\n"
        string += "-"*80+"\n"
        string += "{0:40} = {1:6d}\n".format('TOTAL FLASH SIZE '+ "OTHER",
                    self.getSizeFlash(Componentlist))
        string += "{0:40} = {1:6d}\n".format('TOTAL RAM SIZE '+ "OTHER",
                    self.getSizeRAM(Componentlist))
        string += "{0:40} = {1:6d}\n".format('TOTAL DATA SIZE '+ "OTHER",
                self.getSizeData(Componentlist))

        self.GLOBAL_LIBR = [self.GLOBAL_FLASH -  self.GLOBAL_HALST[0] - self.GLOBAL_MONTSENY[0], 
                              self.GLOBAL_RAM - self.GLOBAL_HALST[1] - self.GLOBAL_MONTSENY[1], 
                              self.GLOBAL_DATA -  self.GLOBAL_HALST[2] - self.GLOBAL_MONTSENY[2]]
                              
        for lib in os.listdir(os.path.join(dirBase)):
            if not "ST_HAL" in lib:
                self.parseComponentListToFile(listComponents, lib, 
                    os.path.join(dirBase, lib))
        return string
        
        
    def writeMetricComponent(self, listComponent, nameComponent, directory):
        """
            Write a Metric Component
        """
        if os.path.exists(os.path.join(directory, "metrics")):
            file = open(os.path.join(directory, "metrics", nameComponent + "metric_"+args.optimitzation+'.txt'), 'w')
        else:
             file = open(os.path.join(nameComponent + "metric_"+args.optimitzation+'.txt'), 'w')
        list = self.__getComponentList(listComponent, directory)
        string = "------------------------------------------------------------------\n"
        string += "\t\t %s METRIC COMPONENT WITH %s \n"%(nameComponent.upper(), args.optimitzation )
        string += "------------------------------------------------------------------\n"  
        string += "FLASH: \n"                
        string += self.getStringFlash(list) + "\n"
        string += "RAM: \n"   
        string += self.getStringRAM(list) + "\n"
        string += "DATA: \n"  
        string += self.getStringData(list)
        string += "------------------------------------------------------------------\n"
        string += "{0:40} = {1:6d}\n".format('TOTAL FLASH SIZE '+nameComponent.upper(),
                    self.getSizeFlash(list))
        string += "{0:40} = {1:6d}\n".format('TOTAL RAM SIZE '+nameComponent.upper(),
                    self.getSizeRAM(list))
        string += "{0:40} = {1:6d}".format('TOTAL DATA SIZE '+nameComponent.upper(),
                self.getSizeData(list))
        string += "\n------------------------------------------------------------------\n"
        file.write(string)
        file.close()

        
    def writeFile(self, string):
        file = open(os.path.join(args.pathArg,"globalMetric_%s.txt" % args.optimitzation), 'w')
        file.write(string)
        file.close()
        
        
    def getSizeFlash(self, list):
        """
            return FLASH size for listComponent.
        """
        size = 0
        for elem in range(len(list)):
            if "t" in list[elem][2].lower() or "w" in list[elem][2].lower(): 
                size += int(list[elem][1])
        return size
        
    def getStringFlash(self, list):
        """
            return string with the function name and size for listComponent.
        """
        string = ""
        for elem in range(len(list)):
            if "t" in list[elem][2].lower() or "w" in list[elem][2].lower():       
                string += "{0:40} = {1:6d}\n".format(list[elem][3], int(list[elem][1]))
        return string

        
    def getSizeRAM(self, list):
        """
            return RAM size for listComponent
        """
        size = 0
        for elem in range(len(list)):
            if "b" in list[elem][2].lower(): 
                size += int(list[elem][1])
        return size 
        
        
    def getStringRAM(self, list):
        """
            return string with the function name and size for listComponent.
        """
        string = ""
        for elem in range(len(list)):
            if "b" in list[elem][2].lower():      
                string += "{0:40} = {1:6d}\n".format(list[elem][3], int(list[elem][1]))
        return string
        
        
    def getSizeData(self, list):
        """
            return DATA size for listComponent
        """
        size = 0
        for elem in range(len(list)):
            if "d" in list[elem][2].lower() or "r" in list[elem][2].lower(): 
                size += int(list[elem][1])
        return size 

        
    def getStringData(self, list):
        """
            return string with the function name and size for listComponent.
        """
        string = ""
        for elem in range(len(list)):
            if "d" in list[elem][2].lower() or "r" in list[elem][2].lower():     
                string += "{0:40} = {1:6d}\n".format(list[elem][3], int(list[elem][1]))
        return string
           
        
                
metrics = montsenyMetric()
metrics.atollicVersion()
cc = 0

listComponents = metrics.getAtollicNM().splitlines()
list = [listComponents[i].split() for i in range(len(listComponents))]

sizeTot = metrics.getAtollicSize()

for elem in range(len(list)):
    if "t" in list[elem][2].lower():
        cc += int(list[elem][1])
    if "w" in list[elem][2].lower():
        cc += int(list[elem][1])
print "Flash: %s + irq: %s = %s" % (cc+sizeTot[3], sizeTot[1], cc + sizeTot[3] + sizeTot[1])

cc = 0
for elem in range(len(list)):
    if "d" in list[elem][2].lower():
        if int(list[elem][1]) % 2 == 1:
            cc += 1
        cc += int(list[elem][1])
print "data _d: %s" % cc

cc = 0
for elem in range(len(list)):
    if "r" in list[elem][2].lower():
        if int(list[elem][1]) % 2 == 1:
            cc += 1 
        cc += int(list[elem][1])
print "data _r: %s, strings: %s" % (str(sizeTot[0]-cc + cc), sizeTot[0]-cc)

cc = 0
for elem in range(len(list)):
    if "b" in list[elem][2].lower():
        if int(list[elem][1]) % 2 == 1:
            cc += 1
        cc += int(list[elem][1])
print "RAM: %s" % str(cc + sizeTot[2])

string = ""
stringLib = ""
string += metrics.searchMontsenyModules(os.path.join(args.pathArg,"Src"))
stringLib += metrics.searchHALModules(os.path.join(args.pathArg,"libraries"))
stringLib += metrics.searchOtherLibraries(os.path.join(args.pathArg,"libraries"))
string += metrics.getSubTotal()
string += metrics.getTotalResultMetric() + "\n"
string += stringLib
metrics.writeFile(string)
