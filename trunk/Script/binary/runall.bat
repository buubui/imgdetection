REM @echo off
set WIDTH=64
set HEIGHT=128

set HardOption= --poscases 2416 --negcases 12180 

set BinDIR=.\bin
set DumpRHOG=%BinDIR%\dump_rhog
set ClassifyRHOG=%BinDIR%\classify_rhog
set dump4mc=%BinDIR%\dump4svmlearn
set svm_learn=%BinDIR%\svm_learn

set Option=  -W %WIDTH%,%HEIGHT% -C 8,8 -N 2,2 -B 9 -G 8,8 -S 0 --wtscale 2 --maxvalue 0.2 --epsilon 1 --fullcirc 0 -v 3 --proc rgb_sqrt --norm l2hys 

set ExtraOption2=  -t 0 -m 0 
set ExtraOption1=  -p 1,0 --no_nonmax 0 -z 8,16,1.3 --cachesize 128 --scaleratio 1.05 --winstride 8 --margin 4,4 --avsize 0,96 
set ExtraOption=%ExtraOption1% %ExtraOption2%
set OutDir=HOG

set OutFile=%OutDir%\record
set CMDLINE=%OutDir%\record


REM set CMD="mkdir %OutDir%"
REM %CMD%

REM get features on positive images
set CMD=%DumpRHOG% %Option% -s 1  train\pos.lst %OutDir%\train_pos.RHOG 
echo %CMD% >> %CMDLINE%
%CMD% >> %OutFile%
REM get features on negative images
set CMD=%DumpRHOG% %Option% -s 10 train\neg.lst %OutDir%\train_neg.RHOG 
echo %CMD% >> %CMDLINE%
%CMD% >> %OutFile%

REM  dump to svmdense format
set CMD= %dump4mc% -p %OutDir%\train_pos.RHOG -n %OutDir%\train_neg.RHOG %OutDir%\train_BiSVMLight.blt -v 4 
echo %CMD% >> %CMDLINE%
%CMD% >> %OutFile%

REM learn
set CMD=%svm_learn% -j 3 -B 1 -z c -v 1 -t 0 %OutDir%\train_BiSVMLight.blt %OutDir%\model_4BiSVMLight.alt 
echo %CMD% >> %CMDLINE%
%CMD% >> %OutFile%

 REM First iteration complete

REM create hard examples
set HardDir=%OutDir%\hard
REM set CMD=mkdir %HardDir%
REM echo %CMD% >> %CMDLINE%
REM %CMD% >> %OutFile%

set CMD=%ClassifyRHOG% train\neg.lst %HardDir%\list.txt %OutDir%\model_4BiSVMLight.alt -d %HardDir%\hard_neg.txt -c %HardDir%\hist.txt -m 0 -t 0 --no_nonmax 1 --avsize 0 --margin 0 --scaleratio 1.2 -l N %Option% 
echo %CMD% >> %CMDLINE%
%CMD% >> %OutFile%

echo Hard examples created

REM ## now second iteration
REM # dump hard examples
set CMD=%DumpRHOG% %Option% -s 0 %HardDir%\hard_neg.txt %OutDir%\train_hard_neg.RHOG %HardOption% --dumphard 1 --hardscore 0 --memorylimit 1700 
echo %CMD% >> %CMDLINE%
%CMD% >> %OutFile%
    
REM # dump postive, negative and hard examples
set CMD=%dump4mc% -p %OutDir%\train_pos.RHOG  -n %OutDir%\train_neg.RHOG -n %OutDir%\train_hard_neg.RHOG %OutDir%\train_BiSVMLight.blt -v 4 
echo %CMD% >> %CMDLINE%
%CMD% >> %OutFile%

REM # learn, second iteration
echo Doing second learning
set CMD=%svm_learn% -j 3 -B 1 -z c -v 1 -t 0 %OutDir%\train_BiSVMLight.blt %OutDir%\model_4BiSVMLight.alt 
echo %CMD% >> %CMDLINE%
%CMD% >> %OutFile%

echo Second iteration complete

REM ## Now test
echo Checking negative images
set ExtraOption2= -t 0 -m 0 --avsize 0 --margin 0
set ExtraOption1= -p 1 --no_nonmax 1 --nopyramid 0 --scaleratio 1.2 
set ExtraOption=%ExtraOption1% %ExtraOption2%

set ResultDir=%OutDir%\WindowTest_Negative
REM CMD=mkdir %ResultDir%
REM echo %CMD% >> %CMDLINE%
REM %CMD% >> %OutFile%
set CMD=%ClassifyRHOG% %Option%  %ExtraOption% test\neg.lst %ResultDir%\list.txt %OutDir%\model_4BiSVMLight.alt -c %ResultDir%\histogram.txt 
echo %CMD% >> %CMDLINE%
%CMD% >> %OutFile%

set ExtraOption1= -p 1 --no_nonmax 1 --nopyramid 1 
set ExtraOption= %ExtraOption1% %ExtraOption2% 
echo Checking positive images over scale space
set ResultDir=%OutDir%\WindowTest_Positive
REM CMD=mkdir %ResultDir%
REM echo %CMD% >> %CMDLINE%
REM %CMD% >> %OutFile%
set CMD=%ClassifyRHOG% %Option%  %ExtraOption% test\pos.lst %ResultDir%\list.txt %OutDir%\model_4BiSVMLight.alt -c %ResultDir%\histogram.txt 
echo %CMD% >> %CMDLINE%
%CMD% >> %OutFile%

