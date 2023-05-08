package DAQQ;

public class PLT {
    int t;    //术语下标的上限
    public double[] pro;    //概率

    public PLT(int TermCount, double[] TermData) {
        t = TermCount;
        pro = new double[(t + 1) * 2];
        System.arraycopy(TermData, 0, pro, 0, (t + 1) * 2);
    }

    public PLT(int TermCount) {
        t = TermCount;
        pro = new double[(t + 1) * 2];
    }

    public void show() {
        System.out.print("{");
        boolean isPrint = false;
        for (int i = 0; i < (t + 1) * 2; i = i + 2) {
            if (pro[i] != 0 || pro[i + 1] != 0) {
                if (isPrint) {
                    System.out.print(",");
                }
                if (pro[i] == pro[i + 1]) {
                    System.out.printf("S%d(%.4f)", i / 2, pro[i]);
                } else {
                    System.out.printf("S%d[%.4f,%.4f]", i / 2, pro[i], pro[i + 1]);
                }
                isPrint = true;
            }
        }
        System.out.println("}");
    }

    public void getLingoForScore(){
        System.out.println("title: Score Max/Min Method;");
        System.out.println("!目标函数;");
        System.out.print("max/min = (");
        for (int i = 1; i <= t; i++){
            System.out.print(i + "*" + "p" + i);
            if (i != t){
                System.out.print("+");
            }
            else{
                System.out.println(")/" + t + ";");
            }
        }
        System.out.println("!约束条件;");
        for (int i = 0; i < (t + 1) * 2; i = i + 2) {
            if (pro[i] == pro[i + 1]) {
                System.out.printf("p%d=%.4f;\n", i / 2, pro[i]);
            } else {
                System.out.printf("p%d>=%.4f;p%d<=%.4f;\n", i / 2, pro[i], i / 2, pro[i + 1]);
            }
        }
        for (int j = 0; j <= t; j++){
            System.out.printf("p%d", j);
            if (j != t){
                System.out.print("+");
            }
            else{
                System.out.println("=1;");
            }
        }
        System.out.println();
    }
}
