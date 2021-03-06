#ifndef FACTORY_MONITOR_HPP
#define FACTORY_MONITOR_HPP

#include "Module/Monitor/Monitor.hpp"
#include "Tools/params.h"

#include "Factory.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
struct Factory_monitor : public Factory
{
	static module::Monitor<B,R>* build(const parameters &params, const int n_frames = 1);
};
}
}

#endif /* FACTORY_MONITOR_HPP */
