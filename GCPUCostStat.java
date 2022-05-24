package com.mymediaplayer;

import java.util.ArrayList;

public class GCPUCostStat {
    public static class CpuCostStat{
        protected CpuCostStat(String name){
            this.name = name;
        }
        String name;
        public long startstat(){
            return System.nanoTime() / 1000;//ns转us
        }
        public void endstat(long t1){
            long t2 = startstat();
            long cost = t2 - t1;
            synchronized (this){
                totalcost += cost;
            }
            totalpt++;
        }
        long totalcost = 0;
        long totalpt = 0;
        long last_totalcost = 0;
        long last_totalpt = 0;
    }
    static GCPUCostStat stat = new GCPUCostStat();
    static ArrayList<CpuCostStat> list = new ArrayList<>();
    static Thread thread = null;
    public static GCPUCostStat getInstance(){
        return stat;
    }
    public static CpuCostStat allocStat(String name){
        System.out.println("cpu cost alloc stat " + name);
        synchronized (stat) {
            CpuCostStat ret = new CpuCostStat(name);
            list.add(ret);
            return ret;
        }
    }
    static{
        startDumpThread();
    }

    public static void dump(){
        synchronized (stat){
            for(CpuCostStat s:list){
                long discost = (s.totalcost - s.last_totalcost);
                discost /= 1000;//us转ms
                long dispt = (s.totalpt - s.last_totalpt);
                s.last_totalcost = s.totalcost;
                s.last_totalpt = s.totalpt;
                if(dispt <= 0){
                    System.out.println(s.name + " cost 0");
                }
                else {
                    System.out.println(s.name + ",inc=" + discost + "ms" +",fps=" + (discost / dispt) + "ms/pt");
                }

            }
        }
    }

    public static void startDumpThread(){
        if(thread == null){
            thread = new Thread(new Runnable() {
                @Override
                public void run() {
                    while (!Thread.currentThread().isInterrupted()) {
                        System.out.println("GCpuCostStat dump");
                        dump();
                        try {
                            Thread.sleep(1000);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }
                }
            });
            thread.start();
        }
    }

}
