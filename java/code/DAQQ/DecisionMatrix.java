package DAQQ;

import UsefulMethod.Useful;

public class DecisionMatrix {
    int aCount;    //方案
    int cCount;    //指标
    double[][] dataMatrix;    //数据矩阵
    double[] weight;          //权重
    double[] indexType;       //指标类型

    double[] worst;    //AAI
    double[] best;     //AI

    public DecisionMatrix(int alternativeCount, int criteriaCount, String filePath){
        aCount = alternativeCount;
        cCount = criteriaCount;
        dataMatrix = Useful.getMatrix(filePath + "DecisionMatrix.txt", aCount, cCount);
        indexType = Useful.getMatrix(filePath + "indexType.txt", cCount);
    }

    public void normalization(){
        double[] min = new double[cCount];
        double[] max = new double[cCount];
        worst = new double[cCount];
        best = new double[cCount];

        for (int j = 0; j < cCount; j++){    //计算最小和最大值
            min[j] = dataMatrix[0][j];
            max[j] = dataMatrix[0][j];
        }
        for (int j = 0; j < cCount; j++){
            for (int i = 1; i < aCount; i++){
                if (dataMatrix[i][j] < min[j]){
                    min[j] = dataMatrix[i][j];
                }
                if (dataMatrix[i][j] > max[j]){
                    max[j] = dataMatrix[i][j];
                }
            }
        }

        for (int j = 0; j < cCount; j++){    //计算AAI和AI
            if (indexType[j] == 1){    //benefit类型
                worst[j] = min[j];
                best[j] = max[j];
            }
            else{    //cost类型
                worst[j] = max[j];
                best[j] = min[j];
            }
        }

        for (int i = 0; i < aCount; i++){    //计算Nij
            for (int j = 0; j < cCount; j++){
                if (indexType[j] == 1){    //benefit类型
                    dataMatrix[i][j] = dataMatrix[i][j] / best[j];
                }
                else{    //cost类型
                    dataMatrix[i][j] = best[j] / dataMatrix[i][j];
                }
            }
        }
    }

    public void getWeight(String filePath){
        weight = Useful.getMatrix(filePath + "weight.txt", cCount);
    }

    public void getLingoStatement(){    //输出求解权重的lingo语句
        double[] avg = new double[cCount];    //各指标平均值
        double[] sum = new double[cCount];
        double[] subjectWeight = new double[cCount];
        double[] objectWeight = new double[cCount];
        double[][] pearson = new double[cCount][cCount];
        double[] G = new double[cCount];    //Gini系数
        double[] R = new double[cCount];    //相关性

        /********客观权重********/
        for (int i = 0; i < aCount; i++){     //每一列的和
            for (int j = 0; j < cCount; j++){
                sum[j] += dataMatrix[i][j];
            }
        }

        for (int j = 0; j < cCount; j++){    //每一列的均值
            avg[j] = sum[j] / aCount;
        }

        for (int i = 0; i < cCount; i++){    //pearson相关系数
            for (int j = 0; j < cCount; j++){
                if (i == j){
                    pearson[i][j] = 1;
                }
                else{
                    double numerator = 0;
                    double denominator_x = 0;
                    double denominator_y = 0;
                    for (int k = 0; k < aCount; k++){
                        numerator += (dataMatrix[k][i] - avg[i]) * (dataMatrix[k][j] - avg[j]);
                        denominator_x += (dataMatrix[k][i] - avg[i]) * (dataMatrix[k][i] - avg[i]);
                        denominator_y += (dataMatrix[k][j] - avg[j]) * (dataMatrix[k][j] - avg[j]);
                    }
                    denominator_x = Math.sqrt(denominator_x);
                    denominator_y = Math.sqrt(denominator_y);
                    pearson[i][j] = numerator / (denominator_x * denominator_y);
                }
            }
        }

        System.out.println("Gini系数：");    //计算G
        double giniSum;
        for (int j = 0; j < cCount; j++){
            giniSum = 0;
            for (int i = 0; i < aCount; i++){
                giniSum += (dataMatrix[i][j] / sum[j]) * (dataMatrix[i][j] / sum[j]);
            }
            G[j] = 1 - giniSum;
            System.out.printf("%.4f\t", G[j]);
        }
        System.out.println();

        System.out.println("相关性：");    //计算R
        for (int j = 0; j < cCount; j++){
            for (int i = 0; i < cCount; i++){
                R[j] += pearson[i][j];
            }
            System.out.printf("%.4f\t", R[j]);
        }
        System.out.println();

        double weightSum = 0;          //计算权重
        for (int j = 0; j < cCount; j++){
            weightSum += G[j] * R[j] / (G[j] + R[j]);
        }
        for (int j = 0; j < cCount; j++){
            objectWeight[j] = G[j] * R[j] / (G[j] + R[j]) / weightSum;
        }

        System.out.println("objectWeight:");
        for (int j = 0; j < cCount; j++){
            System.out.printf("%.4f\t", objectWeight[j]);
        }
        System.out.println();


        /********主观权重********/
        for (int j = 0; j < cCount; j++){
            subjectWeight[j] = 1.0 / cCount;
        }

        System.out.println("subjectWeight:");
        for (int j = 0; j < cCount; j++){
            System.out.printf("%.4f\t", subjectWeight[j]);
        }
        System.out.println();


        /********优化语句********/
        System.out.println("\ntitle: Range Max Method;");
        System.out.println("!目标函数;");
        System.out.print("max = ");    //最大优化
        for (int i = 0; i < aCount; i++){
            System.out.print("(");
            for (int j = 0; j < cCount; j++){
                System.out.printf("(%.4f)*w%d", (dataMatrix[i][j] - avg[j]), (j + 1));
                if (j < cCount - 1){
                    System.out.print("+");
                }
            }

            if (i < aCount - 1){
                System.out.print(")^2\n+");
            }
            else{
                System.out.println(")^2;");
            }
        }

        System.out.println("!约束条件;");    //输出约束
        for (int j = 0; j < cCount; j++){
            System.out.printf("w%d", (j + 1));
            if (j < cCount - 1){
                System.out.print("+");
            }
            else{
                System.out.println("=1;");
            }
        }
        for (int j = 0; j < cCount; j++){
            System.out.printf("w%d>=%.4f;w%d<=%.4f;\n",
                    (j + 1), Math.min(subjectWeight[j], objectWeight[j]),
                    (j + 1), Math.max(subjectWeight[j], objectWeight[j]));
        }
    }

