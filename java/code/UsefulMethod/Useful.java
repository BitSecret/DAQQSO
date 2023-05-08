package UsefulMethod;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class Useful {

    public static double[][] getMatrix(String filePath, int raw, int column){
        double[][] result = new double[raw][column];
        try{
            BufferedReader bufferedReader = new BufferedReader(new FileReader(filePath));
            String[] data;
            for (int i = 0; i < raw; i++){
                data = bufferedReader.readLine().split("\t");
                for (int j = 0; j < column; j++){
                    result[i][j] = Double.parseDouble(data[j]);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return  result;
    }

    public static double[] getMatrix(String filePath, int column){
        double[] result = new double[column];
        try{
            BufferedReader bufferedReader = new BufferedReader(new FileReader(filePath));
            String[] data = bufferedReader.readLine().split("\t");
            for (int j = 0; j < column; j++){
                result[j] = Double.parseDouble(data[j]);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return  result;
    }
}
