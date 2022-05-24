#include "CpuCostStat.h"
#include <mutex>

void CpuCostStat::stat_end(int64_t start)
{
	auto now = getMicroSysTime();
	auto dis = now - start;
	do{
		long total = total_cost_ms.load();
		long next = total + dis;
		if (total_cost_ms.compare_exchange_weak(total, next)){
			break;
		}
	} while (true);
	//start_in = false;
	total_cost_times++;
}

std::vector<CpuCostStat *> GCpuCostStat::stats;
std::thread * GCpuCostStat::thread_dump;
static std::mutex mutex_alloc;

CpuCostStat * GCpuCostStat::allocStat(const char * name)
{
	auto ret = new CpuCostStat(name);
	mutex_alloc.lock();
	stats.push_back(ret);
	mutex_alloc.unlock();
	return ret;
}

void GCpuCostStat::startDumpThread()
{
	if (!thread_dump)
	{
		thread_dump = new std::thread([&]
		{
			while (true)
			{
				dump();
				usleep(1000*1000);
			}
		});
	}
}

void GCpuCostStat::dump()
{
	mutex_alloc.lock();
	for (auto &c : stats){
		long cost = c->getCost();
		long times = c->getCostTimes();
		long dis_times = times - c->last_cost_times;
		if (dis_times > 0){
			printf("%s cost inc=%ld us,avg=%ld us/pt\n", c->getName(), cost - c->last_cost, (cost - c->last_cost) / (times - c->last_cost_times));
			c->last_cost = cost;
			c->last_cost_times = times;
		}
		else{
			printf("%s cost 0£¬locked:%d\n", c->getName(),c->start_in);
		}
	}
	mutex_alloc.unlock();
}