#ifndef __CpuCostStat_H__
#define __CpuCostStat_H__

/**
用于分析统计代码段总耗时
*/
#include <vector>
#include <atomic>
#include <thread>
#include "Util_func.h"

class CpuCostStat{
public:
	CpuCostStat(const char * name){
		total_cost_ms.store(0);
		m_name = name;
		last_cost = 0;
		last_cost_times = 0;
		start_in = false;
	}
	int64_t stat_start(){
		//start_in = true;
		return getMicroSysTime();
	}
	void stat_end(int64_t start);

	inline const char * getName(){
		return m_name;
	}
	inline long getCost(){
		return total_cost_ms.load();
	}
	inline long getCostTimes(){//花费的调用次数
		return total_cost_times.load();
	}

protected:
	std::atomic<long> total_cost_ms;
	std::atomic<long> total_cost_times;
	const char * m_name;
	long last_cost;
	long last_cost_times;
	bool start_in;
	friend class GCpuCostStat;
	friend class TmpCpuStart;
};

class GCpuCostStat{
public:
	static CpuCostStat * allocStat(const char * name);
	static void startDumpThread();
	static void dump();
protected:
	static std::vector<CpuCostStat *> stats;
	static std::thread * thread_dump;
};

class TmpCpuStart
{
public:
	TmpCpuStart(CpuCostStat * x)
	{
		m_x = x;
		t1 = x->stat_start();
		m_x->start_in = true;
	}
	~TmpCpuStart()
	{
		m_x->start_in = false;
	}
	CpuCostStat* m_x;
	long t1;
};

#define ALLOCCPUSTAT(x) static CpuCostStat * x = GCpuCostStat::allocStat(#x);
#define CPUSTATSTART(x) TmpCpuStart x##t1(x);
#define CPUSTATEND(x) x->stat_end(x##t1.t1);
#define ALLOCCPUANDSTART(x) ALLOCCPUSTAT(x) CPUSTATSTART(x)


#endif