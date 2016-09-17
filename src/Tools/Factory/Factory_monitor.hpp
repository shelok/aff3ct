#ifndef FACTORY_MONITOR_HPP
#define FACTORY_MONITOR_HPP

#include "Module/Monitor/Monitor.hpp"
#include "Tools/params.h"


template <typename B>
struct Factory_monitor
{
	static Monitor<B>* build(const parameters &params, const int n_frames = 1);
};

#endif /* FACTORY_MONITOR_HPP */