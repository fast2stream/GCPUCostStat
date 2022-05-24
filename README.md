# GCPUCostStat
我们在调试代码的时候经常发现cpu使用过高，特别代码庞大的时候，需要一点点的排查，此时需要一个工具类帮助我们快速定位耗时过高的代码块。本人编写了一个GCPUCostStat类，包括c++和java两种语言。
//使用非常方便
1，初始化一个统计对象
static GCPUCostStat.CpuCostStat drawStat = GCPUCostStat.allocStat("drawStat");
2，在代码段开始调用t1 = drawStat.startstat();
3，在代码段的结束调用drawStat.endstat(t1);
程序会自动打印这个代码段的耗时：
drawStat  inc 10 ms, fps  10 ms/pt
最后一次调用耗时10ms,平均每次耗时10ms.
