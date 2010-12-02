echo "<usage>: runonimage.sh <image name\image directory\list file> "
echo "          <out text file> <out image file\out image dir>"
set    InFile=%1
set    OutFile=%2
set    ImageFile=%3

set WIDTH=64
set HEIGHT=128

set BinDIR=.\bin
set Classifier=%BinDIR%\classify_rhog

set Option= -W %WIDTH%,%HEIGHT% -C 8,8 -N 2,2 -B 9 -G 8,8 -S 0 --wtscale 2 --maxvalue 0.2 --epsilon 1 --fullcirc 0 -v 3 --proc rgb_sqrt --norm l2hys 

    
set Margin=4
set ExtraOption2= -t -2.0 -m 0 
set ExtraOption1= -p 1,0 --no_nonmax 0 -z 8,16,1.3 --cachesize 128 --scaleratio 1.05 --winstride 8 --margin %Margin%,%Margin% --avsize 0,96 
set ExtraOption=%ExtraOption1% %ExtraOption2%

echo Running hog on image list
set ResultDir=%OutFile%
set CMD=%Classifier% %Option%  %ExtraOption% %InFile% %OutFile% HOG\model_4BiSVMLight.alt -i %ImageFile% 
%CMD%
