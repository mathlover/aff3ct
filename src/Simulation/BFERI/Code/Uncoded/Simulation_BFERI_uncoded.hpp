#ifndef SIMULATION_BFERI_UNCODED_HPP_
#define SIMULATION_BFERI_UNCODED_HPP_

#include "Simulation/BFERI/Standard/STD_Simulation_BFERI.hpp"

namespace aff3ct
{
template <typename B, typename R, typename Q>
class Simulation_BFERI_uncoded : public Simulation_BFERI<B,R,Q>
{
public:
	Simulation_BFERI_uncoded(const parameters& params);

	virtual ~Simulation_BFERI_uncoded();

protected:
	void          launch_precompute();
	void          snr_precompute   ();
	Encoder<B>*   build_encoder    (const int tid = 0);
	SISO<Q>*      build_siso       (const int tid = 0);
	Decoder<B,Q>* build_decoder    (const int tid = 0);
};
}

#endif /* SIMULATION_BFERI_UNCODED_HPP_ */
