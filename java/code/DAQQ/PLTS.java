package DAQQ;

import UsefulMethod.Useful;

/*  区间概率语言术语集的相关内容
1.只处理语言确定形式的PLTS
2.提供输出show()
3.提供聚合函数aggregation()
4.提供求解最大最小评分的Lingo语句
* */
public class PLTS {
    int count;    //PLT的数量
    int t;        //术语下标的上限
    PLT[] data;   //储存PLT

    public PLTS(int Count, int T, String filePath){
        count = Count;
        t = T;
        data = new PLT[count];

        double[][] temp = Useful.getMatrix(filePath, count, (t + 1) * 2);

        for (int i = 0; i < count; i++){
            data[i] = new PLT(t, temp[i]);
        }
    }

    public PLT PLTSWA(){
        PLT result = new PLT(t);

        for (int i = 0; i < count; i++){
            for (int j = 0; j < (t + 1) * 2; j++){
                result.pro[j] += data[i].pro[j];
            }
        }

        for (int j = 0; j < (t + 1) * 2; j++){
            result.pro[j] /= count;
        }

        return result;
    }

    public void show(){
        for (int i = 0; i < count; i++){
            data[i].show();
        }
    }
}

