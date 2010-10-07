@ECHO OFF
echo "Learning Start..."
svm_learn train.txt model.txt > learn_out.txt 
echo "Learning Done..."
echo "Classifying Start..."
svm_classify val.txt model.txt classify_result.txt > classify_out.txt 
echo "Classifying Done..."
type classify_out.txt
echo "Calculating hyperplane weights..."
perl svm2weight.pl model.txt > svm2weight.txt
echo "Done..."