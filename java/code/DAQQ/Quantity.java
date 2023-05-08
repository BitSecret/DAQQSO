package DAQQ;

import UsefulMethod.Useful;

//定量数据聚合
public class Quantity {
    double[][][] subjectsDataMatrix;
    int aCount;    //方案数量
    int sCount;    //被试数量
    int cCount;    //指标数量
    public Quantity(int AlternativeCount, int SubjectsCount, int CriteriaCount,
                               String FilePath, String[] Filename){
        aCount = AlternativeCount;
        sCount = SubjectsCount;
        cCount = CriteriaCount;
        subjectsDataMatrix = new double[aCount][sCount][cCount];
        for (int i = 0; i < aCount; i++){
            subjectsDataMatrix[i] = Useful.getMatrix(FilePath + Filename[i],
                    sCount, cCount);
        }
    }

    public void aggregation(double f){
        int[][][] rankingMatrix = new int[aCount][sCount][cCount];
        for (int i = 0; i < aCount; i++){
            for(int j = 0; j < sCount; j++){
                for (int k = 0; k < cCount; k++){
                    for (int temp = 0; temp < aCount; temp++){
                        if (subjectsDataMatrix[i][j][k] >= subjectsDataMatrix[temp][j][k]){
                            rankingMatrix[i][j][k] = rankingMatrix[i][j][k] + 1;
                        }
                    }
                }
            }
        }

        for (int i = 0; i < aCount; i++){
            System.out.println("方案" + i + "评级：");
            for(int j = 0; j < sCount; j++){
                for (int k = 0; k < cCount; k++){
                    System.out.printf("%d\t", rankingMatrix[i][j][k]);
                }
                System.out.println();
            }
            System.out.println();
        }



        for(int j = 0; j < sCount; j++){
            for (int k = 0; k < cCount; k++){
                double max = subjectsDataMatrix[0][j][k];    //求方案的最大值
                for (int i = 1; i < aCount; i++){
                    if (subjectsDataMatrix[i][j][k] > max){
                        max = subjectsDataMatrix[i][j][k];
                    }
                }
                for (int i = 0; i < aCount; i++){    //规范化
                    subjectsDataMatrix[i][j][k] = subjectsDataMatrix[i][j][k] / max;
                }
            }
        }


        for (int i = 0; i < aCount; i++){
            System.out.println("方案" + i + "规范化：");
            for(int j = 0; j < sCount; j++){
                for (int k = 0; k < cCount; k++){
                    System.out.printf("%.4f\t", subjectsDataMatrix[i][j][k]);
                }
                System.out.println();
            }
            System.out.println();
        }


        System.out.println("方案a1-a" + aCount + "聚合:");
        for (int i = 0; i < aCount; i++){
            for (int k = 0; k < cCount; k++){
                double sum = 0;
                double temp = 0;
                for (int j = 0; j < sCount; j++){
                    temp= Math.sqrt(f * Math.pow(subjectsDataMatrix[i][j][k], 2)
                                    + (1 - f) * Math.pow(1.0 * rankingMatrix[i][j][k] / aCount, 2));
                    sum += temp;
                }
                System.out.printf("%.4f\t", sum / sCount);
            }
            System.out.println();
        }
    }
}
