#ifndef SIMULATION_UNCODED_HPP_
#define SIMULATION_UNCODED_HPP_

#include "Simulation/BFER/Standard/STD_Simulation_BFER.hpp"

namespace aff3ct
{
template <typename B, typename R, typename Q>
class Simulation_BFER_uncoded : public Simulation_BFER<B,R,Q>
{
public:
	Simulation_BFER_uncoded(const parameters& params);

	virtual ~Simulation_BFER_uncoded();

protected:
	void          launch_precompute();
	void          snr_precompute   ();
	Encoder<B>*   build_encoder    (const int tid = 0);
	Decoder<B,Q>* build_decoder    (const int tid = 0);
};
}

#endif /* SIMULATION_UNCODED_HPP_ */
