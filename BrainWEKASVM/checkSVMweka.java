import weka.core.Instances;
import weka.classifiers.Classifier;
import weka.classifiers.Evaluation;
import weka.classifiers.functions.LibSVM;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.*;

class A{
public static void main(String a[]) throws Exception{

 BufferedReader reader = new BufferedReader(
                              new FileReader("D:/DEAP DATASET/data_made_1/emotion.arff"));

 Instances data = new Instances(reader);
 reader.close();
 data.setClassIndex(data.numAttributes() - 1);

BufferedReader reader1 = new BufferedReader(
                              new FileReader("D:/DEAP DATASET/data_made_1/testdata.arff"));

 Instances data1 = new Instances(reader1);
 reader1.close();
 data1.setClassIndex(data1.numAttributes() - 1);
//System.out.println();
PrintWriter writer = new PrintWriter("brainfile.txt", "UTF-8");

// Instances train = ...   // from somewhere
// Instances test = ...    // from somewhere
 // train classifier
//J48 tree = new J48();
//tree.buildClassifier(data);

LibSVM sv = new LibSVM();
//sv.setSVMType(new SelectedTag(LibSVM.SVMTYPE_EPSILON_SVR,LibSVM.TAGS_SVMTYPE));
sv.setCost(Math.pow(2, 2));
sv.setGamma(Math.pow(2, 1));
sv.setEps(0.00001);
Classifier c=sv;


 //Classifier cls = new J48();
 c.buildClassifier(data);
 // evaluate classifier and print some statistics
 Evaluation eval = new Evaluation(data);
 eval.evaluateModel(c, data1);
 System.out.println(eval.toSummaryString("\nResults\n======\n", false));

    
if (eval.correct()==(double)1.0){
//System.out.println("Positive");
    writer.println(data1.toString().charAt(237));
}
else{
//System.out.println("Negative");
writer.println('e');
}
    writer.close();


}
}