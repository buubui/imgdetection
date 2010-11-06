'''
Created on Nov 5, 2010

@author: Buu Bui
'''
#import psyco
#psyco.full()
import subprocess
import sys
hardList=[]
hardListNew=[]
def appendHardList(trainFile, hardList):
    f=open(trainFile,'a')
    f.write(''.join(hardList))
    f.close()
    
def compressHardList(hardList):
    for hardEx in hardList:
        parts = hardEx.split('#')
        if len(parts)==2:
            hardEx=parts[1]
            
def checkHardExOccur(hardEx,hardList):
    fInfo = hardEx.split('#')[1]
    for hard in hardList:
        i = hard.find(fInfo)
        if i !=-1: 
            return True
    return False
        
def getHardList(fileresult, fileData,hardList,hardListNew):
    
    inResult=open(fileresult)
    print inResult
    inData = open(fileData)
    
    i=0
    lineResult=''
    lineData=''
    while(True):
        lineResult = inResult.readline()
#        //    getline(inList,lineList);
        if len(lineResult)<1:
            break
        lineData = inData.readline()
            
        if lineData[0]=='+':
            d=float(lineResult)
            if d<0:
                if checkHardExOccur(lineData, hardList)==False:
                    hardList.append(lineData)
                    hardListNew.append(lineData)  
                    i+=1
        elif lineData[0]=='-':
            d=float(lineResult)
            if d>0:
                if checkHardExOccur(lineData, hardList)==False:
                    hardList.append(lineData)
                    hardListNew.append(lineData) 
                    i+=1

    inData.close()
    inResult.close()
    return i
#    outHard=open("hard.txt",'w')
#    outHard.write(''.join(hardList))
#    outHard.close()
#AutoSVM numVal
def writeHardList(hardList, filename,isAppend): 
    mode ='w'
    if isAppend==True:
        mode='a'
    f = open(filename,mode)
    f.write(''.join(hardList))
    
    
if __name__ == '__main__':
    n=1
    if len(sys.argv)>1:
        n=int(float(sys.argv[1]))
  #  f = open('fname')
  #  lines = f.readlines()
    
    valList=[]
    for i in range(0,n):
        valList.append(['val_'+str(i)+'.txt',False])
    print valList
#    i=getHardList(r'D:\Lectures\Luan_van\tool\svm_light_windows\classify_result1.txt', r'D:\Lectures\Luan_van\tool\svm_light_windows\val_1.txt',hardList)
#    writeHardList(hardList, 'hardlist.txt')
    iTime = 0
    needTrain = True
    svmLearnCmd=r'bin/svm_learn.exe '
    isDone=0
    while(isDone<len(valList)):
        if(len(hardListNew)>0):
            appendHardList('train.txt', hardListNew)
            hardListNew =[]
        print len(valList)
        svmLearnArgs = r'train.txt'+r' model_'+ str(iTime) +'.txt'
        output=''
        if needTrain==True:
            output,error=subprocess.Popen(svmLearnCmd + svmLearnArgs,stdout = subprocess.PIPE).communicate()
            needTrain=False
            f=open('learn_out_'+str(iTime)+'.txt','w')
            f.write('\n'.join(output.splitlines()))
            f.close()
        for i in range(0,len(valList)):
            if valList[i][1]==True:
                continue
            svmClassifyCmd=r'bin/svm_classify.exe '
            svmClassifyArgs = valList[i][0]+ ' model_'+str(iTime)+'.txt ' + ' classify_result_' + valList[i][0]+ '_'+ str(iTime)+'.txt '
            output,error=subprocess.Popen(svmClassifyCmd + svmClassifyArgs,stdout = subprocess.PIPE).communicate()
            f=open('classify_out_'+valList[i][0]+ '_'+ str(iTime)+'.txt','w')
            f.write('\n'.join(output.splitlines()))
            f.close()
            hardNum=0
            hardNum=getHardList('classify_result_' + valList[i][0]+ '_'+ str(iTime)+'.txt', valList[i][0],hardList,hardListNew)
            if hardNum==0:
                valList[i][1]=True
                isDone +=1
                
        needTrain =True
        iTime +=1
        if iTime==0:
            writeHardList(hardListNew, 'hardlist.txt',False)
        else:
            writeHardList(hardListNew, 'hardlist.txt',True)
        compressHardList(hardList)
   # os.system()
#    print i
#    line='3772:0.457411         #crop001002a.png     (16, 16, 64, 128)'
#    parts=line.split('#')
#    print line.find(parts[1])
#    parts = parts[len(parts)-1].split('(')
#    print parts
    pass