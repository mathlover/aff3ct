#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <algorithm>

#include "Tools/Display/bash_tools.h"

#include "Tools/Factory/Factory_interleaver.hpp"
#include "Module/Encoder/RA/Encoder_RA.hpp"
#include "Module/Decoder/RA/Decoder_RA.hpp"

#include "Simulation_BFER_RA.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
Simulation_BFER_RA<B,R,Q>
::Simulation_BFER_RA(const parameters& params)
: Simulation_BFER<B,R,Q>(params),
  interleaver(nullptr)
{
	assert(params.code.N % params.code.K == 0);

	// build the interleaver for the encoder and the decoder
	interleaver = Factory_interleaver<int>::build(this->params, this->params.code.N, this->rd_engine_seed[0]());
}

template <typename B, typename R, typename Q>
Simulation_BFER_RA<B,R,Q>
::~Simulation_BFER_RA()
{
	delete interleaver;
}

template <typename B, typename R, typename Q>
void Simulation_BFER_RA<B,R,Q>
::launch_precompute()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFER_RA<B,R,Q>
::snr_precompute()
{
}

template <typename B, typename R, typename Q>
Encoder<B>* Simulation_BFER_RA<B,R,Q>
::build_encoder(const int tid)
{
	auto encoder = Simulation_BFER<B,R,Q>::build_encoder(tid);
	if (encoder == nullptr)
		encoder = new Encoder_RA<B>(this->params.code.K, this->params.code.N, *interleaver);
	return encoder;
}

template <typename B, typename R, typename Q>
Decoder<B,Q>* Simulation_BFER_RA<B,R,Q>
::build_decoder(const int tid)
{
	Decoder<B,Q>* decoder = new Decoder_RA<B,Q>(this->params.code.K,
	                                            this->params.code.N,
	                                            *interleaver,
	                                            this->params.decoder.n_ite,
	                                            this->params.simulation.inter_frame_level);
	if (tid == 0)
		interleaver->set_n_frames(decoder->get_n_frames());

	return decoder;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::simulation::Simulation_BFER_RA<B_8,R_8,Q_8>;
template class aff3ct::simulation::Simulation_BFER_RA<B_16,R_16,Q_16>;
template class aff3ct::simulation::Simulation_BFER_RA<B_32,R_32,Q_32>;
template class aff3ct::simulation::Simulation_BFER_RA<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::Simulation_BFER_RA<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
