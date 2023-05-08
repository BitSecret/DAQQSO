import DAQQ.DecisionMatrix;
import DAQQ.Interval;
import DAQQ.PLTS;
import DAQQ.Quantity;

import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);    //控制输入的

        String filePath = "F:\\data-set\\DAQQ\\";    //数据保存的路径
        System.out.print("输入数据文件夹路径，如 C:\\data-set\\DAQQ\\ :");
        filePath = scanner.nextLine();
        System.out.print("请将数据文件保存至 " + filePath + " 输入任意数字然后按下回车继续:");
        String line = scanner.nextLine();
        DecisionMatrix decisionMatrix;
        for (int i = 0; i < 5; i++){
            if (i == 0){
                System.out.println("\n\n***************************定性数据聚合***************************");
                aggregationPLTS(filePath + "rawData\\");    //聚合被试定性和定量数据
                System.out.print("\n请将求解优化模型得到的区间值形式的定性评价保存到文件，输入任意数字然后按下回车继续:");
                line = scanner.nextLine();
            }
            else if (i == 1){
                System.out.println("\n\n***************************区间值转化为评分***************************");
                getIntervalScore(filePath);
                System.out.print("\n请将区间的评分保存到决策矩阵文件，输入任意数字然后按下回车继续:");
                line = scanner.nextLine();
            }
            else if (i == 2){
                System.out.println("\n\n***************************定量数据聚合***************************");
                System.out.print("\n请输入定量数据聚合参数φ的值：");
                double f = Double.parseDouble(scanner.nextLine());
                aggregationQuantity(filePath + "rawData\\", f);
                System.out.print("\n请将聚合结果保存到决策矩阵文件，输入任意数字然后按下回车继续:");
                line = scanner.nextLine();
            }
            else if (i == 3){
                System.out.println("\n\n***************************权重求解***************************");
                decisionMatrix = new DecisionMatrix(4, 13, filePath);
                decisionMatrix.normalization();

                decisionMatrix.getLingoStatement();
                System.out.print("\n\n请将求解的权重保存到权重文件，输入任意数字然后按下回车继续:");
                line = scanner.nextLine();
            }
            else{
                System.out.println("\n\n***************************MARCOS方法***************************");
                decisionMatrix = new DecisionMatrix(4, 13, filePath);
                System.out.println("原始决策矩阵:");
                decisionMatrix.showMatrix();
                decisionMatrix.normalization();    //规范化
                decisionMatrix.getWeight(filePath);
                decisionMatrix.showAll();
                decisionMatrix.MARCOS();    //MRACOS方法
                //decisionMatrix.WA();
            }
        }

        System.out.print("\n\n输入任意数字然后按下回车退出程序:");
        line = scanner.nextLine();
    }

    public static void aggregationPLTS(String filePath){
        String[] PLTSFilename = {"plts_a1.txt", "plts_a2.txt", "plts_a3.txt", "plts_a4.txt"};    //被试定性数据
        PLTS plts_a1 = new PLTS(20, 5, filePath + PLTSFilename[0]);
        PLTS plts_a2 = new PLTS(20, 5, filePath + PLTSFilename[1]);
        PLTS plts_a3 = new PLTS(20, 5, filePath + PLTSFilename[2]);
        PLTS plts_a4 = new PLTS(20, 5, filePath + PLTSFilename[3]);
        System.out.println("定性评价聚合结果：");
        plts_a1.PLTSWA().show();
        plts_a2.PLTSWA().show();
        plts_a3.PLTSWA().show();
        plts_a4.PLTSWA().show();
        System.out.println("定性数据评分Lingo求解语句：");
        plts_a1.PLTSWA().getLingoForScore();
        plts_a2.PLTSWA().getLingoForScore();
        plts_a3.PLTSWA().getLingoForScore();
        plts_a4.PLTSWA().getLingoForScore();
    }

    public static void getIntervalScore(String filePath){
        String intervalsFilename= "intervals.txt";    //PLTS转化成的区间数
        Interval interval = new Interval(filePath + intervalsFilename);
        interval.showScore();
    }

    public static void aggregationQuantity(String filePath, double f){
        String[] subjectsFilename = {"a1.txt", "a2.txt", "a3.txt", "a4.txt"};    //被试定量数据
        Quantity quantity = new Quantity(4, 20, 13,
                filePath, subjectsFilename);
        quantity.aggregation(f);
    }
}