    public void MARCOS(){
        double[] S = new double[aCount + 2];    //计算S
        for (int i = 0; i < aCount; i++){
            for (int j = 0; j < cCount; j++){
                S[i] += dataMatrix[i][j] * weight[j];
            }
        }
        for (int j = 0; j < cCount; j++){
            S[aCount] += worst[j] * weight[j];
        }
        for (int j = 0; j < cCount; j++){
            S[aCount + 1] += best[j] * weight[j];
        }

        System.out.println("MRACOS决策结果：\nS\tK+\tK-\tK");
        for (int i = 0; i < aCount; i++){
            double K_Positive = S[i] / S[aCount];
            double K_Negative = S[i] / S[aCount + 1];
            double K = (K_Positive + K_Negative) / (1 + S[aCount] / S[aCount + 1] + S[aCount + 1] / S[aCount]);

            System.out.printf("%.4f\t%.4f\t%.4f\t%.4f\n", S[i], K_Positive, K_Negative, K);
        }
    }

    public void WA(){
        double[] S = new double[aCount];    //计算S
        for (int i = 0; i < aCount; i++){
            for (int j = 0; j < cCount; j++){
                S[i] += dataMatrix[i][j] * weight[j];
            }
        }

        System.out.println("加权平均结果：");
        for (int j = 0; j < aCount; j++){
            System.out.printf("%.4f\n", S[j]);
        }
    }

    public void showMatrix(){
        for (int i = 0; i < aCount; i++){
            for (int j = 0; j < cCount; j++){
                System.out.printf("%.4f\t", dataMatrix[i][j]);
            }
            System.out.println();
        }
    }

    public void showAll(){
        System.out.println("规范化决策矩阵:");
        for (int i = 0; i < aCount; i++){
            for (int j = 0; j < cCount; j++){
                System.out.printf("%.4f\t", dataMatrix[i][j]);
            }
            System.out.println();
        }

        System.out.println("指标权重:");
        for (int j = 0; j < cCount; j++){
            System.out.printf("%.4f\t", weight[j]);
        }
        System.out.println();

        System.out.println("指标类型:");
        for (int j = 0; j < cCount; j++){
            System.out.printf("%.4f\t", indexType[j]);
        }
        System.out.println();
    }
}
