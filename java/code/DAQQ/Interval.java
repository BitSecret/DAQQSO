package DAQQ;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

//区间数，为了求解PLTS的评分
public class Interval {
    int count;
    double[][] data;
    public Interval(String filename){
        try{
            BufferedReader bufferedReader = new BufferedReader(new FileReader(filename));
            String[] fileData;
            count = Integer.parseInt(bufferedReader.readLine());
            data = new double[count][2];
            for (int i = 0; i < count; i++){
                fileData = bufferedReader.readLine().split("\t");
                data[i][0] = Double.parseDouble(fileData[0]);
                data[i][1] = Double.parseDouble(fileData[1]);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void showScore(){
        System.out.println("区间数评分为：");
        for (int i = 0; i < count; i++){
            System.out.printf("%.4f\n", (2 + 3 * data[i][0] - data[i][1]) / 4);
        }
    }
}
