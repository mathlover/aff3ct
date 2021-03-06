#ifndef SIMULATION_EXIT_RSC_HPP_
#define SIMULATION_EXIT_RSC_HPP_

#include <chrono>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Simulation/EXIT/Simulation_EXIT.hpp"

namespace aff3ct
{
namespace simulation
{
template <typename B = int, typename R = float, typename Q = R, typename QD = Q>
class Simulation_EXIT_RSC : public Simulation_EXIT<B,R,Q>
{
protected:
	// the trellis representation
	std::vector<std::vector<int>> trellis;
	
public:
	Simulation_EXIT_RSC(const tools::parameters& params);
	virtual ~Simulation_EXIT_RSC();

protected:
	void extract_sys_par(const mipp::vector<R> &Lch_N, 
	                     const mipp::vector<R> &La_K, 
	                           mipp::vector<R> &sys, 
	                           mipp::vector<R> &par);

	void                launch_precompute();
	void                snr_precompute   ();
	module::Encoder<B>* build_encoder    ();
	module::SISO<R>*    build_siso       ();
};
}
}

#endif /* SIMULATION_EXIT_RSC_HPP_ */
